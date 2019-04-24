//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef FILEMESHPD_H
#define FILEMESHPD_H

#include "MooseMeshPD.h"

class FileMeshPD;

template <>
InputParameters validParams<FileMeshPD>();

/**
 * Mesh class for creating peridynamic mesh from EXODUS finite element mesh
 */
class FileMeshPD : public MooseMeshPD
{
public:
  FileMeshPD(const InputParameters & parameters);
  FileMeshPD(const FileMeshPD & other_mesh);

  virtual std::unique_ptr<MooseMesh> safeClone() const override;
  virtual void init() override;
  virtual void buildMesh() override;

  // Get/Set Filename (for meshes read from a file)
  void setFileName(const std::string & file_name) { _file_name = file_name; }
  virtual std::string getFileName() const override { return _file_name; }

protected:
  /// the file_name from whence this mesh came
  std::string _file_name;

  /// EXODUS finite element mesh
  MeshBase * _fe_mesh;

  /// minimum spacing between nodes for the whole mesh
  Real _min_spacing;
};

#endif /* FILEMESHPD_H */
