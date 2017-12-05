//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "Factory.h"
#include "FEProblemBase.h"
#include "Conversion.h"
#include "MooseObjectAction.h"
#include "PeridynamicsAction.h"

registerMooseAction("PeridynamicsApp", PeridynamicsAction, "add_kernel");
registerMooseAction("PeridynamicsApp", PeridynamicsAction, "add_user_object");
registerMooseAction("PeridynamicsApp", PeridynamicsAction, "setup_quadrature");

template <>
InputParameters
validParams<PeridynamicsAction>()
{
  InputParameters params = validParams<Action>();
  params.addClassDescription("Class for setting up peridynamic kernels. Default is BPD. For SPD, "
                             "partial Jacobian is the default");
  params.addRequiredParam<std::vector<NonlinearVariableName>>(
      "displacements", "Nonlinear variable names for the displacements");
  MooseEnum formulation_option("Bond OrdinaryState NonOrdinaryState", "Bond");
  params.addParam<MooseEnum>("formulation",
                             formulation_option,
                             "Available peridynamic formulation options: " +
                                 formulation_option.getRawNames());
  MooseEnum stabilization_option("Force Self", "Self");
  params.addParam<MooseEnum>(
      "stabilization",
      stabilization_option,
      "Available stabilization options for Non-Ordinary State Based Peridynamics: " +
          stabilization_option.getRawNames());
  params.addParam<bool>(
      "full_jacobian", false, "Whether to use full jacobian for state based formulation");
  params.addParam<bool>(
      "finite_strain_formulation", false, "whether the formulation is finite strain or not");
  params.addParam<NonlinearVariableName>("temperature",
                                         "Nonlinear variable name for the temperature");
  params.addParam<NonlinearVariableName>("out_of_plane_strain",
                                         "Nonlinear variable name for the out_of_plane strain for "
                                         "plane stress using SNOSPD formulation");
  params.addRequiredParam<AuxVariableName>("bond_status",
                                           "Auxiliary variable name for the bond failure status");
  params.addParam<bool>(
      "use_displaced_mesh", false, "Whether or not to use the displaced mesh for the kernels");
  params.addParam<std::vector<SubdomainName>>("block",
                                              "The list of ids of the blocks (subdomain) that the "
                                              "stress divergence kernel will be applied to");
  params.addParam<std::vector<AuxVariableName>>("save_in", "The displacement residuals");
  params.addParam<std::vector<AuxVariableName>>("diag_save_in",
                                                "The displacement diagonal preconditioner terms");
  params.addParam<std::vector<MaterialPropertyName>>(
      "eigenstrain_names", "List of eigenstrains to be applied in this strain calculation");

  return params;
}

PeridynamicsAction::PeridynamicsAction(const InputParameters & params)
  : Action(params),
    _displacements(getParam<std::vector<NonlinearVariableName>>("displacements")),
    _ndisp(_displacements.size()),
    _formulation(getParam<MooseEnum>("formulation")),
    _stabilization(getParam<MooseEnum>("stabilization")),
    _finite_strain_formulation(getParam<bool>("finite_strain_formulation")),
    _save_in(getParam<std::vector<AuxVariableName>>("save_in")),
    _diag_save_in(getParam<std::vector<AuxVariableName>>("diag_save_in"))
{
  if (_save_in.size() != 0 && _save_in.size() != _ndisp)
    mooseError("Number of save_in variables should equal to the number of displacement variables ",
               _ndisp);

  if (_diag_save_in.size() != 0 && _diag_save_in.size() != _ndisp)
    mooseError(
        "Number of diag_save_in variables should equal to the number of displacement variables ",
        _ndisp);
}

void
PeridynamicsAction::act()
{
  if (_current_task == "add_user_object")
  {
    // add ghosting UO
    std::string uo_type = "GhostElemPDUserObject";
    std::string uo_name = "GhostElemPDUserObject";

    InputParameters params = _factory.getValidParams(uo_type);
    params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

    _problem->addUserObject(uo_type, uo_name, params);
  }
  else if (_current_task == "add_kernel")
  {
    std::string kernel_type = getKernelType();
    InputParameters params = getKernelParameters(kernel_type);

    for (unsigned int i = 0; i < _ndisp; ++i)
    {
      std::string kernel_name = "Peridynamics_" + Moose::stringify(i);

      params.set<unsigned int>("component") = i;
      params.set<NonlinearVariableName>("variable") = _displacements[i];

      if (_save_in.size() != 0)
        params.set<std::vector<AuxVariableName>>("save_in") = {_save_in[i]};
      if (_diag_save_in.size() != 0)
        params.set<std::vector<AuxVariableName>>("diag_save_in") = {_diag_save_in[i]};

      _problem->addKernel(kernel_type, kernel_name, params);
    }
  }
  else if (_current_task == "setup_quadrature") // set the quadrature type to GAUSS_LOBATTO and
                                                // order to FIRST, such that the two quadrature
                                                // points are the two end nodes of a Edge2 element
  {
    QuadratureType type = Moose::stringToEnum<QuadratureType>("GAUSS_LOBATTO");
    Order order = Moose::stringToEnum<Order>("FIRST");

    _problem->createQRules(type, order, order, order);
  }
  else
    mooseError("Task error in PeridynamicsAction");
}

std::string
PeridynamicsAction::getKernelType()
{
  std::string type;

  switch (_formulation)
  {
    case 0: // bond based PD formulation
      type = "BPDKernel";
      break;

    case 1: // ordinary state based PD formulation
      type = "OSPDKernel";
      break;

    case 2: // non-ordinary state based PD formulation
      switch (_stabilization)
      {
        case 0: // fictitious force-stabilized
          type = "FNOSPDKernel";
          break;

        case 1: // self-stabilized
          if (_finite_strain_formulation)
            type = "FiniteSNOSPDKernel";
          else
            type = "SmallSNOSPDKernel";
          break;

        default:
          mooseError("Unknown PD stabilization scheme. Choose from: Force Self");
      }
      break;

    default:
      mooseError("Unsupported PD formulation. Choose from: Bond OrdinaryState NonOrdinaryState");
  }

  return type;
}

InputParameters
PeridynamicsAction::getKernelParameters(std::string type)
{
  InputParameters params = _factory.getValidParams(type);
  params.set<std::vector<NonlinearVariableName>>("displacements") = _displacements;

  if (isParamValid("temperature"))
    params.set<NonlinearVariableName>("temperature") =
        getParam<NonlinearVariableName>("temperature");

  if (isParamValid("out_of_plane_strain"))
    params.set<NonlinearVariableName>("out_of_plane_strain") =
        getParam<NonlinearVariableName>("out_of_plane_strain");

  params.set<AuxVariableName>("bond_status") = getParam<AuxVariableName>("bond_status");

  params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

  params.set<bool>("full_jacobian") = getParam<bool>("full_jacobian");

  if (_formulation == "NonOrdinaryState")
  {
    params.set<std::vector<MaterialPropertyName>>("eigenstrain_names") =
        getParam<std::vector<MaterialPropertyName>>("eigenstrain_names");
  }

  // check whether this kernel is restricted to certain block?
  if (isParamValid("block"))
    params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");

  return params;
}
