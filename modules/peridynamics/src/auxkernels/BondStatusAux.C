//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "BondStatusAux.h"

registerMooseObject("PeridynamicsApp", BondStatusAux);

template <>
InputParameters
validParams<BondStatusAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addClassDescription("Class for updating the bond status");
  params.addCoupledVar("bond_status", "Auxiliary variable for bond status");
  params.addCoupledVar("critical_stretch", "Auxiliary variable for bond critical stretch");
  params.set<ExecFlagEnum>("execute_on") = EXEC_TIMESTEP_END;

  return params;
}

BondStatusAux::BondStatusAux(const InputParameters & parameters)
  : AuxKernel(parameters),
    _elastic_stretch(getMaterialProperty<Real>("elastic_stretch")),
    _critical_stretch(coupledValue("critical_stretch")),
    _bond_status(coupledValue("bond_status"))
{
}

Real
BondStatusAux::computeValue()
{
  if (std::abs(_bond_status[0] - 1.0) < 0.01 && _elastic_stretch[0] < _critical_stretch[0])
    return 1.0;
  else
    return 0.0;
}
