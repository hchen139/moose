//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef FINITESNOSPDKERNEL_H
#define FINITESNOSPDKERNEL_H

#include "SNOSPDKernelBase.h"

class FiniteSNOSPDKernel;

template <>
InputParameters validParams<FiniteSNOSPDKernel>();

class FiniteSNOSPDKernel : public SNOSPDKernelBase
{
public:
  FiniteSNOSPDKernel(const InputParameters & parameters);

protected:
  virtual void computeLocalResidual() override;

  virtual void computeLocalJacobian() override;
  virtual void computeNonlocalJacobian() override;

  virtual void computeLocalOffDiagJacobian(unsigned int coupled_component) override;
  virtual void computePDNonlocalOffDiagJacobian(unsigned int jvar_num,
                                                unsigned int coupled_component) override;
  virtual RankTwoTensor computeDSDU(unsigned int component, unsigned int nd) override;

  RankFourTensor computeDSDFhat(unsigned int nd);
  Real computeDJDU(unsigned int component, unsigned int nd);
  RankTwoTensor computeDinvFTDU(unsigned int component, unsigned int nd);

  const MaterialProperty<RankTwoTensor> & _dgrad_old;
  const MaterialProperty<RankTwoTensor> & _E_inc;
  const MaterialProperty<RankTwoTensor> & _R_inc;

  const unsigned int _component;
};

#endif // FINITESNOSPDKERNEL_H
