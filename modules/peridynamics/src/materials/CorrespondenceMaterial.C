//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "CorrespondenceMaterial.h"

#include "libmesh/quadrature.h"

template <>
InputParameters
validParams<CorrespondenceMaterial>()
{
  InputParameters params = validParams<PDMechanicMaterial>();
  params.addClassDescription(
      "Class for computing nodal quantities for residual and jacobian calculation "
      "for Self-stabilized Non-Ordinary State-based PeriDynamic (SNOSPD) "
      "correspondence model under small strain assumptions");

  params.addParam<std::vector<MaterialPropertyName>>(
      "eigenstrain_names", "List of eigenstrains to be applied in this strain calculation");

  return params;
}

CorrespondenceMaterial::CorrespondenceMaterial(const InputParameters & parameters)
  : DerivativeMaterialInterface<PDMechanicMaterial>(parameters),
    _eigenstrain_names(getParam<std::vector<MaterialPropertyName>>("eigenstrain_names")),
    _eigenstrains(_eigenstrain_names.size()),
    _shape_tensor(declareProperty<RankTwoTensor>("shape_tensor")),
    _deformation_gradient(declareProperty<RankTwoTensor>("deformation_gradient")),
    _ddgraddu(declareProperty<RankTwoTensor>("ddeformation_gradient_du")),
    _ddgraddv(declareProperty<RankTwoTensor>("ddeformation_gradient_dv")),
    _ddgraddw(declareProperty<RankTwoTensor>("ddeformation_gradient_dw")),
    _total_strain(declareProperty<RankTwoTensor>("total_strain")),
    _mechanical_strain(declareProperty<RankTwoTensor>("mechanical_strain")),
    _multi(declareProperty<Real>("multi"))
{
  for (unsigned int i = 0; i < _eigenstrains.size(); ++i)
  {
    _eigenstrain_names[i] = _eigenstrain_names[i];
    _eigenstrains[i] = &getMaterialProperty<RankTwoTensor>(_eigenstrain_names[i]);
  }
}

void
CorrespondenceMaterial::initQpStatefulProperties()
{
  _mechanical_strain[_qp].zero();
  _total_strain[_qp].zero();
  _deformation_gradient[_qp].zero();
  _deformation_gradient[_qp].addIa(1.0);
}

void
CorrespondenceMaterial::computeQpDeformationGradient()
{
  _shape_tensor[_qp].zero();
  _deformation_gradient[_qp].zero();
  _ddgraddu[_qp].zero();
  _ddgraddv[_qp].zero();
  _ddgraddw[_qp].zero();

  if (_dim == 2)
    _shape_tensor[_qp](2, 2) = _deformation_gradient[_qp](2, 2) = 1.0;

  Node * cur_nd = _current_elem->get_node(_qp);
  Node * end_nd = _current_elem->get_node(1 - _qp); // two nodes for edge2 element
  std::vector<dof_id_type> neighbors = _pdmesh.neighbors(cur_nd->id());
  std::vector<dof_id_type> bonds = _pdmesh.bonds(cur_nd->id());

  unsigned int nb = std::find(neighbors.begin(), neighbors.end(), end_nd->id()) - neighbors.begin();
  std::vector<unsigned int> dgnodes = _pdmesh.dgNodeInfo(cur_nd->id(), nb);

  if (dgnodes.size() < _dim)
    mooseError("Not enough neighboring bonds for deformation gradient approximation for element: ",
               _current_elem->id());

  // calculate the shape tensor and prepare the deformation gradient tensor
  Real dgnodes_vsum = 0.0;

  RealGradient ori_vec(_dim), cur_vec(_dim);
  ori_vec = 0.0;
  cur_vec = 0.0;

  for (unsigned int j = 0; j < dgnodes.size(); ++j)
  {
    Node * node_j = _mesh.nodePtr(neighbors[dgnodes[j]]);
    Real vol_j = _pdmesh.volume(neighbors[dgnodes[j]]);
    dgnodes_vsum += vol_j;
    for (unsigned int k = 0; k < _dim; ++k)
    {
      ori_vec(k) = _pdmesh.coord(neighbors[dgnodes[j]])(k) - _pdmesh.coord(cur_nd->id())(k);
      cur_vec(k) =
          ori_vec(k) + _disp_var[k]->getNodalValue(*node_j) - _disp_var[k]->getNodalValue(*cur_nd);
    }
    Real ori_len = ori_vec.norm();
    Real bond_status_ij = _bond_status_var.getElementalValue(_pdmesh.elemPtr(bonds[dgnodes[j]]));
    for (unsigned int k = 0; k < _dim; ++k)
    {
      for (unsigned int l = 0; l < _dim; ++l)
      {
        _shape_tensor[_qp](k, l) +=
            _horizon[_qp] / ori_len * ori_vec(k) * ori_vec(l) * vol_j * bond_status_ij;
        _deformation_gradient[_qp](k, l) +=
            _horizon[_qp] / ori_len * cur_vec(k) * ori_vec(l) * vol_j * bond_status_ij;
      }
      // calculate derivatives of deformation_gradient w.r.t displacements of node i
      _ddgraddu[_qp](0, k) += -_horizon[_qp] / ori_len * ori_vec(k) * vol_j * bond_status_ij;
      _ddgraddv[_qp](1, k) += -_horizon[_qp] / ori_len * ori_vec(k) * vol_j * bond_status_ij;
      if (_dim == 3)
        _ddgraddw[_qp](2, k) += -_horizon[_qp] / ori_len * ori_vec(k) * vol_j * bond_status_ij;
    }
  }

  // force state multiplier
  // _multi[_qp] = _horizon[_qp] / _origin_length * _nv[0] * _nv[1] * dgnodes.size() /
  // neighbors.size();
  _multi[_qp] = _horizon[_qp] / _origin_length * _nv[0] * _nv[1] * dgnodes_vsum / _nvsum[_qp];

  // finalize the deformation gradient and its derivatives
  if (_shape_tensor[_qp].det() == 0)
  {
    _shape_tensor[_qp] = RankTwoTensor::initIdentity;
    _deformation_gradient[_qp] = RankTwoTensor::initIdentity;
    _ddgraddu[_qp].zero();
    _ddgraddv[_qp].zero();
    _ddgraddw[_qp].zero();
  }
  else
  {
    _deformation_gradient[_qp] *= _shape_tensor[_qp].inverse();
    _ddgraddu[_qp] *= _shape_tensor[_qp].inverse();
    _ddgraddv[_qp] *= _shape_tensor[_qp].inverse();
    _ddgraddw[_qp] *= _shape_tensor[_qp].inverse();
  }
}

void
CorrespondenceMaterial::computeProperties()
{
  PDMechanicMaterial::computeProperties();

  for (_qp = 0; _qp < _qrule->n_points(); ++_qp)
    computeQpStrain();
}
