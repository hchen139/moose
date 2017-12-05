//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "GeneralizedPlaneStrainPDUserObject.h"

template <>
InputParameters
validParams<GeneralizedPlaneStrainPDUserObject>()
{
  InputParameters params = validParams<PDElementUserObject>();
  params.addClassDescription("Base class for calculating the scalar residual and diagonal Jacobian "
                             "entry for generalized plane strain formulation");
  params.addParam<FunctionName>(
      "out_of_plane_pressure",
      "0",
      "Function used to prescribe pressure in the out-of-plane direction");
  params.addParam<Real>("factor", 1.0, "Scale factor applied to prescribed pressure");
  params.set<ExecFlagEnum>("execute_on") = EXEC_LINEAR;

  return params;
}

GeneralizedPlaneStrainPDUserObject::GeneralizedPlaneStrainPDUserObject(
    const InputParameters & parameters)
  : PDElementUserObject(parameters),
    _Cijkl(getMaterialProperty<RankFourTensor>("elasticity_tensor")),
    _pressure(getFunction("out_of_plane_pressure")),
    _factor(getParam<Real>("factor"))
{
}

void
GeneralizedPlaneStrainPDUserObject::initialize()
{
  _residual = 0;
  _jacobian = 0;
}

void
GeneralizedPlaneStrainPDUserObject::threadJoin(const UserObject & uo)
{
  const GeneralizedPlaneStrainPDUserObject & gpsuo =
      static_cast<const GeneralizedPlaneStrainPDUserObject &>(uo);
  _residual += gpsuo._residual;
  _jacobian += gpsuo._jacobian;
}

void
GeneralizedPlaneStrainPDUserObject::finalize()
{
  gatherSum(_residual);
  gatherSum(_jacobian);
}

Real
GeneralizedPlaneStrainPDUserObject::returnResidual() const
{
  return _residual;
}

Real
GeneralizedPlaneStrainPDUserObject::returnJacobian() const
{
  return _jacobian;
}
