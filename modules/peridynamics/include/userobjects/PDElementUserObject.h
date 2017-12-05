//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef PDELEMENTUSEROBJECT_H
#define PDELEMENTUSEROBJECT_H

#include "ElementUserObject.h"
#include "PeridynamicMesh.h"

class PDElementUserObject;

template <>
InputParameters validParams<PDElementUserObject>();

class PDElementUserObject : public ElementUserObject
{
public:
  PDElementUserObject(const InputParameters & parameters);

protected:
  MooseVariableFE & _bond_status_var;

  AuxiliarySystem & _aux;
  NumericVector<Number> & _aux_sln;

  PeridynamicMesh & _pdmesh;
};

#endif // PDELEMENTUSEROBJECT_H
