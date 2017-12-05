//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NodalRankTwoComponentUserObject.h"
#include "RankTwoTensor.h"

registerMooseObject("PeridynamicsApp", NodalRankTwoComponentUserObject);

template <>
InputParameters
validParams<NodalRankTwoComponentUserObject>()
{
  InputParameters params = validParams<NodalRankTwoTensorUserObject>();
  params.addClassDescription(
      "Class for calculating components of nodal rank-two tensors "
      "(stress/strain) from material properties (stress/strain) for edge elements (i.e., bonds) "
      "connected at that node. NOTE: This UserObject only applies to SNOSPD model.");

  params.addRequiredRangeCheckedParam<unsigned int>(
      "index_i",
      "index_i >= 0 & index_i <= 2",
      "The index i of ij for the tensor to output (0, 1, 2)");
  params.addRequiredRangeCheckedParam<unsigned int>(
      "index_j",
      "index_j >= 0 & index_j <= 2",
      "The index j of ij for the tensor to output (0, 1, 2)");

  return params;
}

NodalRankTwoComponentUserObject::NodalRankTwoComponentUserObject(const InputParameters & parameters)
  : NodalRankTwoTensorUserObject(parameters),
    _i(getParam<unsigned int>("index_i")),
    _j(getParam<unsigned int>("index_j"))
{
}

void
NodalRankTwoComponentUserObject::assembleValue(
    unsigned int id, Real ib_dof, Real vbsum, Real vnsum, Real bond_status)
{
  _aux_sln.add(ib_dof, _tensor[id](_i, _j) * vbsum / vnsum * bond_status);
}
