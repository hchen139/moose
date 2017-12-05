//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef FAILUREINDEXUSEROBJECT_H
#define FAILUREINDEXUSEROBJECT_H

#include "PDElementUserObject.h"

class FailureIndexUserObject;

template <>
InputParameters validParams<FailureIndexUserObject>();

class FailureIndexUserObject : public PDElementUserObject
{
public:
  FailureIndexUserObject(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual void threadJoin(const UserObject & uo) override;
  virtual void finalize() override;

protected:
  MooseVariable * _failure_index_var;
};

#endif // FAILUREINDEXUSEROBJECT_H
