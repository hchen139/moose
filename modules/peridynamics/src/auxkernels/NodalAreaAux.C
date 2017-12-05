//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NodalAreaAux.h"
#include "PeridynamicMesh.h"

registerMooseObject("PeridynamicsApp", NodalAreaAux);

template <>
InputParameters
validParams<NodalAreaAux>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addClassDescription("Class for output nodal area(2D) / volume(3D)");
  params.set<ExecFlagEnum>("execute_on") = EXEC_INITIAL;

  return params;
}

NodalAreaAux::NodalAreaAux(const InputParameters & parameters)
  : AuxKernel(parameters), _pdmesh(dynamic_cast<PeridynamicMesh &>(_mesh))
{
}

Real
NodalAreaAux::computeValue()
{
  return _pdmesh.volume(_current_node->id());
}
