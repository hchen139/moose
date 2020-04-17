//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "WeightStabilizedFiniteStrainMechanicsNOSPD.h"

registerMooseObject("PeridynamicsApp", WeightStabilizedFiniteStrainMechanicsNOSPD);

InputParameters
WeightStabilizedFiniteStrainMechanicsNOSPD::validParams()
{
  InputParameters params = MechanicsFiniteStrainBaseNOSPD::validParams();
  params.addClassDescription(
      "Class for calculating the residual and the Jacobian for the weight-stabilized peridynamic "
      "correspondence model under finite strain assumptions");

  params.addParam<unsigned int>(
      "weight_function_power",
      3,
      "The power of cosine weight function used to stabilized the correpondence model");
  params.addRequiredParam<unsigned int>(
      "component",
      "An integer corresponding to the variable this kernel acts on (0 for x, 1 for y, 2 for z)");

  return params;
}

WeightStabilizedFiniteStrainMechanicsNOSPD::WeightStabilizedFiniteStrainMechanicsNOSPD(
    const InputParameters & parameters)
  : MechanicsFiniteStrainBaseNOSPD(parameters),
    _pow(getParam<unsigned int>("weight_function_power")),
    _component(getParam<unsigned int>("component"))
{
}

void
WeightStabilizedFiniteStrainMechanicsNOSPD::computeLocalResidual()
{
  // For finite strain formulation, the _stress tensor gotten from material class is the
  // Cauchy stress (Sigma). the first Piola-Kirchhoff stress (P) is then obtained as
  // P = J * Sigma * inv(F)^T.
  // Nodal force states are based on the first Piola-Kirchhoff stress tensors (P).
  // i.e., T = (J * Sigma * inv(F)^T) * inv(Shape) * xi * multi.
  // Cauchy stress is calculated as Sigma_n+1 = Sigma_n + R * (C * dt * D) * R^T

  for (unsigned int nd = 0; nd < _nnodes; ++nd)
    for (_i = 0; _i < _nnodes; ++_i)
      _local_re(_i) += (_i == 0 ? -1 : 1) * _multi[nd] *
                       ((_dgrad[nd].det() * _stress[nd] * _dgrad[nd].inverse().transpose()) *
                        _shape2[nd].inverse())
                           .row(_component) *
                       _origin_vec * _bond_status;
}

void
WeightStabilizedFiniteStrainMechanicsNOSPD::computeNonlocalResidual()
{
  for (unsigned int nd = 0; nd < _nnodes; ++nd)
  {
    // calculation of residual contribution to current_node's neighbors
    std::vector<dof_id_type> ivardofs(_nnodes);
    ivardofs[0] = _ivardofs[nd];
    std::vector<dof_id_type> neighbors = _pdmesh.getNeighbors(_current_elem->node_id(nd));
    std::vector<dof_id_type> bonds = _pdmesh.getBonds(_current_elem->node_id(nd));

    RealGradient origin_vec_nb;
    Real inner_prod_nb, weight_nb;

    for (unsigned int nb = 0; nb < neighbors.size(); ++nb)
      if (neighbors[nb] != _current_elem->node_id(1 - nd) &&
          _bond_status_var->getElementalValue(_pdmesh.elemPtr(bonds[nb])) >
              0.5) // exclude the other node of current element from nonlocal residual, which is
                   // considered in the local residual
      {
        ivardofs[1] = _pdmesh.nodePtr(neighbors[nb])->dof_number(_sys.number(), _var.number(), 0);
        origin_vec_nb =
            *_pdmesh.nodePtr(neighbors[nb]) - *_pdmesh.nodePtr(_current_elem->node_id(nd));

        inner_prod_nb = 0;
        for (_i = 0; _i < _dim; ++_i)
          inner_prod_nb += (nd == 0 ? 1 : -1) * _origin_vec(_i) * origin_vec_nb(_i);

        weight_nb = 1.0;
        for (_i = 0; _i < _pow; ++_i)
          weight_nb *= 0.5 + 0.5 * inner_prod_nb / (_origin_vec.norm() * origin_vec_nb.norm());

        for (_i = 0; _i < _nnodes; ++_i)
          _local_re(_i) = (_i == 0 ? -1 : 1) * _multi[nd] * weight_nb *
                          ((_dgrad[nd].det() * _stress[nd] * _dgrad[nd].inverse().transpose()) *
                           _shape2[nd].inverse())
                              .row(_component) *
                          origin_vec_nb * _bond_status;

        // cache the residual contribution
        _assembly.cacheResidualNodes(_local_re, ivardofs);

        // save in the displacement residuals
        if (_has_save_in)
        {
          Threads::spin_mutex::scoped_lock lock(Threads::spin_mtx);
          for (_i = 0; _i < _save_in.size(); ++_i)
          {
            std::vector<dof_id_type> save_in_dofs(_nnodes);
            save_in_dofs[0] = _current_elem->node_ptr(nd)->dof_number(
                _save_in[_i]->sys().number(), _save_in[_i]->number(), 0);
            save_in_dofs[1] =
                _pdmesh.nodePtr(neighbors[nb])
                    ->dof_number(_save_in[_i]->sys().number(), _save_in[_i]->number(), 0);

            _save_in[_i]->sys().solution().add_vector(_local_re, save_in_dofs);
          }
        }
      }
  }
}

