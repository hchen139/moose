//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef GENERATEDMESHPD_H
#define GENERATEDMESHPD_H

#include "MooseMeshPD.h"

class GeneratedMeshPD;

template <>
InputParameters validParams<GeneratedMeshPD>();

/**
 * Mesh class for creating regular peridynamic mesh of rectangular domains
 */
class GeneratedMeshPD : public MooseMeshPD
{
public:
  GeneratedMeshPD(const InputParameters & parameters);
  GeneratedMeshPD(const GeneratedMeshPD & /* other_mesh */) = default;

  // No copy
  GeneratedMeshPD & operator=(const GeneratedMeshPD & other_mesh) = delete;
  virtual std::unique_ptr<MooseMesh> safeClone() const override;
  virtual void init() override;
  virtual void buildMesh() override;
  virtual Real getMinInDimension(unsigned int component) const override;
  virtual Real getMaxInDimension(unsigned int component) const override;
  virtual void prepared(bool state) override;

  void init2DRectangular();
  void init3DRectangular();
  void build2DRectangular(UnstructuredMesh & mesh, BoundaryInfo & boundary_info);
  void build3DRectangular(UnstructuredMesh & mesh, BoundaryInfo & boundary_info);

protected:
  /// Number of elements in x direction
  unsigned int _nx;

  /// Domain size in x, y, z direction
  Real _xmin, _ymin, _zmin, _xmax, _ymax, _zmax;

  /// Nearest neighbor distance
  Real _spacing;

  /// Boolean to indicate that dimensions may have changed
  bool _dims_may_have_changed;
};

#endif /* GENERATEDMESHPD_H */
