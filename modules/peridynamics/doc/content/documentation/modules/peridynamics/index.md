# Peridynamics Module

!media peridynamics/pd_fuel_fracture_examples.png style=width:300px;padding-left:20px;float:right;
       caption=Peridynamics results of 3D fuel fragmentation

- [Complete System List](peridynamics/systems.md)

The MOOSE peridynamic module is a library for solving classical solid mechanics and heat transfer problems using the peridynamics theory. For advanced deformation modeling, such as plasticity and creep, the peridynamics module should be used in conjunction with material classes from tensor mechanics module. More information about this module is found below:

## Basic Concepts in Peridynamic Theory

- [Horizon and States](peridynamics/HorizonStates.md): Basic information about the formulation of peridynamic theory
- [Deformation Gradient](peridynamics/DeformationGradients.md): Using automatic differentiation of free energy material objects

## Peridynamic Models

- [Peridynamic Mechanics and Heat Conduction Models](peridynamics/PeridynamicModels.md): Basic information about the equations underlying different peridynamic models

## Using Materials From Tensor Mechanics in Correspondence Material Model

In peridynamic correspondence material model, concepts such as strain and stress tensors from classical Continuum Mechanics still apply, i.e., given the nonlocal deformation gradient calculated in peridynamics, calculations based on nonlocal deformation gradient to establish the constitutive relationship between stress and strain follows the same methodology as in local continuum theory. Strain and stress tensors from Continuum Mechanics reside at each discrete material point rather than quadrature point in peridynamic correspondence material model. Plasticity and creep material models from tensor mechanics can be directly used in peridynamics for nonlinear deformation modeling.

- [Strains](tensor_mechanics/Strains.md)
- [Stresses](tensor_mechanics/Stresses.md)

## Spatial Discretization

Similar to mesh generation in finite element methods, a spatial discretization is required to discretize the domain of interest into discrete material points. Family, or connectivity, information needs to be built for each material points. Current peridynamic module supports two type of discretization schemes: build-in regular (i.e., 2D & 3D rectangular) domain discretization and external ExodusII FEM elements based domain discretization.
