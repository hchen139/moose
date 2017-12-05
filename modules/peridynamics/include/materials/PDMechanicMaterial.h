//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef PDMECHANICMATERIAL_H
#define PDMECHANICMATERIAL_H

#include "PDMaterialBase.h"

class PDMechanicMaterial;

template <>
InputParameters validParams<PDMechanicMaterial>();

class PDMechanicMaterial : public PDMaterialBase
{
public:
  PDMechanicMaterial(const InputParameters & parameters);

protected:
  virtual void computeProperties() override;

  void computeBondCurrentLength();

  const bool _has_temp;

  MooseVariableFE * _temp_var;
  MooseVariableFE & _bond_status_var;
  std::vector<MooseVariableFE *> _disp_var;

  Real _current_length;
};

#endif // PDMECHANICMATERIAL_H
