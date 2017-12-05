//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef NODALRANKTWOTENSORUSEROBJECT_H
#define NODALRANKTWOTENSORUSEROBJECT_H

#include "PDElementUserObject.h"

class NodalRankTwoTensorUserObject;
class RankTwoTensor;

template <>
InputParameters validParams<NodalRankTwoTensorUserObject>();

class NodalRankTwoTensorUserObject : public PDElementUserObject
{
public:
  NodalRankTwoTensorUserObject(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual void threadJoin(const UserObject & uo) override;
  virtual void finalize() override;

protected:
  virtual void
  assembleValue(unsigned int id, Real ib_dof, Real vbsum, Real vnsum, Real bond_status) = 0;

  const MaterialProperty<RankTwoTensor> & _tensor;

  AuxVariableName _var_name;
  MooseVariableFE & _var;
};

#endif // NODALRANKTWOTENSORUSEROBJECT_H
