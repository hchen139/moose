//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "FailureIndexUserObject.h"
#include "AuxiliarySystem.h"
#include "PeridynamicMesh.h"
#include "MooseVariable.h"

registerMooseObject("PeridynamicsApp", FailureIndexUserObject);

template <>
InputParameters
validParams<FailureIndexUserObject>()
{
  InputParameters params = validParams<PDElementUserObject>();
  params.addClassDescription("Class for updating the nodal failure index");

  params.addCoupledVar("failure_index", "Auxiliary variable for nodal failure index");

  params.set<ExecFlagEnum>("execute_on") = {EXEC_INITIAL, EXEC_TIMESTEP_END};

  return params;
}

FailureIndexUserObject::FailureIndexUserObject(const InputParameters & parameters)
  : PDElementUserObject(parameters), _failure_index_var(getVar("failure_index", 0))
{
}

void
FailureIndexUserObject::initialize()
{
  std::vector<std::string> zero_vars;
  zero_vars.push_back("failure_index");
  _aux.zeroVariables(zero_vars);
}

void
FailureIndexUserObject::execute()
{
  Real bond_status = _bond_status_var.getElementalValue(_current_elem);

  dof_id_type ib_dof_i =
      _current_elem->get_node(0)->dof_number(_aux.number(), _failure_index_var->number(), 0);
  dof_id_type ib_dof_j =
      _current_elem->get_node(1)->dof_number(_aux.number(), _failure_index_var->number(), 0);

  unsigned int tb_i = _pdmesh.nneighbors(_current_elem->get_node(0)->id());
  unsigned int tb_j = _pdmesh.nneighbors(_current_elem->get_node(1)->id());

  if (std::abs(bond_status - 0.0) < 0.01)
  {
    _aux_sln.add(ib_dof_i, 1.0 / tb_i);
    _aux_sln.add(ib_dof_j, 1.0 / tb_j);
  }
}

void
FailureIndexUserObject::threadJoin(const UserObject & uo)
{
  dof_id_type ib_dof_i =
      _current_elem->get_node(0)->dof_number(_aux.number(), _failure_index_var->number(), 0);
  dof_id_type ib_dof_j =
      _current_elem->get_node(1)->dof_number(_aux.number(), _failure_index_var->number(), 0);

  const FailureIndexUserObject & fi = static_cast<const FailureIndexUserObject &>(uo);
  _aux_sln.add(ib_dof_i, fi._aux_sln(ib_dof_i));
  _aux_sln.add(ib_dof_j, fi._aux_sln(ib_dof_j));
}

void
FailureIndexUserObject::finalize()
{
  _aux_sln.close();
}
