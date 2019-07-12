//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "BondStatusBasePD.h"
#include "PeridynamicsMesh.h"

template <>
InputParameters
validParams<BondStatusBasePD>()
{
  InputParameters params = validParams<AuxKernelBasePD>();
  params.addClassDescription("Base class for different failure criteria to update the bond status");

  params.addRequiredCoupledVar("critical_variable", "Name of critical AuxVariable");

  params.set<ExecFlagEnum>("execute_on") = {EXEC_TIMESTEP_BEGIN, EXEC_TIMESTEP_END};

  return params;
}

BondStatusBasePD::BondStatusBasePD(const InputParameters & parameters)
  : AuxKernelBasePD(parameters),
    _bond_status_var(_subproblem.getVariable(_tid, "bond_status")),
    _critical_val(coupledValue("critical_variable"))
{
}

Real
BondStatusBasePD::computeValue()
{
  Real val = 0.0;

  if (_bond_status_var.getElementalValue(_current_elem) > 0.5) // unbroken bond
  {
    Real failure_criterion_val = computeFailureCriterionValue();
    if (failure_criterion_val < 0.0) // unmet failure criterion
      val = 1.0;                     // bond is still unbroken

    // check whether there are sufficient unbroken bonds connected at the two end material points of
    // current bond
    std::vector<unsigned int> intact_bonds(2, 0);
    for (unsigned int i = 0; i < 2; ++i)
    {
      std::vector<dof_id_type> bonds = _pdmesh.getBonds(_current_elem->node_ptr(i)->id());
      for (unsigned int j = 0; j < bonds.size(); ++j)
        if (_bond_status_var.getElementalValue(_pdmesh.elemPtr(bonds[j])) > 0.5)
          intact_bonds[i]++;
    }
    if (intact_bonds[0] < _dim || intact_bonds[1] < _dim)
      val = 0.0;
  }

  return val;
}
