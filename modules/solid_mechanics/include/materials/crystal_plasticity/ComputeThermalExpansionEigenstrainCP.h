//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ComputeEigenstrainBase.h"

/**
 * ComputeThermalExpansionEigenstrainCP computes an eigenstrain for thermal expansion
 * with a constant expansion coefficient.
 */

class ComputeThermalExpansionEigenstrainCP : public ComputeEigenstrainBase
{
public:
  static InputParameters validParams();

  ComputeThermalExpansionEigenstrainCP(const InputParameters & parameters);

  virtual void computeQpEigenstrain() override;

protected:
  const VariableValue & _temperature;
  const VariableValue & _stress_free_temperature;
  const RankTwoTensor _thermal_expansion_coeff;

  const MaterialProperty<RankTwoTensor> & _crysrot;
};
