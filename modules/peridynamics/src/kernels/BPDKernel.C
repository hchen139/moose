//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "BPDKernel.h"

registerMooseObject("PeridynamicsApp", BPDKernel);

template <>
InputParameters
validParams<BPDKernel>()
{
  InputParameters params = validParams<PDMechanicKernel>();
  params.addClassDescription("Class for calculating residual and jacobian for Bond-based "
                             "PeriDynamic solid mechanics formulation");

  params.addRequiredParam<unsigned int>(
      "component",
      "An integer corresponding to the variable this kernel acts on (0 for x, 1 for y, 2 for z)");

  return params;
}

BPDKernel::BPDKernel(const InputParameters & parameters)
  : PDMechanicKernel(parameters),
    _bond_force_ij(getMaterialProperty<Real>("bond_force_ij")),
    _bond_dfdU_ij(getMaterialProperty<Real>("bond_dfdU_ij")),
    _bond_dfdT_ij(getMaterialProperty<Real>("bond_dfdT_ij")),
    _component(getParam<unsigned int>("component"))
{
}

void
BPDKernel::computeLocalResidual()
{
  _local_re(0) = -_bond_force_ij[0] * _cur_ori_ij(_component) * _bond_status_ij;
  _local_re(1) = -_local_re(0);

  // if (_origin_vec_ij.norm() > 1.0001 * _horizons_ij[0])
  //   _local_re(0) = 0;
  // else if (_origin_vec_ij.norm() > 1.0001 * _horizons_ij[1])
  //   _local_re(1) = 0;
}

void
BPDKernel::computeLocalJacobian()
{
  Real diag =
      _cur_ori_ij(_component) * _cur_ori_ij(_component) * _bond_dfdU_ij[0] +
      _bond_force_ij[0] * (1.0 - _cur_ori_ij(_component) * _cur_ori_ij(_component)) / _cur_len_ij;

  for (_i = 0; _i < _test.size(); _i++)
    for (_j = 0; _j < _phi.size(); _j++)
      _local_ke(_i, _j) += (_i == _j ? 1 : -1) * diag * _bond_status_ij;
}

void
BPDKernel::computeLocalOffDiagJacobian(unsigned int coupled_component)
{
  if (coupled_component == 3)
  {
    for (_i = 0; _i < _test.size(); _i++)
      for (_j = 0; _j < _phi.size(); _j++)
        _local_ke(_i, _j) +=
            (_i == 1 ? 1 : -1) * _cur_ori_ij(_component) * _bond_dfdT_ij[_j] * _bond_status_ij;
  }
  else
  {
    Real val =
        _cur_ori_ij(_component) * _cur_ori_ij(coupled_component) * _bond_dfdU_ij[0] -
        _bond_force_ij[0] * _cur_ori_ij(_component) * _cur_ori_ij(coupled_component) / _cur_len_ij;
    for (_i = 0; _i < _test.size(); _i++)
      for (_j = 0; _j < _phi.size(); _j++)
        _local_ke(_i, _j) += (_i == _j ? 1 : -1) * val * _bond_status_ij;
  }
}
