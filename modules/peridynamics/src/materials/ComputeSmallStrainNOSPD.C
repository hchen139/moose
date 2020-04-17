//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ComputeSmallStrainNOSPD.h"

registerMooseObject("PeridynamicsApp", ComputeSmallStrainNOSPD);

InputParameters
ComputeSmallStrainNOSPD::validParams()
{
  InputParameters params = ComputeStrainBaseNOSPD::validParams();
  params.addClassDescription(
      "Class for computing nodal quantities for the residual and Jacobian calculation "
      "for the peridynamic correspondence models under small strain assumptions");

  return params;
}

ComputeSmallStrainNOSPD::ComputeSmallStrainNOSPD(const InputParameters & parameters)
  : ComputeStrainBaseNOSPD(parameters)
{
}

void
ComputeSmallStrainNOSPD::computeQpStrain()
{
  _shape2[_qp].zero();
  _deformation_gradient[_qp].zero();
  _ddgraddu[_qp].zero();
  _ddgraddv[_qp].zero();
  _ddgraddw[_qp].zero();
  _multi[_qp] = 0.0;

  if (_dim == 2)
    _shape2[_qp](2, 2) = _deformation_gradient[_qp](2, 2) = 1.0;

  computeQpDeformationGradient();

  computeQpTotalStrain();

  _mechanical_strain[_qp] = _total_strain[_qp];

  // Subtract Eigen strains
  for (auto es : _eigenstrains)
    _mechanical_strain[_qp] -= (*es)[_qp];
}

void
ComputeSmallStrainNOSPD::computeQpTotalStrain()
{
  // the small strain tensor
  _total_strain[_qp] = 0.5 * (_deformation_gradient[_qp].transpose() + _deformation_gradient[_qp]) -
                       RankTwoTensor(RankTwoTensor::initIdentity);
}
