//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef NODALRANKTWOSCALARUSEROBJECT_H
#define NODALRANKTWOSCALARUSEROBJECT_H

#include "NodalRankTwoTensorUserObject.h"

class NodalRankTwoScalarUserObject;

template <>
InputParameters validParams<NodalRankTwoScalarUserObject>();

class NodalRankTwoScalarUserObject : public NodalRankTwoTensorUserObject
{
public:
  NodalRankTwoScalarUserObject(const InputParameters & parameters);

  virtual void
  assembleValue(unsigned int id, Real ib_dof, Real vbsum, Real vnsum, Real bond_status) override;

protected:
  /**
   * Determines the information to be extracted from the tensor by using the
   * RankTwoScalarTools namespace, e.g., vonMisesStress, EquivalentPlasticStrain,
   * L2norm, MaxPrincipal eigenvalue, etc.
   */
  MooseEnum _scalar_type;

  const Point _point1;
  const Point _point2;
  Point _input_direction;
};

#endif // NODALRANKTWOSCALARUSEROBJECT_H
