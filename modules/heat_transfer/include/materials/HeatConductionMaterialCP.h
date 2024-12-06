//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "Material.h"
#include "PropertyReadFile.h"
#include "RankTwoTensor.h"
#include "libmesh/libmesh.h"

/**
 * Calculates thermal conductivity and specific heat of the material
 */

class HeatConductionMaterialCP : public Material
{
public:
  static InputParameters validParams();

  HeatConductionMaterialCP(const InputParameters & parameters);

protected:
  virtual void computeQpProperties() override;

  const RankTwoTensor _kij;

  MaterialProperty<RankTwoTensor> & _thermal_conductivity;

  MaterialProperty<Real> & _thermal_conductivity_dT;

  const PropertyReadFile * const _read_prop_user_object;
};
