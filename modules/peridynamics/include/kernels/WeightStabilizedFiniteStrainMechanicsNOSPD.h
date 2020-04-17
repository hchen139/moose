//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "MechanicsFiniteStrainBaseNOSPD.h"

/**
 * Kernel class for weight-stabilized peridynamic correspondence model for finite strain
 */
class WeightStabilizedFiniteStrainMechanicsNOSPD : public MechanicsFiniteStrainBaseNOSPD
{
public:
  static InputParameters validParams();

  WeightStabilizedFiniteStrainMechanicsNOSPD(const InputParameters & parameters);

protected:
  virtual void computeLocalResidual() override;
  virtual void computeNonlocalResidual() override;

  virtual void computeLocalJacobian() override;
  virtual void computeNonlocalJacobian() override;

  virtual void computeLocalOffDiagJacobian(unsigned int jvar_num,
                                           unsigned int coupled_component) override;
  virtual void computePDNonlocalOffDiagJacobian(unsigned int jvar_num,
                                                unsigned int coupled_component) override;

  /// The power of cosine weight function used to stabilized the correpondence model
  const unsigned int _pow;
  /// The index of displacement component
  const unsigned int _component;
};
