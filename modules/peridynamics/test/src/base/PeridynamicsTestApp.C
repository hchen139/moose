#include "PeridynamicsTestApp.h"
#include "PeridynamicsApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

template <>
InputParameters
validParams<PeridynamicsTestApp>()
{
  InputParameters params = validParams<PeridynamicsApp>();
  return params;
}

PeridynamicsTestApp::PeridynamicsTestApp(InputParameters parameters) : MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  PeridynamicsApp::registerObjectDepends(_factory);
  PeridynamicsApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  PeridynamicsApp::associateSyntaxDepends(_syntax, _action_factory);
  PeridynamicsApp::associateSyntax(_syntax, _action_factory);

  Moose::registerExecFlags(_factory);
  PeridynamicsApp::registerExecFlags(_factory);

  bool use_test_objs = getParam<bool>("allow_test_objects");
  if (use_test_objs)
  {
    PeridynamicsTestApp::registerObjects(_factory);
    PeridynamicsTestApp::associateSyntax(_syntax, _action_factory);
  }
}

PeridynamicsTestApp::~PeridynamicsTestApp() {}

// External entry point for dynamic application loading
extern "C" void
PeridynamicsTestApp__registerApps()
{
  PeridynamicsTestApp::registerApps();
}
void
PeridynamicsTestApp::registerApps()
{
  registerApp(PeridynamicsApp);
  registerApp(PeridynamicsTestApp);
}

// External entry point for dynamic object registration
extern "C" void
PeridynamicsTestApp__registerObjects(Factory & factory)
{
  PeridynamicsTestApp::registerObjects(factory);
}
void
PeridynamicsTestApp::registerObjects(Factory & /*factory*/)
{
}

// External entry point for dynamic syntax association
extern "C" void
PeridynamicsTestApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  PeridynamicsTestApp::associateSyntax(syntax, action_factory);
}
void
PeridynamicsTestApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}

// External entry point for dynamic execute flag registration
extern "C" void
PeridynamicsTestApp__registerExecFlags(Factory & factory)
{
  PeridynamicsTestApp::registerExecFlags(factory);
}
void
PeridynamicsTestApp::registerExecFlags(Factory & /*factory*/)
{
}
