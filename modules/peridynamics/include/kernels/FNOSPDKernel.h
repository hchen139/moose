//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef FNOSPDKERNEL_H
#define FNOSPDKERNEL_H

#include "PDMechanicKernel.h"
#include "RankTwoTensor.h"

class FNOSPDKernel;

template <>
InputParameters validParams<FNOSPDKernel>();

class FNOSPDKernel : public PDMechanicKernel
{
public:
  FNOSPDKernel(const InputParameters & parameters);

protected:
  virtual void computeLocalResidual() override;
  virtual void computeLocalJacobian() override;
  virtual void computeNonlocalJacobian() override;

  void computeLocalOffDiagJacobian(unsigned int coupled_component) override;
  void computePDNonlocalOffDiagJacobian(unsigned int jvar_num,
                                        unsigned int coupled_component) override;

  RankTwoTensor computeDSDU(unsigned int component, unsigned int nd);

  const MaterialProperty<Real> & _multi;

  const MaterialProperty<RankTwoTensor> & _stress;
  const MaterialProperty<RankTwoTensor> & _shape;
  const MaterialProperty<RankTwoTensor> & _dgrad;
  const MaterialProperty<RankTwoTensor> & _ddgraddu;
  const MaterialProperty<RankTwoTensor> & _ddgraddv;
  const MaterialProperty<RankTwoTensor> & _ddgraddw;
  const MaterialProperty<RankFourTensor> & _Cijkl;

  const MaterialProperty<Real> & _sf_coeff;

  const unsigned int _component;
};

#endif // FNOSPDKERNEL_H
