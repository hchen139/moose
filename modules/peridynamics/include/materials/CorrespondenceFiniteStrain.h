//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef CORRESPONDENCEFINITESTRAIN_H
#define CORRESPONDENCEFINITESTRAIN_H

#include "CorrespondenceMaterial.h"
#include "RankTwoTensor.h"

class CorrespondenceFiniteStrain;

template <>
InputParameters validParams<CorrespondenceFiniteStrain>();

class CorrespondenceFiniteStrain : public CorrespondenceMaterial
{
public:
  CorrespondenceFiniteStrain(const InputParameters & parameters);

  static MooseEnum decompositionType();

protected:
  virtual void computeQpStrain() override;

  virtual void computeQpFhat();

  void computeQpStrainRotationIncrements(RankTwoTensor & e, RankTwoTensor & r);
  void subtractEigenstrainIncrementFromStrain(RankTwoTensor & strain);

  MaterialProperty<RankTwoTensor> & _strain_rate;
  MaterialProperty<RankTwoTensor> & _strain_increment;
  MaterialProperty<RankTwoTensor> & _rotation_increment;

  const MaterialProperty<RankTwoTensor> & _deformation_gradient_old;
  const MaterialProperty<RankTwoTensor> & _mechanical_strain_old;
  const MaterialProperty<RankTwoTensor> & _total_strain_old;

  std::vector<const MaterialProperty<RankTwoTensor> *> _eigenstrains_old;

  std::vector<RankTwoTensor> _Fhat;

private:
  enum class DecompMethod
  {
    TaylorExpansion,
    EigenSolution
  };

  const DecompMethod _decomposition_method;
};

#endif // CORRESPONDENCEFINITESTRAIN_H
