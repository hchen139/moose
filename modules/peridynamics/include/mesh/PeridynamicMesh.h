//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef PERIDYNAMICMESH_H
#define PERIDYNAMICMESH_H

#include "MooseMesh.h"

class PeridynamicMesh;

template <>
InputParameters validParams<PeridynamicMesh>();

struct PD_Node
{
  Point coord;
  Real mesh_spacing;
  Real horizon;
  Real volume;
  Real volumesum;
  unsigned int blockID;
};

class PeridynamicMesh : public MooseMesh
{
public:
  PeridynamicMesh(const InputParameters & parameters);

  virtual unsigned int dimension() const override;
  virtual dof_id_type nNodes() const override;
  virtual dof_id_type nElem() const override;

  /*
   * function to compute the  horizon size for each node
   */
  Real computeHorizon(Real spacing);

  /*
   * function for node neighbor search with given horizon
   */
  void findNodeNeighbor();

  /*
   * function to setup node information for bond deformation gradient calculation
   */
  void setupDGNodeInfo();

  /*
   * return neighbor nodes indices for node node_id
   */
  std::vector<dof_id_type> neighbors(dof_id_type node_id);

  /*
   * return the neighbor index of node_j from node_i's neighbor list
   */
  unsigned int neighborID(dof_id_type node_i, dof_id_type node_j);

  /*
   * return neighbor nodes indices for node node_id
   */
  std::vector<dof_id_type> bonds(dof_id_type node_id);

  /*
   * return nodes indices for calculating deformation gradient
   * for bond between node_id and neighbor_id
   */
  std::vector<unsigned int> dgNodeInfo(dof_id_type node_id, unsigned int neighbor_id);

  /*
   * return coordinates for node node_id
   */
  Point coord(dof_id_type node_id);

  /*
   * return nodal volume for node node_id
   */
  Real volume(dof_id_type node_id);

  /*
   * return summation of neighbor nodal volumes for node node_id
   */
  Real volumeSum(dof_id_type node_id);

  /*
   * return summation of volumes of neighbor nodes using in the deformation gradient calculation for
   * bond connecting node node_id and its neighbor neighbor_id
   */
  Real dgBondVolumeSum(dof_id_type node_id, unsigned int neighbor_id);

  /*
   * return summation of volumes of dgBondVolumeSum at node node_id
   */
  Real dgNodeVolumeSum(dof_id_type node_id);

  /*
   * return number of neighbor for node node_id
   */
  unsigned int nneighbors(dof_id_type node_id);

  /*
   * return mesh_spacing
   */
  Real mesh_spacing(dof_id_type node_id);

  /*
   * return horizon size
   */
  Real horizon(dof_id_type node_id);

  /*
   * check whether a material point falls within a given crack gometry
   */
  bool checkInside(Point start, Point end, Point point, Real width, Real tol = 0);

  /*
   * check whether a bond crosses crack surface
   */
  bool checkCrackIntersection(Point A, Point B, Point C, Point D, Real width);

  /*
   * check whether a segment crosses another segment
   */
  bool checkSegmentIntersection(Point A, Point B, Point C, Point D);

protected:
  const Real _horizon_radius;
  const Real _horizon_number;

  const bool _has_cracks;
  std::vector<Point> _cracks_start;
  std::vector<Point> _cracks_end;
  std::vector<Real> _cracks_width;

  unsigned int _dim;

  std::vector<PD_Node> _pdnode;

  unsigned int _total_nodes;
  unsigned int _total_bonds;

  std::vector<std::vector<dof_id_type>> _node_neighbors;
  std::vector<std::vector<dof_id_type>> _node_n_nearest_neighbors;
  std::vector<std::vector<dof_id_type>> _node_bonds;

  std::vector<std::vector<std::vector<unsigned int>>> _dg_nodeinfo;
  std::vector<std::vector<Real>> _dg_bond_volumesum;
  std::vector<Real> _dg_node_volumesum;
};

#endif // PERIDYNAMICMESH_H
