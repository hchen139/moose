//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef CORRESPONDENCEFNOSPD_H
#define CORRESPONDENCEFNOSPD_H

#include "CorrespondenceSmallStrain.h"

class CorrespondenceFNOSPD;

template <>
InputParameters validParams<CorrespondenceFNOSPD>();

class CorrespondenceFNOSPD : public CorrespondenceSmallStrain
{
public:
  CorrespondenceFNOSPD(const InputParameters & parameters);

protected:
  virtual void computeQpDeformationGradient() override;

  const MaterialProperty<RankFourTensor> & _Cijkl;

  MaterialProperty<Real> & _sf_coeff;
};

#endif // CORRESPONDENCEFNOSPD_H
