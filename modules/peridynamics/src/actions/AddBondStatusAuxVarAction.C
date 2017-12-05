//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "AddBondStatusAuxVarAction.h"
#include "FEProblemBase.h"
#include "MooseObjectAction.h"

#include "libmesh/string_to_enum.h"

registerMooseAction("PeridynamicsApp", AddBondStatusAuxVarAction, "add_bond_status_aux_variable");

template <>
InputParameters
validParams<AddBondStatusAuxVarAction>()
{
  InputParameters params = validParams<Action>();
  params.addClassDescription(
      "Class for Action to set up the bond status aux variable automatically");

  return params;
}

AddBondStatusAuxVarAction::AddBondStatusAuxVarAction(const InputParameters & params)
  : Action(params)
{
}

void
AddBondStatusAuxVarAction::act()
{
  if (_current_task == "add_bond_status_aux_variable")
  {
    // add the bond_status aux variable which is not restricted to any specific block but for
    // all peridynamic domains
    _problem->addAuxVariable("bond_status",
                             FEType(Utility::string_to_enum<Order>("CONSTANT"),
                                    Utility::string_to_enum<FEFamily>("MONOMIAL")));

    // Set the initial value to unit using InitialConditionAction, following the coding in
    // createInitialConditionAction() in AddVariableAction.C

    // Set the parameters for the action
    InputParameters action_params = _action_factory.getValidParams("AddOutputAction");
    action_params.set<ActionWarehouse *>("awh") = &_awh;
    action_params.set<std::string>("type") = "ConstantIC";
    // Create the action
    std::shared_ptr<MooseObjectAction> action = std::static_pointer_cast<MooseObjectAction>(
        _action_factory.create("AddInitialConditionAction", "bond_status_moose", action_params));
    // Set the required parameters for the object to be created
    action->getObjectParams().set<VariableName>("variable") = "bond_status";
    action->getObjectParams().set<Real>("value") = 1.0;
    // Store the action in the ActionWarehouse
    _awh.addActionBlock(action);
  }
}
