//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "SNOSPDKernelBase.h"

template <>
InputParameters
validParams<SNOSPDKernelBase>()
{
  InputParameters params = validParams<PDMechanicKernel>();
  params.addClassDescription("Base class for kernels using Self-stabilized Non-Ordinary "
                             "State-based PeriDynamic (SNOSPD) formulation");

  return params;
}

SNOSPDKernelBase::SNOSPDKernelBase(const InputParameters & parameters)
  : PDMechanicKernel(parameters),
    _multi(getMaterialProperty<Real>("multi")),
    _stress(getMaterialProperty<RankTwoTensor>("stress")),
    _shape(getMaterialProperty<RankTwoTensor>("shape_tensor")),
    _dgrad(getMaterialProperty<RankTwoTensor>("deformation_gradient")),
    _ddgraddu(getMaterialProperty<RankTwoTensor>("ddeformation_gradient_du")),
    _ddgraddv(getMaterialProperty<RankTwoTensor>("ddeformation_gradient_dv")),
    _ddgraddw(getMaterialProperty<RankTwoTensor>("ddeformation_gradient_dw")),
    _Cijkl(getMaterialProperty<RankFourTensor>("elasticity_tensor"))
{
}

RankTwoTensor
SNOSPDKernelBase::computeDSDU(unsigned int component, unsigned int nd)
{
  // compute the derivative of stress w.r.t the solution components for small strain
  RankTwoTensor dSdU;
  if (component == 0)
    dSdU = _Cijkl[nd] * 0.5 *
           (_ddgraddu[nd].transpose() * _dgrad[nd] + _dgrad[nd].transpose() * _ddgraddu[nd]);
  else if (component == 1)
    dSdU = _Cijkl[nd] * 0.5 *
           (_ddgraddv[nd].transpose() * _dgrad[nd] + _dgrad[nd].transpose() * _ddgraddv[nd]);
  else if (component == 2)
    dSdU = _Cijkl[nd] * 0.5 *
           (_ddgraddw[nd].transpose() * _dgrad[nd] + _dgrad[nd].transpose() * _ddgraddw[nd]);

  return dSdU;
}
