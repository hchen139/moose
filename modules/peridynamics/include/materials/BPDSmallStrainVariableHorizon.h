//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef BPDSMALLSTRAINVARIABLEHORIZON_H
#define BPDSMALLSTRAINVARIABLEHORIZON_H

#include "BPDSmallStrainMaterial.h"

class BPDSmallStrainVariableHorizon;

template <>
InputParameters validParams<BPDSmallStrainVariableHorizon>();

class BPDSmallStrainVariableHorizon : public BPDSmallStrainMaterial
{
public:
  BPDSmallStrainVariableHorizon(const InputParameters & parameters);

protected:
  virtual void computePDMicroModuli() override;
};

#endif // BPDSMALLSTRAINVARIABLEHORIZON_H
