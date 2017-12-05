//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef OSPDSMALLSTRAINVARIABLEHORIZON_H
#define OSPDSMALLSTRAINVARIABLEHORIZON_H

#include "OSPDSmallStrainMaterial.h"

class OSPDSmallStrainVariableHorizon;

template <>
InputParameters validParams<OSPDSmallStrainVariableHorizon>();

class OSPDSmallStrainVariableHorizon : public OSPDSmallStrainMaterial
{
public:
  OSPDSmallStrainVariableHorizon(const InputParameters & parameters);

protected:
  virtual void computePDMicroModuli() override;
};

#endif // OSPDSMALLSTRAINVARIABLEHORIZON_H
