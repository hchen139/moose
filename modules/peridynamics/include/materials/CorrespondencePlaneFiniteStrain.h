//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef CORRESPONDENCEPLANEFINITESTRAIN_H
#define CORRESPONDENCEPLANEFINITESTRAIN_H

#include "CorrespondenceFiniteStrain.h"

class CorrespondencePlaneFiniteStrain;

template <>
InputParameters validParams<CorrespondencePlaneFiniteStrain>();

class CorrespondencePlaneFiniteStrain : public CorrespondenceFiniteStrain
{
public:
  CorrespondencePlaneFiniteStrain(const InputParameters & parameters);

protected:
  virtual void computeQpFhat() override;

  virtual Real computeQpOutOfPlaneDeformationGradient();
  virtual Real computeQpOutOfPlaneDeformationGradientOld();

private:
  const bool _scalar_out_of_plane_strain_coupled;
  const VariableValue & _scalar_out_of_plane_strain;
  const VariableValue & _scalar_out_of_plane_strain_old;

  const bool _out_of_plane_strain_coupled;
  const VariableValue & _out_of_plane_strain;
  const VariableValue & _out_of_plane_strain_old;
};

#endif // CORRESPONDENCEPLANEFINITESTRAIN_H
