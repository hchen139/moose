//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef PDMECHANICKERNEL_H
#define PDMECHANICKERNEL_H

#include "PDKernelBase.h"
#include "DerivativeMaterialInterface.h"

class PDMechanicKernel;
class RankTwoTensor;

template <>
InputParameters validParams<PDMechanicKernel>();

class PDMechanicKernel : public DerivativeMaterialInterface<PDKernelBase>
{
public:
  PDMechanicKernel(const InputParameters & parameters);
  virtual void computeOffDiagJacobian(MooseVariableFE & jvar) override;

  // Compute this Kernel's local contribution to the off-diagonal Jacobian at the current nodes
  virtual void computeLocalOffDiagJacobian(unsigned int /* coupled_component */){};

  // Compute this Kernel's nonlocal contribution to the off-diagonal Jacobian at the current nodes
  virtual void computePDNonlocalOffDiagJacobian(unsigned int /* jvar_num */,
                                                unsigned int /* coupled_component */){};

  virtual void initialSetup() override;
  virtual void prepare() override;

protected:
  std::vector<MooseVariableFE *> _disp_var;
  const bool _temp_coupled;
  MooseVariableFE * _temp_var;

  const std::vector<MaterialPropertyName> _eigenstrain_names;
  std::vector<const MaterialProperty<RankTwoTensor> *> _deigenstrain_dT;

  const bool _out_of_plane_strain_coupled;
  MooseVariableFE * _out_of_plane_strain_var;

  const std::vector<RealGradient> * _orientation;
  std::vector<dof_id_type> _ivardofs_ij;
  RealGradient _cur_ori_ij;
  Real _cur_len_ij;
};

#endif // PDMECHANICKERNEL_H