void
WeightStabilizedFiniteStrainMechanicsNOSPD::computeLocalJacobian()
{
  for (unsigned int nd = 0; nd < _nnodes; ++nd)
  {
    std::vector<dof_id_type> ivardofs(_nnodes);
    ivardofs[0] = _current_elem->node_ptr(nd)->dof_number(_sys.number(), _var.number(), 0);
    std::vector<dof_id_type> neighbors = _pdmesh.getNeighbors(_current_elem->node_id(nd));
    std::vector<dof_id_type> bonds = _pdmesh.getBonds(_current_elem->node_id(nd));

    RankTwoTensor dPxdUx =
        computeDJDU(_component, nd) * _stress[nd] * _dgrad[nd].inverse().transpose() +
        _dgrad[nd].det() * computeDSDU(_component, nd) * _dgrad[nd].inverse().transpose() +
        _dgrad[nd].det() * _stress[nd] * computeDinvFTDU(_component, nd);

    RealGradient origin_vec_nb;
    Real inner_prod_nb, weight_nb;

    for (unsigned int nb = 0; nb < neighbors.size(); ++nb)
      if (_bond_status_var->getElementalValue(_pdmesh.elemPtr(bonds[nb])) > 0.5)
      {
        ivardofs[1] = _pdmesh.nodePtr(neighbors[nb])->dof_number(_sys.number(), _var.number(), 0);
        origin_vec_nb =
            *_pdmesh.nodePtr(neighbors[nb]) - *_pdmesh.nodePtr(_current_elem->node_id(nd));

        inner_prod_nb = 0;
        for (_i = 0; _i < _dim; ++_i)
          inner_prod_nb += (nd == 0 ? 1 : -1) * _origin_vec(_i) * origin_vec_nb(_i);

        weight_nb = 1.0;
        for (_i = 0; _i < _pow; ++_i)
          weight_nb *= 0.5 + 0.5 * inner_prod_nb / (_origin_vec.norm() * origin_vec_nb.norm());

        for (_i = 0; _i < _nnodes; ++_i)
          for (_j = 0; _j < _nnodes; ++_j)
            _local_ke(_i, _j) = (_i == 0 ? -1 : 1) * (_j == 0 ? 1 : 0) * _multi[nd] * weight_nb *
                                (dPxdUx * _shape2[nd].inverse()).row(_component) * origin_vec_nb *
                                _bond_status;

        _assembly.cacheJacobianBlock(_local_ke, ivardofs, ivardofs, _var.scalingFactor());
      }
  }
  _local_ke.zero();
}

