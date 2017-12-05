//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef CORRESPONDENCEPLANESMALLSTRAIN_H
#define CORRESPONDENCEPLANESMALLSTRAIN_H

#include "CorrespondenceSmallStrain.h"
#include "RankTwoTensor.h"

class CorrespondencePlaneSmallStrain;

template <>
InputParameters validParams<CorrespondencePlaneSmallStrain>();

class CorrespondencePlaneSmallStrain : public CorrespondenceSmallStrain
{
public:
  CorrespondencePlaneSmallStrain(const InputParameters & parameters);

protected:
  virtual void computeQpTotalStrain() override;

  Real computeOutOfPlaneStrain();

private:
  const bool _scalar_out_of_plane_strain_coupled;
  const VariableValue & _scalar_out_of_plane_strain;

  const bool _out_of_plane_strain_coupled;
  const VariableValue & _out_of_plane_strain;
};

#endif // CORRESPONDENCEPLANESMALLSTRAIN_H
