//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef PDKERNELBASE_H
#define PDKERNELBASE_H

#include "Kernel.h"
#include "Assembly.h"
#include "SystemBase.h"

class PeridynamicMesh;
class PDKernelBase;

template <>
InputParameters validParams<PDKernelBase>();

class PDKernelBase : public Kernel
{
public:
  PDKernelBase(const InputParameters & parameters);

  virtual void computeResidual() override;
  virtual void computeJacobian() override;
  virtual Real computeQpResidual() override { return 0.0; }

protected:
  // Compute this Kernel's local contribution to the residual at the current nodes
  virtual void computeLocalResidual() = 0;

  // Compute this Kernel's nonlocal contribution to the residual at the current nodes
  virtual void computeNonlocalResidual(){};

  // Compute this Kernel's local contribution to the diagonal Jacobian at the current nodes
  virtual void computeLocalJacobian(){};

  // This function precalculates data which will be used in the derived classes
  virtual void prepare();

  MooseVariableFE & _bond_status_var;
  const bool _use_full_jacobian;

  PeridynamicMesh & _pdmesh;
  const unsigned int _dim;
  const unsigned int _nnodes;

  std::vector<Node *> _nodes_ij;
  std::vector<Real> _vols_ij;
  std::vector<Real> _dg_bond_vsum_ij;
  std::vector<Real> _dg_node_vsum_ij;
  std::vector<Real> _horizons_ij;
  RealGradient _origin_vec_ij;

  Real _bond_status_ij;
};

#endif // PDKERNELBASE_H
