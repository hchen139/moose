//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef BONDSTATUSAUX_H
#define BONDSTATUSAUX_H

#include "AuxKernel.h"

class BondStatusAux;

template <>
InputParameters validParams<BondStatusAux>();

class BondStatusAux : public AuxKernel
{
public:
  BondStatusAux(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  const MaterialProperty<Real> & _elastic_stretch;
  const VariableValue & _critical_stretch;
  const VariableValue & _bond_status;
};

#endif // BONDSTATUSAUX_H
