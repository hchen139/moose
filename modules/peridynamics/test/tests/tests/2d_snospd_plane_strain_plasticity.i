
[GlobalParams]
  displacements = 'disp_x disp_y'
  bond_status = bond_status
  full_jacobian = true
[]

[Mesh]
  type = GeneratedMeshPD
  dim = 2
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
  [./elastic_strain_xx]
  [../]
  [./elastic_strain_yy]
  [../]
  [./elastic_strain_xy]
  [../]

  [./plastic_strain_xx]
  [../]
  [./plastic_strain_yy]
  [../]
  [./plastic_strain_xy]
  [../]

  [./total_strain_xx]
  [../]
  [./total_strain_yy]
  [../]
  [./total_strain_xy]
  [../]

  [./stress_xx]
  [../]
  [./stress_yy]
  [../]
  [./stress_xy]
  [../]

  [./von_mises]
  [../]
[]

[Functions]
  [./appl_dispx]
    type = PiecewiseLinear
    x = '0   1.0   2.0   3.0  4.0   5.0  6.0  7.0  8.0'
    y = '0.0 0.25e-4 0.50e-4 0.50e-4 0.50e-4 0.25e-4 0.0 0.0 0.0'
  [../]
  [./appl_dispy]
    type = PiecewiseLinear
    x = '0   1.0   2.0   3.0  4.0   5.0  6.0  7.0  8.0'
    y = '0.0 0.0  0.0 0.25e-4 0.50e-4 0.50e-4 0.50e-4  0.25e-4 0.0 '
  [../]
[]

[BCs]
  [./left_dx]
    type = DirichletBC
    variable = disp_x
    boundary = 0
    value = 0.0
  [../]
  [./bottom_dy]
    type = DirichletBC
    variable = disp_y
    boundary = 2
    value = 0.0
  [../]
  [./right_dx]
    type = FunctionPresetBC
    variable = disp_x
    boundary = 1
    function = appl_dispx
  [../]
  [./top_dy]
    type = FunctionPresetBC
    variable = disp_y
    boundary = 3
    function = appl_dispy
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

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.5e11
    poissons_ratio = 0.25
  [../]
  [./strain]
    type = CorrespondenceFiniteStrain
  [../]
  [./stress]
    type = ComputeMultipleInelasticStress
    inelastic_models = 'isoplasticity'
  [../]
  [./isoplasticity]
    type = IsotropicPlasticityStressUpdate
    yield_stress = 5e6
    hardening_constant = 6.25e10
    relative_tolerance = 1e-20
    absolute_tolerance = 1e-8
    max_inelastic_increment = 0.000001
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

  [./plastic_strain_xx]
    type = NodalRankTwoComponentUserObject
    rank_two_tensor = plastic_strain
    variable = plastic_strain_xx
    index_i = 0
    index_j = 0
  [../]
  [./plastic_strain_yy]
    type = NodalRankTwoComponentUserObject
    rank_two_tensor = plastic_strain
    variable = plastic_strain_yy
    index_i = 1
    index_j = 1
  [../]
  [./plastic_strain_xy]
    type = NodalRankTwoComponentUserObject
    rank_two_tensor = plastic_strain
    variable = plastic_strain_xy
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
    variable =  stress_xy
    index_i = 0
    index_j = 1
  [../]

  [./von_mises]
    type = NodalRankTwoScalarUserObject
    rank_two_tensor = stress
    variable =  von_mises
    scalar_type = VonMisesStress
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

  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu superlu_dist'

  dt = 0.5
  end_time = 8.0
[]

[Outputs]
  exodus = true
[]
