#ifndef PERIDYNAMICSTESTAPP_H
#define PERIDYNAMICSTESTAPP_H

#include "MooseApp.h"

class PeridynamicsTestApp;

template <>
InputParameters validParams<PeridynamicsTestApp>();

class PeridynamicsTestApp : public MooseApp
{
public:
  PeridynamicsTestApp(InputParameters parameters);
  virtual ~PeridynamicsTestApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
  static void registerExecFlags(Factory & factory);
};

#endif /* PERIDYNAMICSTESTAPP_H */
