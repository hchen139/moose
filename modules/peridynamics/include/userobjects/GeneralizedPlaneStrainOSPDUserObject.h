//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef GENERALIZEDPLANESTRAINOSPDUSEROBJECT_H
#define GENERALIZEDPLANESTRAINOSPDUSEROBJECT_H

#include "GeneralizedPlaneStrainPDUserObject.h"

class GeneralizedPlaneStrainOSPDUserObject;

template <>
InputParameters validParams<GeneralizedPlaneStrainOSPDUserObject>();

class GeneralizedPlaneStrainOSPDUserObject : public GeneralizedPlaneStrainPDUserObject
{
public:
  GeneralizedPlaneStrainOSPDUserObject(const InputParameters & parameters);

  virtual void execute() override;

protected:
  MooseVariableFE & _out_of_plane_stress_var;
};

#endif // GENERALIZEDPLANESTRAINOSPDUSEROBJECT_H
