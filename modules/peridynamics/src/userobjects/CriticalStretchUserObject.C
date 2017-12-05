//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "AuxiliarySystem.h"
#include "PeridynamicMesh.h"
#include "MooseRandom.h"
#include "MooseVariableFEBase.h"
#include "CriticalStretchUserObject.h"

registerMooseObject("PeridynamicsApp", CriticalStretchUserObject);

template <>
InputParameters
validParams<CriticalStretchUserObject>()
{
  InputParameters params = validParams<GeneralUserObject>();
  params.addClassDescription("Class for assigning value for AuxVariable critical_stretch. "
                             "Normal distribution is assumed");
  params.addRequiredParam<std::string>("critical_stretch",
                                       "Auxiliary variable for critical stretch");
  params.addRequiredParam<Real>("mean",
                                "Reference value for bond critical stretch value to be randomized");
  params.addParam<Real>(
      "standard_deviation", 0.0, "Standard deviation for the normal distribution");
  params.addParam<unsigned int>("seed", 0, "Seed value for the random number generator");
  params.set<ExecFlagEnum>("execute_on") = EXEC_INITIAL;

  return params;
}

CriticalStretchUserObject::CriticalStretchUserObject(const InputParameters & parameters)
  : GeneralUserObject(parameters),
    _mesh(_subproblem.mesh()),
    _pdmesh(dynamic_cast<PeridynamicMesh &>(_mesh)),
    _aux(_fe_problem.getAuxiliarySystem()),
    _aux_sln(_aux.solution()),
    _critical_stretch_var(&_subproblem.getVariable(_tid, "critical_stretch")),
    _mean(getParam<Real>("mean")),
    _standard_deviation(getParam<Real>("standard_deviation"))
{
  MooseRandom::seed(getParam<unsigned int>("seed"));
}

void
CriticalStretchUserObject::execute()
{
  for (unsigned int i = 0; i < _pdmesh.nElem(); ++i)
  {
    Elem * elem_i = _mesh.elemPtr(i);
    dof_id_type elem_i_dof = elem_i->dof_number(_aux.number(), _critical_stretch_var->number(), 0);
    Real rand_num = MooseRandom::randNormal(_mean, _standard_deviation);
    if (elem_i->processor_id() == processor_id())
      _aux_sln.set(elem_i_dof, rand_num);
  }
}

void
CriticalStretchUserObject::finalize()
{
  _aux_sln.close();
}
