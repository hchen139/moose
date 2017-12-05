//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef NODALRANKTWOCOMPONENTUSEROBJECT_H
#define NODALRANKTWOCOMPONENTUSEROBJECT_H

#include "NodalRankTwoTensorUserObject.h"

class NodalRankTwoComponentUserObject;

template <>
InputParameters validParams<NodalRankTwoComponentUserObject>();

class NodalRankTwoComponentUserObject : public NodalRankTwoTensorUserObject
{
public:
  NodalRankTwoComponentUserObject(const InputParameters & parameters);

  virtual void
  assembleValue(unsigned int id, Real ib_dof, Real vbsum, Real vnsum, Real bond_status) override;

protected:
  const unsigned int _i;
  const unsigned int _j;
};

#endif // NODALRANKTWOCOMPONENTUSEROBJECT_H
