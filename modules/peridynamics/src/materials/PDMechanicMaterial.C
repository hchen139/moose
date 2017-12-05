//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PDMechanicMaterial.h"
#include "MooseVariableFEBase.h"

template <>
InputParameters
validParams<PDMechanicMaterial>()
{
  InputParameters params = validParams<PDMaterialBase>();
  params.addClassDescription("Base class for Peridynamic mechanic materials");

  params.addRequiredParam<std::vector<NonlinearVariableName>>(
      "displacements", "Nonlinear variable name for the displacements");
  params.addParam<VariableName>("temperature", "Variable name for the temperature");
  params.addRequiredParam<AuxVariableName>("bond_status",
                                           "Auxiliary variable for bond failure status");

  return params;
}

PDMechanicMaterial::PDMechanicMaterial(const InputParameters & parameters)
  : PDMaterialBase(parameters),
    _has_temp(isParamValid("temperature")),
    _temp_var(_has_temp ? &_subproblem.getVariable(_tid, getParam<VariableName>("temperature"))
                        : NULL),
    _bond_status_var(_subproblem.getVariable(_tid, getParam<AuxVariableName>("bond_status")))
{
  const std::vector<NonlinearVariableName> & nl_vnames(
      getParam<std::vector<NonlinearVariableName>>("displacements"));
  if (_dim != nl_vnames.size())
    mooseError("Size of displacements vector is different from the mesh dimension!");

  for (unsigned int i = 0; i < _dim; ++i)
    _disp_var.push_back(&_subproblem.getVariable(_tid, nl_vnames[i]));
}

void
PDMechanicMaterial::computeProperties()
{
  PDMaterialBase::computeProperties();

  // current length of a EDGE2 element
  computeBondCurrentLength();
}

void
PDMechanicMaterial::computeBondCurrentLength()
{
  RealGradient dxyz;
  dxyz.zero();

  for (unsigned int i = 0; i < _dim; ++i)
  {
    dxyz(i) =
        (*_current_elem->get_node(1))(i) + _disp_var[i]->getNodalValue(*_current_elem->get_node(1));
    dxyz(i) -=
        (*_current_elem->get_node(0))(i) + _disp_var[i]->getNodalValue(*_current_elem->get_node(0));
  }

  _current_length = dxyz.norm();
}
