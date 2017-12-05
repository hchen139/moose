
[GlobalParams]
  displacements = 'disp_x disp_y'
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

[Kernels]
  [./Peridynamics]
    formulation = NonOrdinaryState
    finite_strain_formulation = true
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 2.1e8
    poissons_ratio = 0.3
  [../]
  [./strain]
    type = CorrespondenceFiniteStrain
  [../]
  [./stress]
    type = ComputeFiniteStrainElasticStress
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
  solve_type = NEWTON
  line_search = basic

  petsc_options = '-snes_check_jacobian -snes_check_jacobian_view'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'

  start_time = 0
  dt = 1
  end_time = 1
[]

[Outputs]
  file_base = 2d_snospd_finite_strain_jacobian
  exodus = true
[]
