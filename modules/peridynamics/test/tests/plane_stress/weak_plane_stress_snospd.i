
[GlobalParams]
  displacements = 'disp_x disp_y'
  bond_status = bond_status
  out_of_plane_strain = strain_zz
[]

[Mesh]
  type = GeneratedMeshPD
  dim = 2
  nx = 10
  horizon_number = 3
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]

  [./strain_zz]
  [../]
[]

[BCs]
  [./left_dx]
    type = PresetBC
    variable = disp_x
    boundary = 0
    value = 0.0
  [../]
  [./left_dy]
    type = PresetBC
    variable = disp_y
    boundary = 0
    value = 0.0
  [../]
  [./right_dx]
    type = PresetBC
    variable = disp_x
    boundary = 1
    value = 0.001
  [../]
[]

[Modules]
  [./Peridynamics]
    [./Master]
      formulation = NonOrdinaryState
    [../]
  [../]
[]

[Kernels]
  [./strain_zz]
    type = PlaneStress
    variable = strain_zz
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.1e8
    poissons_ratio = 0.3
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
  [../]
[]

[Executioner]
  type = Transient
  solve_type = PJFNK
  line_search = none
  start_time = 0
  end_time = 1
[]

[Outputs]
  file_base = weak_plane_stress_snospd
  exodus = true
[]
