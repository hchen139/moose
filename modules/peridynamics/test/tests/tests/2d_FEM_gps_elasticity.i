[GlobalParams]
  displacements = 'disp_x disp_y'
  scalar_out_of_plane_strain = strain_zz
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]

  [./strain_zz]
    order = FIRST
    family = SCALAR
  [../]
[]

[AuxVariables]
  [./temp]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxKernels]
  [./tempfuncaux]
    type = FunctionAux
    variable = temp
    function = tempfunc
    use_displaced_mesh = false
  [../]
[]

[Functions]
  [./tempfunc]
    type = ParsedFunction
    value = '(1-x)*100*t'
  [../]
[]

[BCs]
  [./bottomx]
    type = PresetBC
    boundary = bottom
    variable = disp_x
    value = 0.0
  [../]
  [./bottomy]
    type = PresetBC
    boundary = bottom
    variable = disp_y
    value = 0.0
  [../]
[]

[Modules]
  [./TensorMechanics]
    [./GeneralizedPlaneStrain]
      [./gps]
      [../]
    [../]
  [../]
[]

[Kernels]
  [./TensorMechanics]
  [../]
[]

[Materials]
  [./elasticity_tensor]
    type = ComputeIsotropicElasticityTensor
    youngs_modulus = 1e6
    poissons_ratio = 0.3
  [../]
  [./strain]
    type = ComputePlaneSmallStrain
    eigenstrain_names = thermal_strain
  [../]
  [./thermal_strain]
    type = ComputeThermalExpansionEigenstrain
    eigenstrain_name = thermal_strain
    temperature = temp
    thermal_expansion_coeff = 1.0e-5
    stress_free_temperature = 0.5
  [../]
  [./stress]
    type = ComputeLinearElasticStress
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
