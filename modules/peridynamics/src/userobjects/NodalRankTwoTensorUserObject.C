//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NodalRankTwoTensorUserObject.h"

template <>
InputParameters
validParams<NodalRankTwoTensorUserObject>()
{
  InputParameters params = validParams<PDElementUserObject>();
  params.addClassDescription(
      "Base class for calculating components and scalar type quantities of nodal rank-two tensors "
      "(stress/strain) from material properties (stress/strain) for edge elements "
      "(i.e., bonds) connected at that node. NOTE: This UserObject only applies to SNOSPD model.");

  params.addRequiredParam<MaterialPropertyName>(
      "rank_two_tensor", "Name of the nodal rank two tensors (stress/strains)");
  params.addRequiredParam<AuxVariableName>("variable", "Name of the component auxiliary variable");

  params.set<ExecFlagEnum>("execute_on") = EXEC_TIMESTEP_END;

  return params;
}

NodalRankTwoTensorUserObject::NodalRankTwoTensorUserObject(const InputParameters & parameters)
  : PDElementUserObject(parameters),
    _tensor(getMaterialProperty<RankTwoTensor>("rank_two_tensor")),
    _var_name(getParam<AuxVariableName>("variable")),
    _var(_subproblem.getVariable(_tid, _var_name))
{
}

void
NodalRankTwoTensorUserObject::initialize()
{
  std::vector<std::string> zero_vars;
  zero_vars.push_back(Moose::stringify(_var_name));
  _aux.zeroVariables(zero_vars);
}

void
NodalRankTwoTensorUserObject::execute()
{
  dof_id_type ib_dof_i = _current_elem->get_node(0)->dof_number(_aux.number(), _var.number(), 0);
  dof_id_type ib_dof_j = _current_elem->get_node(1)->dof_number(_aux.number(), _var.number(), 0);

  unsigned int id_j_in_i =
      _pdmesh.neighborID(_current_elem->get_node(0)->id(), _current_elem->get_node(1)->id());
  unsigned int id_i_in_j =
      _pdmesh.neighborID(_current_elem->get_node(1)->id(), _current_elem->get_node(0)->id());

  Real vbsum_i = _pdmesh.dgBondVolumeSum(_current_elem->get_node(0)->id(), id_j_in_i);
  Real vbsum_j = _pdmesh.dgBondVolumeSum(_current_elem->get_node(1)->id(), id_i_in_j);

  Real vnsum_i = _pdmesh.dgNodeVolumeSum(_current_elem->get_node(0)->id());
  Real vnsum_j = _pdmesh.dgNodeVolumeSum(_current_elem->get_node(1)->id());

  Real bond_status = _bond_status_var.getElementalValue(_current_elem);

  // assemble volume weighted contribution to node i
  assembleValue(0, ib_dof_i, vbsum_i, vnsum_i, bond_status);
  // assmble volume weighted contribution to node j
  assembleValue(1, ib_dof_j, vbsum_j, vnsum_j, bond_status);
}

void
NodalRankTwoTensorUserObject::threadJoin(const UserObject & uo)
{
  dof_id_type ib_dof_i = _current_elem->get_node(0)->dof_number(_aux.number(), _var.number(), 0);
  dof_id_type ib_dof_j = _current_elem->get_node(1)->dof_number(_aux.number(), _var.number(), 0);

  const NodalRankTwoTensorUserObject & nrttuo =
      static_cast<const NodalRankTwoTensorUserObject &>(uo);
  _aux_sln.add(ib_dof_i, nrttuo._aux_sln(ib_dof_i));
  _aux_sln.add(ib_dof_j, nrttuo._aux_sln(ib_dof_j));
}

void
NodalRankTwoTensorUserObject::finalize()
{
  _aux_sln.close();
}
