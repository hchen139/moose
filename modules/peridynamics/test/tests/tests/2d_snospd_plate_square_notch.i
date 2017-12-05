
[GlobalParams]
  displacements = 'disp_x disp_y'
  bond_status = bond_status
  full_jacobian = true
  plane_stress = true
  youngs_modulus = 2.1e8
  poissons_ratio = 0.3
  out_of_plane_strain = strain_zz
[]

[Mesh]
  type = GeneratedMeshPD
  dim = 2
  nx = 400
  horizon_number = 3
  #horizon_radius = 0.003
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
  [./strain_zz]
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

  #[./left_dx]
  #  type = FunctionPresetBC
  #  variable = disp_x
  #  boundary = 0
  #  function = '-0.0002*t'
  #[../]
  #[./right_dx]
  #  type = FunctionPresetBC
  #  variable = disp_x
  #  boundary = 1
  #  function = '0.0002*t'
  #[../]

  #[./leftcenter_dy]
  #  type = PresetBC
  #  variable = disp_y
  #  boundary = 5
  #  value = 0.0
  #[../]
  #[./rightcenter_dy]
  #  type = PresetBC
  #  variable = disp_y
  #  boundary = 6
  #  value = 0.0
  #[../]
  #[./topcenter_dx]
  #  type = PresetBC
  #  variable = disp_x
  #  boundary = 8
  #  value = 0.0
  #[../]
  #[./bottomcenter_dx]
  #  type = PresetBC
  #  variable = disp_x
  #  boundary = 7
  #  value = 0.0
  #[../]
[]

#[NodalKernels]
#  #[./left_fx]
#  #  type = ConstantRate
#  #  variable = disp_x
#  #  boundary = 0
#  #  rate = -0.25
#  #[../]
#  #[./right_fx]
#  #  type = ConstantRate
#  #  variable = disp_x
#  #  boundary = 1
#  #  rate = 0.25
#  #[../]
#  [./left_fx]
#    type = UserForcingFunctionNodalKernel
#    variable = disp_x
#    boundary = 0
#    function = -250
#  [../]
#  [./right_fx]
#    type = UserForcingFunctionNodalKernel
#    variable = disp_x
#    boundary = 1
#    function = 250
#  [../]
#[]

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

[AuxKernels]
  [./strain_xx]
    type = NodalStressStrainAux
    rank_two_tensor = total_strain
    variable = strain_xx
    quantity_type = Component
    index_i = 0
    index_j = 0
  [../]
  [./strain_yy]
    type = NodalStressStrainAux
    rank_two_tensor = total_strain
    variable = strain_yy
    quantity_type = Component
    index_i = 1
    index_j = 1
  [../]
  [./strain_xy]
    type = NodalStressStrainAux
    rank_two_tensor = total_strain
    variable = strain_xy
    quantity_type = Component
    index_i = 0
    index_j = 1
  [../]

  [./stress_xx]
    type = NodalStressStrainAux
    rank_two_tensor = stress
    variable = stress_xx
    quantity_type = Component
    index_i = 0
    index_j = 0
  [../]
  # [./stress_yy]
  #   type = NodalStressStrainAux
  #   rank_two_tensor = stress
  #   variable = stress_yy
  #   quantity_type = Component
  #   index_i = 1
  #   index_j = 1
  # [../]
  [./stress_xy]
    type = NodalStressStrainAux
    rank_two_tensor = stress
    variable = stress_xy
    quantity_type = Component
    index_i = 0
    index_j = 1
  [../]
[]

[UserObjects]
  [./stress_yy]
    type = NodalRankTwoComponentUserObject
    rank_two_tensor = stress
    variable = stress_yy
    index_i = 1
    index_j = 1
  [../]
[]

[Materials]
  [./elasticity]
    type = ComputeIsotropicElasticityTensor
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

  petsc_options = '-snes_ksp_ew'
  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -snes_lag_jacobian'
  petsc_options_value = 'lu superlu_dist 10'

  end_time = 1
[]

[VectorPostprocessors]
  #[./crack]
  #  type = NodalValueSampler
  #  variable = 'disp_x disp_y strain_xx strain_yy strain_xy stress_xx stress_yy stress_xy'
  #  boundary = 10
  #  sort_by = y
  #[../]
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
    file_base = 2d_snospd_plate_square_notch
  [../]
[]
