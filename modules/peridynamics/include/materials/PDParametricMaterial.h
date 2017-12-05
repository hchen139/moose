//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef PDPARAMETRICMATERIALS_H
#define PDPARAMETRICMATERIALS_H

#include "PDMechanicMaterial.h"
#include "RankFourTensor.h"

class PDParametricMaterial;

template <>
InputParameters validParams<PDParametricMaterial>();

class PDParametricMaterial : public PDMechanicMaterial
{
public:
  PDParametricMaterial(const InputParameters & parameters);

protected:
  virtual void computeProperties() override;
  virtual void computeQpProperties() override;
  void computeNodalTemperature();
  void computeBondStretch();
  virtual void computeBondForce() = 0;

  const bool _plane_stress;

  const Real _youngs_modulus;
  const Real _poissons_ratio;

  const bool _scalar_out_of_plane_strain_coupled;
  const VariableValue & _scalar_out_of_plane_strain;

  std::vector<Real> _temp;
  const Real _temp_ref;
  Real _alpha;

  MaterialProperty<Real> & _total_stretch;
  MaterialProperty<Real> & _mechanical_stretch;
  MaterialProperty<Real> & _elastic_stretch;

  MaterialProperty<Real> & _bond_force_ij;
  MaterialProperty<Real> & _bond_dfdU_ij;
  MaterialProperty<Real> & _bond_dfdT_ij;
  MaterialProperty<Real> & _bond_dfdE_ij;

  MaterialProperty<RankFourTensor> & _elasticity_tensor;
  MaterialProperty<Real> & _thermal_expansion_coeff;

  Real _shear_modulus;
  Real _bulk_modulus;

  RankFourTensor _Cijkl;
};

#endif // PDPARAMETRICMATERIAL_H
