//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef BPDKERNEL_H
#define BPDKERNEL_H

#include "PDMechanicKernel.h"

class BPDKernel;

template <>
InputParameters validParams<BPDKernel>();

class BPDKernel : public PDMechanicKernel
{
public:
  BPDKernel(const InputParameters & parameters);

protected:
  virtual void computeLocalResidual() override;
  virtual void computeLocalJacobian() override;
  virtual void computeLocalOffDiagJacobian(unsigned int jvar) override;

  const MaterialProperty<Real> & _bond_force_ij;
  const MaterialProperty<Real> & _bond_dfdU_ij;
  const MaterialProperty<Real> & _bond_dfdT_ij;

  const unsigned int _component;
};

#endif // BPDKERNEL_H
