//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef BPDTHERMALVARIABLEHORIZON_H
#define BPDTHERMALVARIABLEHORIZON_H

#include "BPDThermalMaterial.h"

class BPDThermalVariableHorizon;

template <>
InputParameters validParams<BPDThermalVariableHorizon>();

class BPDThermalVariableHorizon : public BPDThermalMaterial
{
public:
  BPDThermalVariableHorizon(const InputParameters & parameters);

protected:
  virtual void computePDMicroConductivity() override;
};

#endif // BPDTHERMALVARIABLEHORIZON_H
