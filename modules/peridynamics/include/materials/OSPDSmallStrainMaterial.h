//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef OSPDSMALLSTRAINMATERIAL_H
#define OSPDSMALLSTRAINMATERIAL_H

#include "PDParametricMaterial.h"

class OSPDSmallStrainMaterial;

template <>
InputParameters validParams<OSPDSmallStrainMaterial>();

class OSPDSmallStrainMaterial : public PDParametricMaterial
{
public:
  OSPDSmallStrainMaterial(const InputParameters & parameters);

protected:
  virtual void computeBondForce() override;
  virtual void computePDMicroModuli() = 0;

  MaterialProperty<Real> & _bond_force_i_j;
  MaterialProperty<Real> & _bond_dfdU_i_j;
  MaterialProperty<Real> & _bond_dfdT_i_j;
  MaterialProperty<Real> & _bond_dfdE_i_j;

  Real _a;
  Real _b;
  std::vector<Real> _d;
};

#endif // OSPDSMALLSTRAINMATERIAL_H