void
WeightStabilizedFiniteStrainMechanicsNOSPD::computeNonlocalJacobian()
{
  // includes dTi/dUj and dTj/dUi contributions
  for (unsigned int nd = 0; nd < _nnodes; ++nd)
  {
    RankFourTensor dSdFhat = computeDSDFhat(nd);
    RankTwoTensor invF = _dgrad[nd].inverse();
    Real detF = _dgrad[nd].det();
    // calculation of jacobian contribution to current_node's neighbors
    std::vector<dof_id_type> ivardofs(_nnodes), jvardofs(_nnodes);
    ivardofs[0] = _current_elem->node_ptr(nd)->dof_number(_sys.number(), _var.number(), 0);
    jvardofs[0] = _current_elem->node_ptr(nd)->dof_number(_sys.number(), _var.number(), 0);
    std::vector<dof_id_type> neighbors = _pdmesh.getNeighbors(_current_elem->node_id(nd));
    std::vector<dof_id_type> bonds = _pdmesh.getBonds(_current_elem->node_id(nd));

    Real inner_prod_nb1, weight_nb1;
    RealGradient origin_vec_nb1;

    for (unsigned int nb1 = 0; nb1 < neighbors.size(); ++nb1)
      if (_bond_status_var->getElementalValue(_pdmesh.elemPtr(bonds[nb1])) > 0.5)
      {
        ivardofs[1] = _pdmesh.nodePtr(neighbors[nb1])->dof_number(_sys.number(), _var.number(), 0);
        origin_vec_nb1 =
            *_pdmesh.nodePtr(neighbors[nb1]) - *_pdmesh.nodePtr(_current_elem->node_id(nd));

        inner_prod_nb1 = 0;
        for (_i = 0; _i < _dim; ++_i)
          inner_prod_nb1 += (nd == 0 ? 1 : -1) * _origin_vec(_i) * origin_vec_nb1(_i);

        weight_nb1 = 1.0;
        for (_i = 0; _i < _pow; ++_i)
          weight_nb1 *= 0.5 + 0.5 * inner_prod_nb1 / (_origin_vec.norm() * origin_vec_nb1.norm());

        Real vol_nb2, inner_prod_nb2, weight_nb2, dJdU;
        RealGradient origin_vec_nb2;
        RankTwoTensor dFdUk, dPxdUkx, dSdU, dinvFTdU;

        for (unsigned int nb2 = 0; nb2 < neighbors.size(); ++nb2)
          if (_bond_status_var->getElementalValue(_pdmesh.elemPtr(bonds[nb2])) > 0.5)
          {
            ivardofs[1] =
                _pdmesh.nodePtr(neighbors[nb2])->dof_number(_sys.number(), _var.number(), 0);
            vol_nb2 = _pdmesh.getNodeVolume(neighbors[nb2]);

            origin_vec_nb2 =
                *_pdmesh.nodePtr(neighbors[nb2]) - *_pdmesh.nodePtr(_current_elem->node_id(nd));

            inner_prod_nb2 = 0;
            for (_i = 0; _i < _dim; ++_i)
              inner_prod_nb2 += (nd == 0 ? 1 : -1) * _origin_vec(_i) * origin_vec_nb2(_i);

            weight_nb2 = 1.0;
            for (_i = 0; _i < _pow; ++_i)
              weight_nb2 *=
                  0.5 + 0.5 * inner_prod_nb2 / (_origin_vec.norm() * origin_vec_nb2.norm());

            dFdUk.zero();
            for (_i = 0; _i < _dim; ++_i)
              dFdUk(_component, _i) = weight_nb2 * origin_vec_nb2(_i) * vol_nb2;

            dFdUk *= _shape2[nd].inverse();

            // calculate dJ/du
            dJdU = 0.0;
            for (_i = 0; _i < 3; ++_i)
              for (_j = 0; _j < 3; ++_j)
                dJdU += detF * invF(_j, _i) * dFdUk(_i, _j);

            // calculate dS/du
            dSdU = dSdFhat * dFdUk * _dgrad_old[nd].inverse();

            // calculate dinv(F)Tdu
            dinvFTdU.zero();
            for (unsigned int i = 0; i < 3; ++i)
              for (unsigned int J = 0; J < 3; ++J)
                for (unsigned int k = 0; k < 3; ++k)
                  for (unsigned int L = 0; L < 3; ++L)
                    dinvFTdU(i, J) += -invF(J, k) * invF(L, i) * dFdUk(k, L);

            // calculate the derivative of first Piola-Kirchhoff stress w.r.t displacements
            dPxdUkx = dJdU * _stress[nd] * invF.transpose() + detF * dSdU * invF.transpose() +
                      detF * _stress[nd] * dinvFTdU;

            for (_i = 0; _i < _nnodes; ++_i)
              for (_j = 0; _j < _nnodes; ++_j)
                _local_ke(_i, _j) = (_i == 0 ? -1 : 1) * (_j == 0 ? 0 : 1) * _multi[nd] *
                                    weight_nb1 * (dPxdUkx * _shape2[nd].inverse()).row(_component) *
                                    origin_vec_nb1 * _bond_status;

            _assembly.cacheJacobianBlock(_local_ke, ivardofs, jvardofs, _var.scalingFactor());

            if (_has_diag_save_in)
            {
              unsigned int rows = _nnodes;
              DenseVector<Real> diag(rows);
              for (_i = 0; _i < rows; ++_i)
                diag(_i) = _local_ke(_i, _i);

              Threads::spin_mutex::scoped_lock lock(Threads::spin_mtx);
              for (_i = 0; _i < _diag_save_in.size(); ++_i)
              {
                std::vector<dof_id_type> diag_save_in_dofs(2);
                diag_save_in_dofs[0] = _current_elem->node_ptr(nd)->dof_number(
                    _diag_save_in[_i]->sys().number(), _diag_save_in[_i]->number(), 0);
                diag_save_in_dofs[1] = _pdmesh.nodePtr(neighbors[nb2])
                                           ->dof_number(_diag_save_in[_i]->sys().number(),
                                                        _diag_save_in[_i]->number(),
                                                        0);

                _diag_save_in[_i]->sys().solution().add_vector(diag, diag_save_in_dofs);
              }
            }
          }
      }
  }
}

