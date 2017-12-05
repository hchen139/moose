//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef PERIDYNAMICSACTION_H
#define PERIDYNAMICSACTION_H

#include "Action.h"

class PeridynamicsAction;

template <>
InputParameters validParams<PeridynamicsAction>();

class PeridynamicsAction : public Action
{
public:
  PeridynamicsAction(const InputParameters & params);

  virtual void act() override;

protected:
  virtual std::string getKernelType();
  virtual InputParameters getKernelParameters(std::string type);

  std::vector<NonlinearVariableName> _displacements;
  const unsigned int _ndisp;

  const MooseEnum _formulation;
  const MooseEnum _stabilization;
  const bool _finite_strain_formulation;

  std::vector<AuxVariableName> _save_in;
  std::vector<AuxVariableName> _diag_save_in;
};

#endif // PERIDYNAMICSACTION_H
