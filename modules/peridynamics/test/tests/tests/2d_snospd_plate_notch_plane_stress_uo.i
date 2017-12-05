
[GlobalParams]
  displacements = 'disp_x disp_y'
  bond_status = bond_status
  full_jacobian = true
  youngs_modulus = 2.1e8
  poissons_ratio = 0.3
[]

[Mesh]
  type = GeneratedMeshPD
  dim = 2
  nx = 100
  horizon_number = 3
  xmax = 0.1
  ymax = 0.04
  partitioner = centroid
  centroid_partitioner_direction = radial
  cracks_start = '0.05 0.015 0'
  cracks_end = '0.05 0.025 0'
  cracks_width = 0.00399
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
    function = '0.0004*t'
  [../]
[]

[Modules]
  [./Peridynamics]
    [./Master]
      formulation = NonOrdinaryState
    [../]
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

[Materials]
  [./elasticty_tensor]
    type = ComputePlaneStressIsotropicElasticityTensor
  [../]
  [./strain]
    type = CorrespondenceSmallStrain
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

  petsc_options = '-snes_ksp_ew'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_lag_jacobian'
  petsc_options_value = 'lu superlu_dist 10'

  end_time = 1
[]

[VectorPostprocessors]
  [./axis1]
    type = NodalValueSampler
    variable = 'disp_x disp_y strain_xx strain_yy strain_xy stress_xx stress_yy stress_xy'
    boundary = 9
    sort_by = x
  [../]
  [./axis2]
    type = NodalValueSampler
    variable = 'disp_x disp_y strain_xx strain_yy strain_xy stress_xx stress_yy stress_xy'
    boundary = 10
    sort_by = y
  [../]
[]

[Outputs]
  exodus = true
  [./line_output]
    type = CSV
    file_base = 2d_snospd_plate_notch_planestress_uo
  [../]
[]