void
WeightStabilizedFiniteStrainMechanicsNOSPD::computeLocalOffDiagJacobian(
    unsigned int jvar_num, unsigned int coupled_component)
{
  if (coupled_component == 3) // temperature is coupled
  {
    std::vector<RankTwoTensor> dSdT(_nnodes);
    for (unsigned int nd = 0; nd < _nnodes; ++nd)
      for (unsigned int es = 0; es < _deigenstrain_dT.size(); ++es)
        dSdT[nd] = -_dgrad[nd].det() * _Jacobian_mult[nd] * (*_deigenstrain_dT[es])[nd] *
                   _dgrad[nd].inverse().transpose();

    for (unsigned int nd = 0; nd < _nnodes; ++nd)
    {
      std::vector<dof_id_type> ivardofs(_nnodes), jvardofs(_nnodes);
      ivardofs[0] = _current_elem->node_ptr(nd)->dof_number(_sys.number(), _var.number(), 0);
      jvardofs[0] = _current_elem->node_ptr(nd)->dof_number(_sys.number(), jvar_num, 0);
      std::vector<dof_id_type> neighbors = _pdmesh.getNeighbors(_current_elem->node_id(nd));
      std::vector<dof_id_type> bonds = _pdmesh.getBonds(_current_elem->node_id(nd));

      Real inner_prod_nb, weight_nb;
      RealGradient origin_vec_nb;

      for (unsigned int nb = 0; nb < neighbors.size(); ++nb)
        if (_bond_status_var->getElementalValue(_pdmesh.elemPtr(bonds[nb])) > 0.5)
        {
          ivardofs[1] = _pdmesh.nodePtr(neighbors[nb])->dof_number(_sys.number(), _var.number(), 0);
          jvardofs[1] = _pdmesh.nodePtr(neighbors[nb])->dof_number(_sys.number(), jvar_num, 0);
          origin_vec_nb =
              *_pdmesh.nodePtr(neighbors[nb]) - *_pdmesh.nodePtr(_current_elem->node_id(nd));

          inner_prod_nb = 0;
          for (_i = 0; _i < _dim; ++_i)
            inner_prod_nb += (nd == 0 ? 1 : -1) * _origin_vec(_i) * origin_vec_nb(_i);

          weight_nb = 1.0;
          for (_i = 0; _i < _pow; ++_i)
            weight_nb *= 0.5 + 0.5 * inner_prod_nb / (_origin_vec.norm() * origin_vec_nb.norm());

          for (_i = 0; _i < _nnodes; ++_i)
            for (_j = 0; _j < _nnodes; ++_j)
              _local_ke(_i, _j) = (_i == 0 ? -1 : 1) * (_j == 0 ? 1 : 0) * _multi[nd] * weight_nb *
                                  (dSdT[nd] * _shape2[nd].inverse()).row(_component) *
                                  origin_vec_nb * _bond_status;

          _assembly.cacheJacobianBlock(_local_ke, ivardofs, jvardofs, _var.scalingFactor());
        }
    }
    _local_ke.zero();
  }
  else if (coupled_component == 4) // weak plane stress case, out_of_plane_strain is coupled
  {
    std::vector<RankTwoTensor> dSdE33(_nnodes);
    for (unsigned int nd = 0; nd < _nnodes; ++nd)
    {
      for (_i = 0; _i < 3; ++_i)
        for (_j = 0; _j < 3; ++_j)
          dSdE33[nd](_i, _j) = _Jacobian_mult[nd](_i, _j, 2, 2);

      dSdE33[nd] = _dgrad[nd].det() * dSdE33[nd] * _dgrad[nd].inverse().transpose();
    }

    for (unsigned int nd = 0; nd < _nnodes; ++nd)
    {
      std::vector<dof_id_type> ivardofs(_nnodes), jvardofs(_nnodes);
      ivardofs[0] = _current_elem->node_ptr(nd)->dof_number(_sys.number(), _var.number(), 0);
      jvardofs[0] = _current_elem->node_ptr(nd)->dof_number(_sys.number(), jvar_num, 0);
      std::vector<dof_id_type> neighbors = _pdmesh.getNeighbors(_current_elem->node_id(nd));
      std::vector<dof_id_type> bonds = _pdmesh.getBonds(_current_elem->node_id(nd));

      Real inner_prod_nb, weight_nb;
      RealGradient origin_vec_nb;

      for (unsigned int nb = 0; nb < neighbors.size(); ++nb)
        if (_bond_status_var->getElementalValue(_pdmesh.elemPtr(bonds[nb])) > 0.5)
        {
          ivardofs[1] = _pdmesh.nodePtr(neighbors[nb])->dof_number(_sys.number(), _var.number(), 0);
          jvardofs[1] = _pdmesh.nodePtr(neighbors[nb])->dof_number(_sys.number(), jvar_num, 0);
          origin_vec_nb =
              *_pdmesh.nodePtr(neighbors[nb]) - *_pdmesh.nodePtr(_current_elem->node_id(nd));

          inner_prod_nb = 0;
          for (_i = 0; _i < _dim; ++_i)
            inner_prod_nb += (nd == 0 ? 1 : -1) * _origin_vec(_i) * origin_vec_nb(_i);

          weight_nb = 1.0;
          for (_i = 0; _i < _pow; ++_i)
            weight_nb *= 0.5 + 0.5 * inner_prod_nb / (_origin_vec.norm() * origin_vec_nb.norm());

          for (_i = 0; _i < _nnodes; ++_i)
            for (_j = 0; _j < _nnodes; ++_j)
              _local_ke(_i, _j) = (_i == 0 ? -1 : 1) * (_j == 0 ? 1 : 0) * _multi[nd] *
                                  _horizon_radius[nd] / origin_vec_nb.norm() *
                                  (dSdE33[nd] * _shape2[nd].inverse()).row(_component) *
                                  origin_vec_nb * _bond_status;

          _assembly.cacheJacobianBlock(_local_ke, ivardofs, jvardofs, _var.scalingFactor());
        }
    }
    _local_ke.zero();
  }
  else // off-diagonal Jacobian with respect to other displacement variables
  {
    for (unsigned int nd = 0; nd < _nnodes; ++nd)
    {
      std::vector<dof_id_type> ivardofs(_nnodes), jvardofs(_nnodes);
      ivardofs[0] = _current_elem->node_ptr(nd)->dof_number(_sys.number(), _var.number(), 0);
      jvardofs[0] = _current_elem->node_ptr(nd)->dof_number(_sys.number(), jvar_num, 0);
      std::vector<dof_id_type> neighbors = _pdmesh.getNeighbors(_current_elem->node_id(nd));
      std::vector<dof_id_type> bonds = _pdmesh.getBonds(_current_elem->node_id(nd));

      RankTwoTensor dPxdUy =
          computeDJDU(coupled_component, nd) * _stress[nd] * _dgrad[nd].inverse().transpose() +
          _dgrad[nd].det() * computeDSDU(coupled_component, nd) * _dgrad[nd].inverse().transpose() +
          _dgrad[nd].det() * _stress[nd] * computeDinvFTDU(coupled_component, nd);

      Real inner_prod_nb, weight_nb;
      RealGradient origin_vec_nb;

      for (unsigned int nb = 0; nb < neighbors.size(); ++nb)
        if (_bond_status_var->getElementalValue(_pdmesh.elemPtr(bonds[nb])) > 0.5)
        {
          ivardofs[1] = _pdmesh.nodePtr(neighbors[nb])->dof_number(_sys.number(), _var.number(), 0);
          jvardofs[1] = _pdmesh.nodePtr(neighbors[nb])->dof_number(_sys.number(), jvar_num, 0);
          origin_vec_nb =
              *_pdmesh.nodePtr(neighbors[nb]) - *_pdmesh.nodePtr(_current_elem->node_id(nd));

          inner_prod_nb = 0;
          for (_i = 0; _i < _dim; ++_i)
            inner_prod_nb += (nd == 0 ? 1 : -1) * _origin_vec(_i) * origin_vec_nb(_i);

          weight_nb = 1.0;
          for (_i = 0; _i < _pow; ++_i)
            weight_nb *= 0.5 + 0.5 * inner_prod_nb / (_origin_vec.norm() * origin_vec_nb.norm());

          for (_i = 0; _i < _nnodes; ++_i)
            for (_j = 0; _j < _nnodes; ++_j)
              _local_ke(_i, _j) = (_i == 0 ? -1 : 1) * (_j == 0 ? 1 : 0) * _multi[nd] * weight_nb *
                                  (dPxdUy * _shape2[nd].inverse()).row(_component) * origin_vec_nb *
                                  _bond_status;

          _assembly.cacheJacobianBlock(_local_ke, ivardofs, jvardofs, _var.scalingFactor());
        }
    }
    _local_ke.zero();
  }
}

