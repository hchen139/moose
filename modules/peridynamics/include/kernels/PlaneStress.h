//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef PLANESTRESS_H
#define PLANESTRESS_H

#include "SNOSPDKernelBase.h"

class PlaneStress;

template <>
InputParameters validParams<PlaneStress>();

class PlaneStress : public SNOSPDKernelBase
{
public:
  PlaneStress(const InputParameters & parameters);

  virtual void computeLocalResidual() override;
  virtual void computeLocalJacobian() override;
  virtual void computeLocalOffDiagJacobian(unsigned int coupled_component) override;
  virtual void computePDNonlocalOffDiagJacobian(unsigned int jvar_num,
                                                unsigned int coupled_component) override;
};
#endif // PLANESTRESS_H
