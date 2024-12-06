//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "HeatConductionMaterialCP.h"
#include "RankTwoTensorImplementation.h"

registerMooseObject("HeatTransferApp", HeatConductionMaterialCP);

InputParameters
HeatConductionMaterialCP::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Material model for heat conduction of polycrystal");
  params.addRequiredParam<std::vector<Real>>("thermal_conductivity",
                                             "The thermal conductivity tensor values");

  params.addRequiredParam<UserObjectName>("read_prop_user_object",
                                          "The ElementReadPropertyFile "
                                          "GeneralUserObject to read element "
                                          "specific property values from file");
  return params;
}

HeatConductionMaterialCP::HeatConductionMaterialCP(const InputParameters & parameters)
  : Material(parameters),
    _kij(getParam<std::vector<Real>>("thermal_conductivity")),
    _thermal_conductivity(declareProperty<RankTwoTensor>("thermal_conductivity")),
    _thermal_conductivity_dT(declareProperty<Real>("thermal_conductivity_dT")),
    _read_prop_user_object(&getUserObject<PropertyReadFile>("read_prop_user_object"))
{
}

void
HeatConductionMaterialCP::computeQpProperties()
{
  _thermal_conductivity[_qp] = _kij;
  _thermal_conductivity_dT[_qp] = 0;

  // the rotation tensor for a Z1, X2, Z3 rotation
  RankTwoTensor R;

  Real euler_angle_0 = _read_prop_user_object->getData(_current_elem, 0);
  Real euler_angle_1 = _read_prop_user_object->getData(_current_elem, 1);
  Real euler_angle_2 = _read_prop_user_object->getData(_current_elem, 2);

  Real c1 = std::cos(euler_angle_0 * (libMesh::pi / 180.0));
  Real c2 = std::cos(euler_angle_1 * (libMesh::pi / 180.0));
  Real c3 = std::cos(euler_angle_2 * (libMesh::pi / 180.0));

  Real s1 = std::sin(euler_angle_0 * (libMesh::pi / 180.0));
  Real s2 = std::sin(euler_angle_1 * (libMesh::pi / 180.0));
  Real s3 = std::sin(euler_angle_2 * (libMesh::pi / 180.0));

  R(0, 0) = c1 * c3 - c2 * s1 * s3;  // R11
  R(1, 0) = -c1 * s3 - c2 * c3 * s1; // R21
  R(2, 0) = s1 * s2;                 // R31

  R(0, 1) = c3 * s1 + c1 * c2 * s3; // R12
  R(1, 1) = c1 * c2 * c3 - s1 * s3; // R22
  R(2, 1) = -c1 * s2;               // R32

  R(0, 2) = s2 * s3; // R13
  R(1, 2) = c3 * s2; // R23
  R(2, 2) = c2;      // R33

  // conduct the rotation
  _thermal_conductivity[_qp].rotate(R.transpose());
}
