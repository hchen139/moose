//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "OSPDSmallStrainVariableHorizon.h"

registerMooseObject("PeridynamicsApp", OSPDSmallStrainVariableHorizon);

template <>
InputParameters
validParams<OSPDSmallStrainVariableHorizon>()
{
  InputParameters params = validParams<OSPDSmallStrainMaterial>();
  params.addClassDescription("Class to compute peridynamic micro elastic moduli for ordinary "
                             "state-based model using irregular mesh");

  return params;
}

OSPDSmallStrainVariableHorizon::OSPDSmallStrainVariableHorizon(const InputParameters & parameters)
  : OSPDSmallStrainMaterial(parameters)
{
}

void
OSPDSmallStrainVariableHorizon::computePDMicroModuli()
{
  _a = 0.5 * (_bulk_modulus - (8.0 - _dim) / 3.0 * _shear_modulus);

  // _b = _bij * _horizon_i + _bji * _horizon_j
  _b = _dim * _dim * (_bulk_modulus / 2.0 - _a) * (1.0 / _nvsum[0] + 1.0 / _nvsum[1]);

  // _d_i = _di * _horizon_i = _dim / _nvsum_i
  _d[0] = _dim / _nvsum[0];

  // _d_j = _dj * _horizon_j = _dim / _nvsum_j
  _d[1] = _dim / _nvsum[1];
}
