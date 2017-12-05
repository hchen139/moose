//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef GENERALIZEDPLANESTRAINPDUSEROBJECT_H
#define GENERALIZEDPLANESTRAINPDUSEROBJECT_H

#include "PDElementUserObject.h"

class GeneralizedPlaneStrainPDUserObject;
class RankFourTensor;

template <>
InputParameters validParams<GeneralizedPlaneStrainPDUserObject>();

class GeneralizedPlaneStrainPDUserObject : public PDElementUserObject
{
public:
  GeneralizedPlaneStrainPDUserObject(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void threadJoin(const UserObject & uo) override;
  virtual void finalize() override;

  Real returnResidual() const;
  Real returnJacobian() const;

protected:
  const MaterialProperty<RankFourTensor> & _Cijkl;

  Function & _pressure;
  const Real _factor;

  Real _residual;
  Real _jacobian;
};

#endif // GENERALIZEDPLANESTRAINPDUSEROBJECT_H
