
[GlobalParams]
  displacements = 'disp_x disp_y'
  bond_status = bond_status
  full_jacobian = true
[]

[Mesh]
  type = GeneratedMeshPD
  dim = 2
  nx = 200
  ymax = 0.4
  horizon_number = 3
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxVariables]
  [./strain_xx]
  [../]
  [./strain_yy]
  [../]
  [./strain_xy]
  [../]

  [./stress_xx]
  [../]
  [./stress_yy]
  [../]
  [./stress_xy]
  [../]
[]

[BCs]
  [./left_dx]
    type = DirichletBC
    variable = disp_x
    boundary = 0
    value = 0.0
  [../]
  [./left_dy]
    type = DirichletBC
    variable = disp_y
    boundary = 0
    value = 0.0
  [../]
  [./right_dx]
    type = FunctionPresetBC
    variable = disp_x
    boundary = 1
    function = '0.05*t'
  [../]
[]

[Modules]
  [./Peridynamics]
    [./Master]
      formulation = NonOrdinaryState
    [../]
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.1e8
    poissons_ratio = 0.3
  [../]
  [./strain]
    type = CorrespondenceSmallStrain
  [../]
  [./stress]
    type = ComputeLinearElasticStress
  [../]
[]

[UserObjects]
  [./strain_xx]
    type = NodalRankTwoTensorUserObject
    rank_two_tensor = total_strain
    component = strain_xx
    index_i = 0
    index_j = 0
  [../]
  [./strain_yy]
    type = NodalRankTwoTensorUserObject
    rank_two_tensor = total_strain
    component = strain_yy
    index_i = 1
    index_j = 1
  [../]
  [./strain_xy]
    type = NodalRankTwoTensorUserObject
    rank_two_tensor = total_strain
    component = strain_xy
    index_i = 0
    index_j = 1
  [../]

  [./stress_xx]
    type = NodalRankTwoTensorUserObject
    rank_two_tensor = stress
    component = stress_xx
    index_i = 0
    index_j = 0
  [../]
  [./stress_yy]
    type = NodalRankTwoTensorUserObject
    rank_two_tensor = stress
    component = stress_yy
    index_i = 1
    index_j = 1
  [../]
  [./stress_xy]
    type = NodalRankTwoTensorUserObject
    rank_two_tensor = stress
    component = stress_xy
    index_i = 0
    index_j = 1
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

  petsc_options = '-snes_ksp_ew'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_lag_jacobian'
  petsc_options_value = 'lu superlu_dist 10'

  start_time = 0
  end_time = 1
[]

[Outputs]
  file_base = 2d_snospd_small_strain_stretch
  exodus = true
[]
