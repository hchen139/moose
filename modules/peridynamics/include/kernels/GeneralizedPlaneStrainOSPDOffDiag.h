//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef GENERALIZEDPLANESTRAINOSPDOFFDIAG_H
#define GENERALIZEDPLANESTRAINOSPDOFFDIAG_H

#include "PDMechanicKernel.h"

class GeneralizedPlaneStrainOSPDOffDiag;
class RankFourTensor;

template <>
InputParameters validParams<GeneralizedPlaneStrainOSPDOffDiag>();

class GeneralizedPlaneStrainOSPDOffDiag : public PDMechanicKernel
{
public:
  GeneralizedPlaneStrainOSPDOffDiag(const InputParameters & parameters);

protected:
  virtual void computeLocalResidual() override{};
  virtual void computeOffDiagJacobianScalar(unsigned int jvar_num) override;

  void computeDispFullOffDiagJacobianScalar(unsigned int component, unsigned int jvar_num);
  void computeDispPartialOffDiagJacobianScalar(unsigned int component, unsigned int jvar_num);
  void computeTempOffDiagJacobianScalar(unsigned int jvar_num);

  const MaterialProperty<Real> & _bond_dfdE_ij;
  const MaterialProperty<Real> & _bond_dfdE_i_j;
  const MaterialProperty<Real> & _alpha;
  const MaterialProperty<RankFourTensor> & _Cijkl;

  const unsigned int _scalar_out_of_plane_strain_var_num;
};

#endif // GENERALIZEDPLANESTRAINOSPDOFFDIAG_H
