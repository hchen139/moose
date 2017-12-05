//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef CRITICALSTRETCHUSEROBJECT_H
#define CRITICALSTRETCHUSEROBJECT_H

#include "GeneralUserObject.h"
#include "PeridynamicMesh.h"

class CriticalStretchUserObject;

template <>
InputParameters validParams<CriticalStretchUserObject>();

class CriticalStretchUserObject : public GeneralUserObject
{
public:
  CriticalStretchUserObject(const InputParameters & parameters);

  virtual void initialize() override{};
  virtual void execute() override;
  virtual void finalize() override;

protected:
  MooseMesh & _mesh;
  PeridynamicMesh & _pdmesh;

  AuxiliarySystem & _aux;
  NumericVector<Number> & _aux_sln;
  MooseVariableFE * _critical_stretch_var;

  const Real _mean;
  const Real _standard_deviation;
};

#endif // CRITICALSTRETCHUSEROBJECT_H
