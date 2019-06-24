//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "GhostElemPD.h"
#include "PeridynamicsMesh.h"

registerMooseObject("PeridynamicsApp", GhostElemPD);

template <>
InputParameters
validParams<GhostElemPD>()
{
  InputParameters params = validParams<GeneralUserObjectBasePD>();
  params.addClassDescription("Class for ghosting elements accross processors");

  return params;
}

GhostElemPD::GhostElemPD(const InputParameters & parameters) : GeneralUserObjectBasePD(parameters)
{
  ghostElements();
}

void
GhostElemPD::meshChanged()
{
  ghostElements();
}

void
GhostElemPD::ghostElements()
{
  // Loop through the active local elements and ghost only the nonlocal ones connected to
  // current processor
  const MeshBase::const_element_iterator end_elem = _mesh.getMesh().active_local_elements_end();
  for (MeshBase::const_element_iterator elem = _mesh.getMesh().active_local_elements_begin();
       elem != end_elem;
       ++elem)
    for (unsigned int i = 0; i < 2; ++i)
      for (unsigned int j = 0; j < _pdmesh.getNNeighbors((*elem)->node_id(i)); ++j)
        _subproblem.addGhostedElem(_pdmesh.getAssocBonds((*elem)->node_id(i))[j]);
}
