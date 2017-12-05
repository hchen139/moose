//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef CORRESPONDENCEMATERIAL_H
#define CORRESPONDENCEMATERIAL_H

#include "DerivativeMaterialInterface.h"
#include "PDMechanicMaterial.h"
#include "RankTwoTensor.h"

class CorrespondenceMaterial;

template <>
InputParameters validParams<CorrespondenceMaterial>();

class CorrespondenceMaterial : public DerivativeMaterialInterface<PDMechanicMaterial>
{
public:
  CorrespondenceMaterial(const InputParameters & parameters);

  virtual void initQpStatefulProperties() override;

protected:
  virtual void computeProperties() override;

  virtual void computeQpDeformationGradient();
  virtual void computeQpStrain() = 0;

  std::vector<MaterialPropertyName> _eigenstrain_names;
  std::vector<const MaterialProperty<RankTwoTensor> *> _eigenstrains;

  MaterialProperty<RankTwoTensor> & _shape_tensor;
  MaterialProperty<RankTwoTensor> & _deformation_gradient;

  MaterialProperty<RankTwoTensor> & _ddgraddu;
  MaterialProperty<RankTwoTensor> & _ddgraddv;
  MaterialProperty<RankTwoTensor> & _ddgraddw;

  MaterialProperty<RankTwoTensor> & _total_strain;
  MaterialProperty<RankTwoTensor> & _mechanical_strain;

  MaterialProperty<Real> & _multi;
};

#endif // CORRESPONDENCEMATERIAL_H
