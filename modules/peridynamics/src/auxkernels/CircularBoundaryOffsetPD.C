//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "CircularBoundaryOffsetPD.h"

registerMooseObject("PeridynamicsApp", CircularBoundaryOffsetPD);

InputParameters
CircularBoundaryOffsetPD::validParams()
{
  InputParameters params = AuxKernelBasePD::validParams();
  params.addClassDescription("Class for computing the offset of PD nodes on circular boundary");

  params.addParam<Real>("radius", 0.0041, "Radius of the circular boundary");
  params.addParam<Point>("reference_point", Point(0, 0, 0), "Reference point for the radius");

  params.set<ExecFlagEnum>("execute_on") = EXEC_INITIAL;

  return params;
}

CircularBoundaryOffsetPD::CircularBoundaryOffsetPD(const InputParameters & parameters)
  : AuxKernelBasePD(parameters),
    _radius(getParam<Real>("radius")),
    _center(parameters.get<Point>("reference_point"))

{
  if (!_var.isNodal())
    mooseError("CircularBoundaryOffsetPD operates on nodal variable!");
}

Real
CircularBoundaryOffsetPD::computeValue()
{
  // the offset value should be negative
  Real dist = (*_current_node - _center).norm() - _radius;

  return dist;
}
