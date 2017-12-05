//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef NODALSTRESSSTRAINAUX_H
#define NODALSTRESSSTRAINAUX_H

#include "AuxKernel.h"
#include "RankTwoTensor.h"
#include "RankFourTensor.h"

class NodalStressStrainAux;
class PeridynamicMesh;

template <>
InputParameters validParams<NodalStressStrainAux>();

class NodalStressStrainAux : public AuxKernel
{
public:
  NodalStressStrainAux(const InputParameters & parameters);

protected:
  Real computeValue() override;

  virtual RankTwoTensor computeNodalTotalStrain();
  virtual RankTwoTensor computeNodalElasticStrain();
  virtual RankTwoTensor computeNodalStress();

  PeridynamicMesh & _pdmesh;
  const unsigned int _dim;

  MooseVariableFE * _temp_var;
  MooseVariableFE & _bond_status_var;

  bool _scalar_out_of_plane_strain_coupled;
  const VariableValue & _scalar_out_of_plane_strain;

  const bool _plane_stress;

  const Real _youngs_modulus;
  const Real _poissons_ratio;
  Real _alpha;

  const Real _temp_ref;

  std::string _rank_two_tensor;

  MooseEnum _quantity_type;

  unsigned int _i;
  unsigned int _j;

  std::vector<MooseVariableFE *> _disp_var;

  RankFourTensor _Cijkl;
};

#endif // NODALSTRESSSTRAINAUX_H
