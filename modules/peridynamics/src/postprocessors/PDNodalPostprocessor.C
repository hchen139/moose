//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PDNodalPostprocessor.h"

template <>
InputParameters
validParams<PDNodalPostprocessor>()
{
  InputParameters params = validParams<NodalPostprocessor>();
  params.addClassDescription("Base class for peridynamic nodal Postprocessors");

  return params;
}

PDNodalPostprocessor::PDNodalPostprocessor(const InputParameters & parameters)
  : NodalPostprocessor(parameters),
    _pdmesh(dynamic_cast<PeridynamicMesh &>(_mesh)),
    _dim(_pdmesh.dimension())
{
}
