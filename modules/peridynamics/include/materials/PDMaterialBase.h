//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef PDMATERIALBASE_H
#define PDMATERIALBASE_H

#include "Material.h"
#include "PeridynamicMesh.h"

class PDMaterialBase;

template <>
InputParameters validParams<PDMaterialBase>();

class PDMaterialBase : public Material
{
public:
  PDMaterialBase(const InputParameters & parameters);

protected:
  virtual void computeProperties() override;

  PeridynamicMesh & _pdmesh;

  const unsigned int _dim;
  const unsigned int _nnodes;
  std::vector<Real> _horizon;
  std::vector<Real> _nv;
  std::vector<Real> _nvsum;

  RealGradient _origin_vec;
  Real _origin_length;
};

#endif // PDMATERIALBASE_H
