//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NodalVariableIntegral.h"
#include "MooseVariableFEBase.h"

registerMooseObject("PeridynamicsApp", NodalVariableIntegral);

template <>
InputParameters
validParams<NodalVariableIntegral>()
{
  InputParameters params = validParams<PDNodalIntegralPostprocessor>();
  params.addClassDescription("Class for calculating the domain integral of stress_zz componnent");
  params.addRequiredParam<VariableName>(
      "variable", "The name of the variable that this postprocessor operates on");

  return params;
}

NodalVariableIntegral::NodalVariableIntegral(const InputParameters & parameters)
  : PDNodalIntegralPostprocessor(parameters),
    _var(_subproblem.getVariable(_tid, getParam<VariableName>("variable")))
{
}

Real
NodalVariableIntegral::computeNodalValue()
{
  return _var.getNodalValue(*_current_node);
}
