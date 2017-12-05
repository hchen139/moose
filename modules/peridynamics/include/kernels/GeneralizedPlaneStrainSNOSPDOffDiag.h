//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef GENERALIZEDPLANESTRAINSNOSPDOFFDIAG_H
#define GENERALIZEDPLANESTRAINSNOSPDOFFDIAG_H

#include "SNOSPDKernelBase.h"

class GeneralizedPlaneStrainSNOSPDOffDiag;
class RankFourTensor;

template <>
InputParameters validParams<GeneralizedPlaneStrainSNOSPDOffDiag>();

class GeneralizedPlaneStrainSNOSPDOffDiag : public SNOSPDKernelBase
{
public:
  GeneralizedPlaneStrainSNOSPDOffDiag(const InputParameters & parameters);

protected:
  virtual void computeLocalResidual() override{};
  virtual void computeOffDiagJacobianScalar(unsigned int jvar_num) override;

  void computeDispFullOffDiagJacobianScalar(unsigned int component, unsigned int jvar_num);
  void computeDispPartialOffDiagJacobianScalar(unsigned int component, unsigned int jvar_num);
  void computeTempOffDiagJacobianScalar(unsigned int jvar_num);

  const unsigned int _scalar_out_of_plane_strain_var_num;
};

#endif // GENERALIZEDPLANESTRAINSNOSPDOFFDIAG_H
