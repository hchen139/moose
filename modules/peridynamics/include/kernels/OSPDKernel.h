//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef OSPDKERNEL_H
#define OSPDKERNEL_H

#include "PDMechanicKernel.h"

class OSPDKernel;

template <>
InputParameters validParams<OSPDKernel>();

class OSPDKernel : public PDMechanicKernel
{
public:
  OSPDKernel(const InputParameters & parameters);

protected:
  virtual void computeLocalResidual() override;
  virtual void computeNonlocalResidual() override;

  virtual void computeLocalJacobian() override;
  virtual void computeNonlocalJacobian() override;

  void computeLocalOffDiagJacobian(unsigned int coupled_component) override;
  void computePDNonlocalOffDiagJacobian(unsigned int jvar_num,
                                        unsigned int coupled_component) override;

  const MaterialProperty<Real> & _bond_force_ij;
  const MaterialProperty<Real> & _bond_force_i_j;
  const MaterialProperty<Real> & _bond_dfdU_ij;
  const MaterialProperty<Real> & _bond_dfdU_i_j;
  const MaterialProperty<Real> & _bond_dfdT_ij;
  const MaterialProperty<Real> & _bond_dfdT_i_j;

  const unsigned int _component;
};

#endif // OSPDKERNEL_H
