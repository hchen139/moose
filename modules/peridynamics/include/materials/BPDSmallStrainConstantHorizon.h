//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef BPDSMALLSTRAINCONSTANTHORIZON_H
#define BPDSMALLSTRAINCONSTANTHORIZON_H

#include "BPDSmallStrainMaterial.h"

class BPDSmallStrainConstantHorizon;

template <>
InputParameters validParams<BPDSmallStrainConstantHorizon>();

class BPDSmallStrainConstantHorizon : public BPDSmallStrainMaterial
{
public:
  BPDSmallStrainConstantHorizon(const InputParameters & parameters);

protected:
  virtual void computePDMicroModuli() override;
};

#endif // BPDSMALLSTRAINCONSTANTHORIZON_H
