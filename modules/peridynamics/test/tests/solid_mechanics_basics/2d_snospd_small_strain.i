
[GlobalParams]
  displacements = 'disp_x disp_y'
  bond_status = bond_status
  #full_jacobian = true
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
    function = '0.001*t'
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
  file_base = 2d_snospd_small_strain
  exodus = true
[]
