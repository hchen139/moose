//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef INERTIALFORCEPD_H
#define INERTIALFORCEPD_H

#include "NodalKernel.h"

class InertialForcePD;
class PeridynamicMesh;

template <>
InputParameters validParams<InertialForcePD>();

class InertialForcePD : public NodalKernel
{
public:
  InertialForcePD(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

private:
  PeridynamicMesh & _pdmesh;
  const Real _density;
  const VariableValue & _u_old;
  const VariableValue & _vel_old;
  const VariableValue & _accel_old;
  const Real _beta;
};

#endif // INERTIALFORCEPD_H
