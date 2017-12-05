[GlobalParams]
  displacements = 'disp_x disp_y'
  scalar_out_of_plane_strain = scalar_strain_zz
  bond_status = bond_status
  full_jacobian = true
[]

[Mesh]
  type = GeneratedMeshPD
  dim = 2
  nx = 4
  horizon_number = 3
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]

  [./scalar_strain_zz]
    order = FIRST
    family = SCALAR
  [../]
[]

[Modules]
  [./Peridynamics]
    [./Master]
      formulation = NonOrdinaryState
    [../]
    [./GeneralizedPlaneStrain]
      [./gps]
        formulation = NonOrdinaryState
      [../]
    [../]
  [../]
[]

[Materials]
  [./elastic_tensor]
    type = ComputeIsotropicElasticityTensor
    poissons_ratio = 0.3
    youngs_modulus = 1e6
  [../]
  [./strain]
    type = CorrespondencePlaneSmallStrain
  [../]
  [./stress]
    type = ComputeLinearElasticStress
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
    petsc_options_iname = '-ksp_type -pc_type -snes_type'
    petsc_options_value = 'bcgs bjacobi test'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
[]
