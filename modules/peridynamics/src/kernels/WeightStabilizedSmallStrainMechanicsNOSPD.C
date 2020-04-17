//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "WeightStabilizedSmallStrainMechanicsNOSPD.h"

registerMooseObject("PeridynamicsApp", WeightStabilizedSmallStrainMechanicsNOSPD);

InputParameters
WeightStabilizedSmallStrainMechanicsNOSPD::validParams()
{
  InputParameters params = MechanicsBaseNOSPD::validParams();
  params.addClassDescription(
      "Class for calculating the residual and Jacobian for weight-stabilized peridynamic "
      "correspondence model under small strain assumptions");

  params.addParam<unsigned int>(
      "weight_function_power",
      3,
      "The power of cosine weight function used to stabilized the correpondence model");
  params.addRequiredParam<unsigned int>(
      "component",
      "An integer corresponding to the variable this kernel acts on (0 for x, 1 for y, 2 for z)");

  return params;
}

WeightStabilizedSmallStrainMechanicsNOSPD::WeightStabilizedSmallStrainMechanicsNOSPD(
    const InputParameters & parameters)
  : MechanicsBaseNOSPD(parameters),
    _pow(getParam<unsigned int>("weight_function_power")),
    _component(getParam<unsigned int>("component"))
{
}

void
WeightStabilizedSmallStrainMechanicsNOSPD::computeLocalResidual()
{
  // For small strain assumptions, stress measures, i.e., Cauchy stress (Sigma), the first
  // Piola-Kirchhoff stress (P), and the second Piola-Kirchhoff stress (S) are approximately the
  // same. Thus, the nodal force state tensors are calculated using the Cauchy stresses,
  // i.e., T = Sigma * inv(Shape) * xi * multi.
  // Cauchy stress is calculated as Sigma = C * E.

  for (unsigned int nd = 0; nd < _nnodes; ++nd)
    for (_i = 0; _i < _nnodes; ++_i)
      _local_re(_i) += (_i == 0 ? -1 : 1) * _multi[nd] *
                       (_stress[nd] * _shape2[nd].inverse()).row(_component) * _origin_vec *
                       _bond_status;
}

void
WeightStabilizedSmallStrainMechanicsNOSPD::computeNonlocalResidual()
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
                          (_stress[nd] * _shape2[nd].inverse()).row(_component) * origin_vec_nb *
                          _bond_status;

        // cache the residual contribution
        _assembly.cacheResidualNodes(_local_re, ivardofs);

        // save in the nonlocal residuals
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
WeightStabilizedSmallStrainMechanicsNOSPD::computeLocalJacobian()
{
  for (unsigned int nd = 0; nd < _nnodes; ++nd)
  {
    std::vector<dof_id_type> ivardofs(_nnodes);
    ivardofs[0] = _current_elem->node_ptr(nd)->dof_number(_sys.number(), _var.number(), 0);
    std::vector<dof_id_type> neighbors = _pdmesh.getNeighbors(_current_elem->node_id(nd));
    std::vector<dof_id_type> bonds = _pdmesh.getBonds(_current_elem->node_id(nd));

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
            _local_ke(_i, _j) =
                (_i == 0 ? -1 : 1) * (_j == 0 ? 1 : 0) * _multi[nd] * weight_nb *
                (computeDSDU(_component, nd) * _shape2[nd].inverse()).row(_component) *
                origin_vec_nb * _bond_status;

        _assembly.cacheJacobianBlock(_local_ke, ivardofs, ivardofs, _var.scalingFactor());
      }
  }
  _local_ke.zero();
}

void
WeightStabilizedSmallStrainMechanicsNOSPD::computeNonlocalJacobian()
{
  // include dTi/dUj and dTj/dUi contributions
  for (unsigned int nd = 0; nd < _nnodes; ++nd)
  {
    std::vector<dof_id_type> ivardofs(_nnodes), jvardofs(_nnodes);
    ivardofs[0] = _current_elem->node_ptr(nd)->dof_number(_sys.number(), _var.number(), 0);
    jvardofs[0] = _current_elem->node_ptr(nd)->dof_number(_sys.number(), _var.number(), 0);
    std::vector<dof_id_type> neighbors = _pdmesh.getNeighbors(_current_elem->node_id(nd));
    std::vector<dof_id_type> bonds = _pdmesh.getBonds(_current_elem->node_id(nd));

    RealGradient origin_vec_nb1;
    Real inner_prod_nb1, weight_nb1;

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

        Real vol_nb2, inner_prod_nb2, weight_nb2;
        RealGradient origin_vec_nb2;
        RankTwoTensor dFdUk, dPxdUkx;

        for (unsigned int nb2 = 0; nb2 < neighbors.size(); ++nb2)
          if (_bond_status_var->getElementalValue(_pdmesh.elemPtr(bonds[nb2])) > 0.5)
          {
            jvardofs[1] =
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
            dPxdUkx = _Jacobian_mult[nd] * 0.5 * (dFdUk.transpose() + dFdUk);

            // this excludes the contribute made by local jacobian
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
WeightStabilizedSmallStrainMechanicsNOSPD::computeLocalOffDiagJacobian(
    unsigned int jvar_num, unsigned int coupled_component)
{
  if (coupled_component == 3)
  {
    std::vector<RankTwoTensor> dSdT(_nnodes);
    for (unsigned int nd = 0; nd < _nnodes; ++nd)
      for (unsigned int es = 0; es < _deigenstrain_dT.size(); ++es)
        dSdT[nd] = -(_Jacobian_mult[nd] * (*_deigenstrain_dT[es])[nd]);

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
      for (_i = 0; _i < 3; ++_i)
        for (_j = 0; _j < 3; ++_j)
          dSdE33[nd](_i, _j) = _Jacobian_mult[nd](_i, _j, 2, 2);

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
              _local_ke(_i, _j) =
                  (_i == 0 ? -1 : 1) * (_j == 0 ? 1 : 0) * _multi[nd] * weight_nb *
                  (computeDSDU(coupled_component, nd) * _shape2[nd].inverse()).row(_component) *
                  origin_vec_nb * _bond_status;

          _assembly.cacheJacobianBlock(_local_ke, ivardofs, jvardofs, _var.scalingFactor());
        }
    }
    _local_ke.zero();
  }
}

void
WeightStabilizedSmallStrainMechanicsNOSPD::computePDNonlocalOffDiagJacobian(
    unsigned int jvar_num, unsigned int coupled_component)
{
  if (coupled_component != 3 && coupled_component != 4)
  {
    for (unsigned int nd = 0; nd < _nnodes; ++nd)
    {
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

          Real vol_nb2, inner_prod_nb2, weight_nb2;
          RealGradient origin_vec_nb2;
          RankTwoTensor dFdUk, dPxdUky;

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
              for (unsigned int k = 0; k < _dim; ++k)
                dFdUk(coupled_component, k) = weight_nb2 * origin_vec_nb2(k) * vol_nb2;

              dFdUk *= _shape2[nd].inverse();
              dPxdUky = _Jacobian_mult[nd] * 0.5 * (dFdUk.transpose() + dFdUk);

              // this excludes the contribute made by local off diag jacobian
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
