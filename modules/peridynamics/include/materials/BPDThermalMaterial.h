//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef BPDTHERMALMATERIAL_H
#define BPDTHERMALMATERIAL_H

#include "PDMaterialBase.h"

class BPDThermalMaterial;
class Function;

template <>
InputParameters validParams<BPDThermalMaterial>();

class BPDThermalMaterial : public PDMaterialBase
{
public:
  BPDThermalMaterial(const InputParameters & parameters);

protected:
  virtual void computeProperties() override;

  void computeBondHeatFlow();
  void computeNodalTemperature();

  virtual void computePDMicroConductivity() = 0;

  MooseVariableFE & _temp_var;

  std::vector<Real> _temp;

  const Real _thermal_conductivity;
  Function * _thermal_conductivity_function;

  MaterialProperty<Real> & _bond_heat_flow;
  MaterialProperty<Real> & _bond_dQdT;

  Real _kappa;
  Real _Kij;
};

#endif // BPDTHERMALMATERIAL_H
