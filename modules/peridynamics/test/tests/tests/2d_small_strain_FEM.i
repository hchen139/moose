
[GlobalParams]
  displacements = 'disp_x disp_y'
  youngs_modulus = 2.1e8
  poissons_ratio = 0.3
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  ny = 20
  xmax = 1
  ymax = 0.4
  elem_type = QUAD4
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[AuxVariables]
  [./strain_xx]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./strain_yy]
    family = MONOMIAL
    order = CONSTANT
  [../]
  [./strain_xy]
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
[]

[BCs]
  [./left_dx]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0.0
  [../]
  [./left_dy]
    type = DirichletBC
    variable = disp_y
    boundary = left
    value = 0.0
  [../]
  [./right_dx]
    type = FunctionPresetBC
    variable = disp_x
    boundary = right
    function = '0.05*t'
  [../]
[]

[Modules/TensorMechanics/Master]
  [./all]
    strain = SMALL
  [../]
[]

[AuxKernels]
  [./strain_xx]
    type = RankTwoAux
    rank_two_tensor = total_strain
    variable = strain_xx
    index_i = 0
    index_j = 0
  [../]
  [./strain_yy]
    type = RankTwoAux
    rank_two_tensor = total_strain
    variable = strain_yy
    index_i = 1
    index_j = 1
  [../]
  [./strain_xy]
    type = RankTwoAux
    rank_two_tensor = total_strain
    variable = strain_xy
    index_i = 0
    index_j = 1
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
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
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

[Outputs]
  exodus = true
[]
