//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ComputePlaneStressIsotropicElasticityTensor.h"
#include "ElasticityTensorTools.h"

registerMooseObject("PeridynamicsApp", ComputePlaneStressIsotropicElasticityTensor);

template <>
InputParameters
validParams<ComputePlaneStressIsotropicElasticityTensor>()
{
  InputParameters params = validParams<ComputeIsotropicElasticityTensor>();
  params.addClassDescription("Compute a constant isotropic elasticity tensor for plane stress.");

  return params;
}

ComputePlaneStressIsotropicElasticityTensor::ComputePlaneStressIsotropicElasticityTensor(
    const InputParameters & parameters)
  : ComputeIsotropicElasticityTensor(parameters)
{
  // modify the elasticity tensor for plane stress case
  Real youngs_modulus = ElasticityTensorTools::getIsotropicYoungsModulus(_Cijkl);
  Real poissons_ratio = ElasticityTensorTools::getIsotropicPoissonsRatio(_Cijkl);
  _Cijkl.zero();
  _Cijkl(0, 0, 0, 0) = _Cijkl(1, 1, 1, 1) =
      youngs_modulus / (1 + poissons_ratio) / (1 - poissons_ratio);
  _Cijkl(0, 0, 1, 1) = _Cijkl(1, 1, 0, 0) =
      youngs_modulus * poissons_ratio / (1 + poissons_ratio) / (1 - poissons_ratio);
  _Cijkl(0, 1, 0, 1) = _Cijkl(1, 0, 1, 0) = 0.5 * youngs_modulus / (1 + poissons_ratio);
}
