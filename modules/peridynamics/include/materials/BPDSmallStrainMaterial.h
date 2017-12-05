//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef BPDSMALLSTRAINMATERIAL_H
#define BPDSMALLSTRAINMATERIAL_H

#include "PDParametricMaterial.h"

class BPDSmallStrainMaterial;

template <>
InputParameters validParams<BPDSmallStrainMaterial>();

class BPDSmallStrainMaterial : public PDParametricMaterial
{
public:
  BPDSmallStrainMaterial(const InputParameters & parameters);

protected:
  virtual void computePDMicroModuli() = 0;
  virtual void computeBondForce() override;

  Real _Cij;
};

#endif // BPDSMALLSTRAINMATERIAL_H
