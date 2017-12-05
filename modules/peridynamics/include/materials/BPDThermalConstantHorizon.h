//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef BPDTHERMALCONSTANTHORIZON_H
#define BPDTHERMALCONSTANTHORIZON_H

#include "BPDThermalMaterial.h"

class BPDThermalConstantHorizon;

template <>
InputParameters validParams<BPDThermalConstantHorizon>();

class BPDThermalConstantHorizon : public BPDThermalMaterial
{
public:
  BPDThermalConstantHorizon(const InputParameters & parameters);

protected:
  virtual void computePDMicroConductivity() override;
};

#endif // BPDTHERMALCONSTANTHORIZON_H
