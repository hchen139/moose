//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NodalRankTwoScalarUserObject.h"
#include "RankTwoScalarTools.h"

registerMooseObject("PeridynamicsApp", NodalRankTwoScalarUserObject);

template <>
InputParameters
validParams<NodalRankTwoScalarUserObject>()
{
  InputParameters params = validParams<NodalRankTwoTensorUserObject>();
  params.addClassDescription(
      "Class for calculating scalar quantities of nodal rank-two tensors "
      "(stress/strain) from material properties (stress/strain) for edge elements (i.e., bonds) "
      "connected at that node. NOTE: This UserObject only applies to SNOSPD model.");

  params.addRequiredParam<MooseEnum>(
      "scalar_type", RankTwoScalarTools::scalarOptions(), "Type of scalar output");
  params.addParam<Point>(
      "point1",
      Point(0, 0, 0),
      "Start point for axis used to calculate some cylinderical material tensor quantities");
  params.addParam<Point>("point2",
                         Point(0, 1, 0),
                         "End point for axis used to calculate some material tensor quantities");
  params.addParam<Point>("direction", Point(0, 0, 1), "Direction vector");

  return params;
}

NodalRankTwoScalarUserObject::NodalRankTwoScalarUserObject(const InputParameters & parameters)
  : NodalRankTwoTensorUserObject(parameters),
    _scalar_type(getParam<MooseEnum>("scalar_type")),
    _point1(parameters.get<Point>("point1")),
    _point2(parameters.get<Point>("point2")),
    _input_direction(parameters.get<Point>("direction") / parameters.get<Point>("direction").norm())
{
}

void
NodalRankTwoScalarUserObject::assembleValue(
    unsigned int id, Real ib_dof, Real vbsum, Real vnsum, Real bond_status)
{
  Point curr_point = *_current_elem->get_node(id);
  Real scalar_value = RankTwoScalarTools::getQuantity(
      _tensor[id], _scalar_type, _point1, _point2, curr_point, _input_direction);

  _aux_sln.add(ib_dof, scalar_value * vbsum / vnsum * bond_status);
}
