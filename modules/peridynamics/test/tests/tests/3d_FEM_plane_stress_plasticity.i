
[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
  volumetric_locking_correction = true
[]

[Mesh]
  type = FileMesh
  file = 3d_square_1elem.e
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]
[]

[AuxVariables]
  [./elastic_strain_xx]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./elastic_strain_yy]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./elastic_strain_xy]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./elastic_strain_xz]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./elastic_strain_yz]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./elastic_strain_zz]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./plastic_strain_xx]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./plastic_strain_yy]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./plastic_strain_xy]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./plastic_strain_xz]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./plastic_strain_yz]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./plastic_strain_zz]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./total_strain_xx]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./total_strain_yy]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./total_strain_xy]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./total_strain_xz]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./total_strain_yz]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./total_strain_zz]
    family = MONOMIAL
    order = CONSTANT
  [../]

  [./stress_xx]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./stress_yy]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./stress_xy]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./stress_xz]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./stress_yz]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./stress_zz]
    family = MONOMIAL
    order = CONSTANT
  [../]

  [./von_mises]
    family = MONOMIAL
    order = CONSTANT
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
    type = PresetBC
    variable = disp_x
    boundary = 1
    value = 0.0
  [../]
  [./bottom_dy]
    type = PresetBC
    variable = disp_y
    boundary = 3
    value = 0.0
  [../]
  [./back_dz]
    type = PresetBC
    variable = disp_z
    boundary = 5
    value = 0.0
  [../]
  [./right_dx]
    type = FunctionPresetBC
    variable = disp_x
    boundary = 2
    function = appl_dispx
  [../]
  [./top_dy]
    type = FunctionPresetBC
    variable = disp_y
    boundary = 4
    function = appl_dispy
  [../]
[]

[Kernels]
  [./disp_x]
    type = StressDivergenceTensors
    variable = disp_x
    component = 0
    use_displaced_mesh = true
  [../]
  [./disp_y]
    type = StressDivergenceTensors
    variable = disp_y
    component = 1
    use_displaced_mesh = true
  [../]
  [./disp_z]
    type = StressDivergenceTensors
    variable = disp_z
    component = 2
    use_displaced_mesh = true
  [../]
[]

[AuxKernels]
  [./elastic_strain_xx]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    variable = elastic_strain_xx
    index_i = 0
    index_j = 0
  [../]
  [./elastic_strain_yy]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    variable = elastic_strain_yy
    index_i = 1
    index_j = 1
  [../]
  [./elastic_strain_xy]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    variable = elastic_strain_xy
    index_i = 0
    index_j = 1
  [../]
  [./elastic_strain_xz]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    variable = elastic_strain_xz
    index_i = 0
    index_j = 2
  [../]
  [./elastic_strain_yz]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    variable = elastic_strain_yz
    index_i = 1
    index_j = 2
  [../]
  [./elastic_strain_zz]
    type = RankTwoAux
    rank_two_tensor = elastic_strain
    variable = elastic_strain_zz
    index_i = 2
    index_j = 2
  [../]

  [./plastic_strain_xx]
    type = RankTwoAux
    rank_two_tensor = plastic_strain
    variable = plastic_strain_xx
    index_i = 0
    index_j = 0
  [../]
  [./plastic_strain_yy]
    type = RankTwoAux
    rank_two_tensor = plastic_strain
    variable = plastic_strain_yy
    index_i = 1
    index_j = 1
  [../]
  [./plastic_strain_xy]
    type = RankTwoAux
    rank_two_tensor = plastic_strain
    variable = plastic_strain_xy
    index_i = 0
    index_j = 1
  [../]
  [./plastic_strain_xz]
    type = RankTwoAux
    rank_two_tensor = plastic_strain
    variable = plastic_strain_xz
    index_i = 0
    index_j = 2
  [../]
  [./plastic_strain_yz]
    type = RankTwoAux
    rank_two_tensor = plastic_strain
    variable = plastic_strain_yz
    index_i = 1
    index_j = 2
  [../]
  [./plastic_strain_zz]
    type = RankTwoAux
    rank_two_tensor = plastic_strain
    variable = plastic_strain_zz
    index_i = 2
    index_j = 2
  [../]

  [./total_strain_xx]
    type = RankTwoAux
    rank_two_tensor = total_strain
    variable = total_strain_xx
    index_i = 0
    index_j = 0
  [../]
  [./total_strain_yy]
    type = RankTwoAux
    rank_two_tensor = total_strain
    variable = total_strain_yy
    index_i = 1
    index_j = 1
  [../]
  [./total_strain_xy]
    type = RankTwoAux
    rank_two_tensor = total_strain
    variable = total_strain_xy
    index_i = 0
    index_j = 1
  [../]
  [./total_strain_xz]
    type = RankTwoAux
    rank_two_tensor = total_strain
    variable = total_strain_xz
    index_i = 0
    index_j = 2
  [../]
  [./total_strain_yz]
    type = RankTwoAux
    rank_two_tensor = total_strain
    variable = total_strain_yz
    index_i = 1
    index_j = 2
  [../]
  [./total_strain_zz]
    type = RankTwoAux
    rank_two_tensor = total_strain
    variable = total_strain_zz
    index_i = 2
    index_j = 2
  [../]

  [./stress_xx]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_xx
    index_i = 0
    index_j = 0
  [../]
  [./stress_yy]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_yy
    index_i = 1
    index_j = 1
  [../]
  [./stress_xy]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_xy
    index_i = 0
    index_j = 1
  [../]
  [./stress_xz]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_xz
    index_i = 0
    index_j = 2
  [../]
  [./stress_yz]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_yz
    index_i = 1
    index_j = 2
  [../]
  [./stress_zz]
    type = RankTwoAux
    rank_two_tensor = stress
    variable = stress_zz
    index_i = 2
    index_j = 2
  [../]

  [./vonmises]
    type = RankTwoScalarAux
    rank_two_tensor = stress
    scalar_type = VonMisesStress
    variable = von_mises
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.5e11
    poissons_ratio = 0.25
  [../]
  [./strain]
    type = ComputeFiniteStrain
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
  petsc_options_value = 'lu superlu_dist'

  end_time = 8
[]

[Outputs]
  exodus = true
[]
