//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PeridynamicsApp.h"
#include "TensorMechanicsApp.h" // tensor mechanics dependency
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

template <>
InputParameters
validParams<PeridynamicsApp>()
{
  InputParameters params = validParams<MooseApp>();

  return params;
}

registerKnownLabel("PeridynamicsApp");

PeridynamicsApp::PeridynamicsApp(InputParameters parameters) : MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  PeridynamicsApp::registerObjectDepends(_factory);
  PeridynamicsApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  PeridynamicsApp::associateSyntaxDepends(_syntax, _action_factory);
  PeridynamicsApp::associateSyntax(_syntax, _action_factory);

  Moose::registerExecFlags(_factory);
  PeridynamicsApp::registerExecFlags(_factory);
}

PeridynamicsApp::~PeridynamicsApp() {}

// External entry point for dynamic application loading
extern "C" void
PeridynamicsApp__registerApps()
{
  PeridynamicsApp::registerApps();
}
void
PeridynamicsApp::registerApps()
{
  registerApp(PeridynamicsApp);
}

void
PeridynamicsApp::registerObjectDepends(Factory & factory)
{
  TensorMechanicsApp::registerObjects(factory);
}

// External entry point for dynamic object registration
extern "C" void
PeridynamicsApp__registerObjects(Factory & factory)
{
  PeridynamicsApp::registerObjects(factory);
}
void
PeridynamicsApp::registerObjects(Factory & factory)
{
  Registry::registerObjectsTo(factory, {"PeridynamicsApp"});
}

void
PeridynamicsApp::associateSyntaxDepends(Syntax & syntax, ActionFactory & action_factory)
{
  TensorMechanicsApp::associateSyntax(syntax, action_factory);
}

// External entry point for dynamic syntax association
extern "C" void
PeridynamicsApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  PeridynamicsApp::associateSyntax(syntax, action_factory);
}
void
PeridynamicsApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  Registry::registerActionsTo(action_factory, {"PeridynamicsApp"});

  registerTask("add_bond_status_aux_variable", /*is_required=*/true);
  addTaskDependency("add_bond_status_aux_variable", "add_aux_variable");

  registerSyntax("PeridynamicsAction", "Modules/Peridynamics/Master");
  registerSyntax("GeneralizedPlaneStrainPDAction", "Modules/Peridynamics/GeneralizedPlaneStrain/*");
}

// External entry point for dynamic execute flag registration
extern "C" void
PeridynamicsApp__registerExecFlags(Factory & factory)
{
  PeridynamicsApp::registerExecFlags(factory);
}
void
PeridynamicsApp::registerExecFlags(Factory & /*factory*/)
{
}
