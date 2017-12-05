# Test 5(a-b) from NAFEMS Publication Ref: R0027

[GlobalParams]
  displacements = 'disp_x disp_y'
  temperature = temp
  bond_status = bond_status
  full_jacobian = true
[]

[Mesh]
  type = GeneratedMeshPD
  dim = 2
  xmax = 100
  ymax = 100
  nx = 100
  horizon_number = 3
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxVariables]
  [./temp]
    initial_condition = 1500.0
  [../]
  [./stress_xx]
  [../]
  [./stress_yy]
  [../]
  [./stress_xy]
  [../]
  [./vonmises]
  [../]
  [./creep_strain_xx]
  [../]
  [./creep_strain_yy]
  [../]
  [./creep_strain_xy]
  [../]
  [./elastic_strain_xx]
  [../]
  [./elastic_strain_yy]
  [../]
  [./elastic_strain_xy]
  [../]
  [./total_strain_xx]
  [../]
  [./total_strain_yy]
  [../]
  [./total_strain_xy]
  [../]
[]

[UserObjects]
  [./elastic_strain_xx]
    type = NodalRankTwoComponentUserObject
    rank_two_tensor = elastic_strain
    variable = elastic_strain_xx
    index_i = 0
    index_j = 0
  [../]
  [./elastic_strain_yy]
    type = NodalRankTwoComponentUserObject
    rank_two_tensor = elastic_strain
    variable = elastic_strain_yy
    index_i = 1
    index_j = 1
  [../]
  [./elastic_strain_xy]
    type = NodalRankTwoComponentUserObject
    rank_two_tensor = elastic_strain
    variable = elastic_strain_xy
    index_i = 0
    index_j = 1
  [../]

  [./creep_strain_xx]
    type = NodalRankTwoComponentUserObject
    rank_two_tensor = creep_strain
    variable = creep_strain_xx
    index_i = 0
    index_j = 0
  [../]
  [./creep_strain_yy]
    type = NodalRankTwoComponentUserObject
    rank_two_tensor = creep_strain
    variable = creep_strain_yy
    index_i = 1
    index_j = 1
  [../]
  [./creep_strain_xy]
    type = NodalRankTwoComponentUserObject
    rank_two_tensor = creep_strain
    variable = creep_strain_xy
    index_i = 0
    index_j = 1
  [../]

  [./total_strain_xx]
    type = NodalRankTwoComponentUserObject
    rank_two_tensor = total_strain
    variable = total_strain_xx
    index_i = 0
    index_j = 0
  [../]
  [./total_strain_yy]
    type = NodalRankTwoComponentUserObject
    rank_two_tensor = total_strain
    variable = total_strain_yy
    index_i = 1
    index_j = 1
  [../]
  [./total_strain_xy]
    type = NodalRankTwoComponentUserObject
    rank_two_tensor = total_strain
    variable = total_strain_xy
    index_i = 0
    index_j = 1
  [../]

  [./stress_xx]
    type = NodalRankTwoComponentUserObject
    rank_two_tensor = stress
    variable = stress_xx
    index_i = 0
    index_j = 0
  [../]
  [./stress_yy]
    type = NodalRankTwoComponentUserObject
    rank_two_tensor = stress
    variable = stress_yy
    index_i = 1
    index_j = 1
  [../]
  [./stress_xy]
    type = NodalRankTwoComponentUserObject
    rank_two_tensor = stress
    variable = stress_xy
    index_i = 0
    index_j = 1
  [../]

  [./vonmises]
    type = NodalRankTwoScalarUserObject
    rank_two_tensor = stress
    scalar_type = VonMisesStress
    variable = vonmises
  [../]
[]

[Modules]
  [./Peridynamics]
    [./Master]
      formulation = NonOrdinaryState
      finite_strain_formulation = true
      use_displaced_mesh = true
    [../]
  [../]
[]

[BCs]
  [./bottom_y]
    type = DirichletBC
    variable = disp_y
    boundary = 2
    value = 0.0
  [../]
  [./left_x]
    type = DirichletBC
    variable = disp_x
    boundary = 0
    value = 0.0
  [../]
  # [./top_disp]
  #  type = DirichletBC
  #  variable = disp_y
  #  boundary = 3
  #  value = 0.05
  # [../]
  # [./right_disp]
  #  type = DirichletBC
  #  variable = disp_x
  #  boundary = 1
  #  value = 0.1
  # [../]
[]

[NodalKernels]
  [./top_pull]
    type = ConstantRate
    variable = disp_y
    boundary = 3
    rate = 100.0
  [../]
  [./right_pull]
    type = ConstantRate
    variable = disp_x
    boundary = 1
    rate = 200.0
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 200e3
    poissons_ratio = 0.3
  [../]
  [./strain]
    type = CorrespondenceFiniteStrain
  [../]
  [./stress]
    type = ComputeMultipleInelasticStress
    inelastic_models = 'power_law_creep'
  [../]
  [./power_law_creep]
    type = PowerLawCreepStressUpdate
    coefficient = 3.125e-14
    n_exponent = 5.0
    m_exponent = 0.0
    activation_energy = 0.0
    max_inelastic_increment = 0.01
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

  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = ' lu       superlu_dist'

  line_search = 'none'

  nl_rel_tol = 1e-5
  nl_abs_tol = 1e-8
  end_time = 1000.0

  # [./TimeStepper]
  #   type = IterationAdaptiveDT
  #   dt = 1e-2
  #   time_t = '1e-2  0.13 1.0 5.93 6.0 35.29 36.0 169.51 170.0 572.16 573.0 1000.0'
  #   time_dt = '1e-2 0.87 1.0 0.07 1.0  0.71 10.0 0.49 100.0 0.84 100.0 100.0'
  #   optimal_iterations = 30
  #   iteration_window = 9
  #   growth_factor = 2.0
  #   cutback_factor = 0.5
  # [../]

  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 1e-2
    time_t = '1.0 8.46 9.0 43.01 44.0 107.58 108.0 365.86 366.0 710.23 711.0 1000.0'
    time_dt = '1.0 0.54 1.0 0.99 1.0 0.42 10.0 0.14 10.0 0.77 100.0 100.0'
    optimal_iterations = 30
    iteration_window = 9
    growth_factor = 2.0
    cutback_factor = 0.5
  [../]
[]

[Outputs]
  exodus = true
[]
