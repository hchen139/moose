//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "BPDThermalVariableHorizon.h"

registerMooseObject("PeridynamicsApp", BPDThermalVariableHorizon);

template <>
InputParameters
validParams<BPDThermalVariableHorizon>()
{
  InputParameters params = validParams<BPDThermalMaterial>();
  params.addClassDescription(
      "Class to compute peridynamic micro conductivity for bond-based model using irregular mesh");

  return params;
}

BPDThermalVariableHorizon::BPDThermalVariableHorizon(const InputParameters & parameters)
  : BPDThermalMaterial(parameters)
{
}

void
BPDThermalVariableHorizon::computePDMicroConductivity()
{
  _Kij = _dim * _kappa * (1.0 / _nvsum[0] + 1.0 / _nvsum[1]) / _origin_length;
}
