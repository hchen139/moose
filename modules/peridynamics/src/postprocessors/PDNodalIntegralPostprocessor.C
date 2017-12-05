//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PDNodalIntegralPostprocessor.h"

template <>
InputParameters
validParams<PDNodalIntegralPostprocessor>()
{
  InputParameters params = validParams<PDNodalPostprocessor>();
  params.addClassDescription("Base class for peridynamic nodal integral Postprocessors");

  return params;
}

PDNodalIntegralPostprocessor::PDNodalIntegralPostprocessor(const InputParameters & parameters)
  : PDNodalPostprocessor(parameters), _integral_value(0)
{
}

void
PDNodalIntegralPostprocessor::initialize()
{
  _integral_value = 0;
}

void
PDNodalIntegralPostprocessor::execute()
{
  _integral_value += computeNodalValue() * _pdmesh.volume(_current_node->id());
}

Real
PDNodalIntegralPostprocessor::getValue()
{
  gatherSum(_integral_value);

  return _integral_value;
}

void
PDNodalIntegralPostprocessor::threadJoin(const UserObject & uo)
{
  const PDNodalIntegralPostprocessor & pps = static_cast<const PDNodalIntegralPostprocessor &>(uo);
  _integral_value += pps._integral_value;
}
