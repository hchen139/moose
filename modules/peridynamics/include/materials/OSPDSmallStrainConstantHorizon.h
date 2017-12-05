//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef OSPDSMALLSTRAINCONSTANTHORIZON_H
#define OSPDSMALLSTRAINCONSTANTHORIZON_H

#include "OSPDSmallStrainMaterial.h"

class OSPDSmallStrainConstantHorizon;

template <>
InputParameters validParams<OSPDSmallStrainConstantHorizon>();

class OSPDSmallStrainConstantHorizon : public OSPDSmallStrainMaterial
{
public:
  OSPDSmallStrainConstantHorizon(const InputParameters & parameters);

protected:
  virtual void computePDMicroModuli() override;
};

#endif // OSPDSMALLSTRAINCONSTANTHORIZON_H