void
WeightStabilizedFiniteStrainMechanicsNOSPD::computePDNonlocalOffDiagJacobian(
    unsigned int jvar_num, unsigned int coupled_component)
{
  if (coupled_component != 3 && coupled_component != 4)
  {
    for (unsigned int nd = 0; nd < _nnodes; ++nd)
    {
      RankFourTensor dSdFhat = computeDSDFhat(nd);
      RankTwoTensor invF = _dgrad[nd].inverse();
      Real detF = _dgrad[nd].det();
      // calculation of jacobian contribution to current_node's neighbors
      // NOT including the contribution to nodes i and j, which is considered as local off-diagonal
      std::vector<dof_id_type> ivardofs(_nnodes), jvardofs(_nnodes);
      ivardofs[0] = _current_elem->node_ptr(nd)->dof_number(_sys.number(), _var.number(), 0);
      jvardofs[0] = _current_elem->node_ptr(nd)->dof_number(_sys.number(), jvar_num, 0);
      std::vector<dof_id_type> neighbors = _pdmesh.getNeighbors(_current_elem->node_id(nd));
      std::vector<dof_id_type> bonds = _pdmesh.getBonds(_current_elem->node_id(nd));

      Real inner_prod_nb1, weight_nb1;
      RealGradient origin_vec_nb1;

      for (unsigned int nb1 = 0; nb1 < neighbors.size(); ++nb1)
        if (_bond_status_var->getElementalValue(_pdmesh.elemPtr(bonds[nb1])) > 0.5)
        {
          ivardofs[1] =
              _pdmesh.nodePtr(neighbors[nb1])->dof_number(_sys.number(), _var.number(), 0);
          origin_vec_nb1 =
              *_pdmesh.nodePtr(neighbors[nb1]) - *_pdmesh.nodePtr(_current_elem->node_id(nd));

          inner_prod_nb1 = 0;
          for (_i = 0; _i < _dim; ++_i)
            inner_prod_nb1 += (nd == 0 ? 1 : -1) * _origin_vec(_i) * origin_vec_nb1(_i);

          weight_nb1 = 1.0;
          for (_i = 0; _i < _pow; ++_i)
            weight_nb1 *= 0.5 + 0.5 * inner_prod_nb1 / (_origin_vec.norm() * origin_vec_nb1.norm());

          Real vol_nb2, inner_prod_nb2, weight_nb2, dJdU;
          RealGradient origin_vec_nb2;
          RankTwoTensor dFdUk, dPxdUky, dSdU, dinvFTdU;

          for (unsigned int nb2 = 0; nb2 < neighbors.size(); ++nb2)
            if (_bond_status_var->getElementalValue(_pdmesh.elemPtr(bonds[nb2])) > 0.5)
            {
              jvardofs[1] = _pdmesh.nodePtr(neighbors[nb2])->dof_number(_sys.number(), jvar_num, 0);
              vol_nb2 = _pdmesh.getNodeVolume(neighbors[nb2]);

              origin_vec_nb2 =
                  *_pdmesh.nodePtr(neighbors[nb2]) - *_pdmesh.nodePtr(_current_elem->node_id(nd));

              inner_prod_nb2 = 0;
              for (_i = 0; _i < _dim; ++_i)
                inner_prod_nb2 += (nd == 0 ? 1 : -1) * _origin_vec(_i) * origin_vec_nb2(_i);

              weight_nb2 = 1.0;
              for (_i = 0; _i < _pow; ++_i)
                weight_nb2 *=
                    0.5 + 0.5 * inner_prod_nb2 / (_origin_vec.norm() * origin_vec_nb2.norm());

              dFdUk.zero();
              for (_i = 0; _i < _dim; ++_i)
                dFdUk(coupled_component, _i) = weight_nb2 * origin_vec_nb2(_i) * vol_nb2;

              dFdUk *= _shape2[nd].inverse();

              // calculate dJ/du
              dJdU = 0.0;
              for (_i = 0; _i < 3; ++_i)
                for (_j = 0; _j < 3; ++_j)
                  dJdU += detF * invF(_j, _i) * dFdUk(_i, _j);

              // calculate dS/du
              dSdU = dSdFhat * dFdUk * _dgrad_old[nd].inverse();

              // calculate dinv(F)Tdu
              dinvFTdU.zero();
              for (unsigned int i = 0; i < 3; ++i)
                for (unsigned int J = 0; J < 3; ++J)
                  for (unsigned int k = 0; k < 3; ++k)
                    for (unsigned int L = 0; L < 3; ++L)
                      dinvFTdU(i, J) += -invF(J, k) * invF(L, i) * dFdUk(k, L);

              // calculate the derivative of first Piola-Kirchhoff stress w.r.t displacements
              dPxdUky = dJdU * _stress[nd] * invF.transpose() + detF * dSdU * invF.transpose() +
                        detF * _stress[nd] * dinvFTdU;

              for (_i = 0; _i < _nnodes; ++_i)
                for (_j = 0; _j < _nnodes; ++_j)
                  _local_ke(_i, _j) = (_i == 0 ? -1 : 1) * (_j == 0 ? 0 : 1) * _multi[nd] *
                                      weight_nb1 *
                                      (dPxdUky * _shape2[nd].inverse()).row(_component) *
                                      origin_vec_nb1 * _bond_status;

              _assembly.cacheJacobianBlock(_local_ke, ivardofs, jvardofs, _var.scalingFactor());
            }
        }
    }
  }
}
