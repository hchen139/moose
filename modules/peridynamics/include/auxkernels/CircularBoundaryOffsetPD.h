//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "AuxKernelBasePD.h"

/**
 * Aux Kernel class to compute the offset of PD nodes on circular boundary
 */
class CircularBoundaryOffsetPD : public AuxKernelBasePD
{
public:
  static InputParameters validParams();

  CircularBoundaryOffsetPD(const InputParameters & parameters);

protected:
  Real computeValue() override;

  const Real _radius;
  const Point _center;
};
