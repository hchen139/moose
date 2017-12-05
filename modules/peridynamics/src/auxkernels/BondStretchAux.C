//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "BondStretchAux.h"

registerMooseObject("PeridynamicsApp", BondStretchAux);

template <>
InputParameters
validParams<BondStretchAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addClassDescription("Class for outputing bond stretch");
  params.set<ExecFlagEnum>("execute_on") = EXEC_TIMESTEP_END;

  return params;
}

BondStretchAux::BondStretchAux(const InputParameters & parameters)
  : AuxKernel(parameters), _elastic_stretch(getMaterialProperty<Real>("elastic_stretch"))
{
}

Real
BondStretchAux::computeValue()
{
  return _elastic_stretch[0];
}
