//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef GENERALIZEDPLANESTRAINSNOSPDUSEROBJECT_H
#define GENERALIZEDPLANESTRAINSNOSPDUSEROBJECT_H

#include "GeneralizedPlaneStrainPDUserObject.h"

class GeneralizedPlaneStrainSNOSPDUserObject;

template <>
InputParameters validParams<GeneralizedPlaneStrainSNOSPDUserObject>();

class GeneralizedPlaneStrainSNOSPDUserObject : public GeneralizedPlaneStrainPDUserObject
{
public:
  GeneralizedPlaneStrainSNOSPDUserObject(const InputParameters & parameters);

  virtual void execute() override;

protected:
  const MaterialProperty<RankTwoTensor> & _stress;
};

#endif // GENERALIZEDPLANESTRAINSNOSPDUSEROBJECT_H
