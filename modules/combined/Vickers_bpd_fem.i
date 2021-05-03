[GlobalParams]
  displacements = 'disp_x disp_y disp_z'
[]

[Mesh]
  type = PeridynamicsMesh
  horizon_number = 3

  [./fmg]
    type = FileMeshGenerator
    file = Vickers.e
  [../]
  [./mgpd]
    type = MeshGeneratorPD
    input = fmg
    retain_fe_mesh = false
    blocks_to_pd = 2
    construct_pd_sidesets = true
  [../]
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
  [./slave_gap_offset]
  [../]
  [./penetration]
  [../]
[]

[Functions]
  [./vertical_movement]
    type = ParsedFunction
    value = -t
  [../]
[]

[Kernels]
  [./TensorMechanics]
    use_displaced_mesh = false
    block = 1
  [../]
  [./empty]
    type = NullKernel
    variable = disp_x
    block = 10002
  [../]
[]

[Modules/Peridynamics/Mechanics/Master]
  [./blk2]
    formulation = BOND
    full_jacobian = true
    block = 1002
  [../]
[]

[AuxKernels]
  [./slave_gap_offset]
    type = BoundaryOffsetPD
    variable = slave_gap_offset
    boundary = 1003
  [../]
  [./penetration]
    type = PenetrationAux
    variable = penetration
    boundary = 1003
    paired_boundary = 2
  [../]
[]

[BCs]
  [./bottom_x]
    type = DirichletBC
    variable = disp_x
    boundary = 1004
    value = 0.0
  [../]
  [./bottom_y]
    type = DirichletBC
    variable = disp_y
    boundary = 1004
    value = 0.0
  [../]
  [./bottom_y]
    type = DirichletBC
    variable = disp_z
    boundary = 1004
    value = 0.0
  [../]
[]

[Materials]
  [./elasticity_tensor_fe]
    type = ComputeIsotropicElasticityTensor
    block = 1
    youngs_modulus = 1.21e12
    poissons_ratio = 0.22
  [../]
  [./strain_fe]
    type = ComputeSmallStrain
    block = 1
  [../]
  [./stress_fe]
    type = ComputeLinearElasticStress
    block = 1
  [../]

  [./elasticity_tensor_phantom]
    type = ComputeIsotropicElasticityTensor
    block = 10002
    youngs_modulus = 1e6
    poissons_ratio = 0.3
  [../]

  [./elasticity_tensor_pd]
    type = ComputeIsotropicElasticityTensor
    block = 1002
    youngs_modulus = 1e6
    poissons_ratio = 0.3
  [../]
  [./material_pd]
    type = ComputeSmallStrainVariableHorizonMaterialBPD
    block = 1002
  [../]
[]

[Contact]
  [./leftright]
    system = Constraint
    primary = 2
    secondary = 1003
    model = frictionless
    penalty = 1e6
    slave_gap_offset = slave_gap_offset
  [../]
[]

[Preconditioning]
  [./smp]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient

  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu superlu_dist'

  line_search = 'none'

  dt = 1
  dtmin = 0.1
  end_time = 4

  l_tol = 1e-6
  nl_rel_tol = 1e-10
  nl_abs_tol = 1e-8

  [./Predictor]
    type = SimplePredictor
    scale = 1.0
  [../]
[]

[Outputs]
  file_base = Vickers_bpd_fem_out
  [./exodus]
    type = Exodus
    elemental_as_nodal = true
  [../]
  [./console]
    type = Console
    max_rows = 5
  [../]
[]
