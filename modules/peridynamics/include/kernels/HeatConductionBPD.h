//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef HEATCONDUCTIONBPD_H
#define HEATCONDUCTIONBPD_H

#include "PDKernelBase.h"

class HeatConductionBPD;

template <>
InputParameters validParams<HeatConductionBPD>();

class HeatConductionBPD : public PDKernelBase
{
public:
  HeatConductionBPD(const InputParameters & parameters);

protected:
  virtual void computeLocalResidual() override;
  virtual void computeLocalJacobian() override;

  const MaterialProperty<Real> & _bond_heat_flow;
  const MaterialProperty<Real> & _bond_dQdT;
};

#endif // HEATCONDUCTIONBPD_H
