//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ComputeThermalExpansionEigenstrainCP.h"
#include "RankTwoTensor.h"

registerMooseObject("SolidMechanicsApp", ComputeThermalExpansionEigenstrainCP);

InputParameters
ComputeThermalExpansionEigenstrainCP::validParams()
{
  InputParameters params = ComputeEigenstrainBase::validParams();
  params.addClassDescription("Computes eigenstrain due to thermal expansion "
                             "with a constant anisotropic coefficient");
  params.addCoupledVar("temperature", "Coupled temperature");
  params.addRequiredCoupledVar("stress_free_temperature",
                               "Reference temperature at which there is no "
                               "thermal expansion for thermal eigenstrain "
                               "calculation");
  params.addRequiredParam<std::vector<Real>>("thermal_expansion_coeff",
                                             "Thermal expansion coefficient");

  return params;
}

ComputeThermalExpansionEigenstrainCP::ComputeThermalExpansionEigenstrainCP(
    const InputParameters & parameters)
  : ComputeEigenstrainBase(parameters),
    _temperature(coupledValue("temperature")),
    _stress_free_temperature(coupledValue("stress_free_temperature")),
    _thermal_expansion_coeff(getParam<std::vector<Real>>("thermal_expansion_coeff")),
    _crysrot(getMaterialProperty<RankTwoTensor>("crysrot"))
{
}

void
ComputeThermalExpansionEigenstrainCP::computeQpEigenstrain()
{
  _eigenstrain[_qp].zero();
  RankTwoTensor thermal_strain =
      _thermal_expansion_coeff * (_temperature[_qp] - _stress_free_temperature[_qp]);

  _eigenstrain[_qp] += thermal_strain;
}
