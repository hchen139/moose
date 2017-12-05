//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef SNOSPDKERNELBASE_H
#define SNOSPDKERNELBASE_H

#include "PDMechanicKernel.h"

class SNOSPDKernelBase;

template <>
InputParameters validParams<SNOSPDKernelBase>();

class SNOSPDKernelBase : public PDMechanicKernel
{
public:
  SNOSPDKernelBase(const InputParameters & parameters);

protected:
  virtual RankTwoTensor computeDSDU(unsigned int component, unsigned int nd);

  const MaterialProperty<Real> & _multi;
  const MaterialProperty<RankTwoTensor> & _stress;
  const MaterialProperty<RankTwoTensor> & _shape;
  const MaterialProperty<RankTwoTensor> & _dgrad;
  const MaterialProperty<RankTwoTensor> & _ddgraddu;
  const MaterialProperty<RankTwoTensor> & _ddgraddv;
  const MaterialProperty<RankTwoTensor> & _ddgraddw;
  const MaterialProperty<RankFourTensor> & _Cijkl;
};

#endif // SNOSPDKERNELBASE_H
