
[GlobalParams]
  displacements = 'disp_x disp_y'
  bond_status = bond_status
  full_jacobian = true
[]

[Mesh]
  type = FileMeshPD
  # file = sh_91.e
  # file = sh_936.e
  # file = sh_4901.e
  file = sh_9353.e
  horizon_number = 3
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
[]

[BCs]
  [./left_x]
    type = DirichletBC
    variable = disp_x
    boundary = 1
    value = 0.0
  [../]
  [./bottom_y]
    type = DirichletBC
    variable = disp_y
    boundary = 3
    value = 0.0
  [../]
  [./right_x]
    type = FunctionPresetBC
    variable = disp_x
    boundary = 2
    function = '7.280419e-4*(x+0.5)/((x+0.5)^2+(y+0.5)^2)*(12.499281*((x+0.5)^2+(y+0.5)^2)+5.6+(2-0.448026/((x+0.5)^2+(y+0.5)^2))*(4*(x+0.5)*(x+0.5)/((x+0.5)^2+(y+0.5)^2)-3))'
  [../]
  [./right_y]
    type = FunctionPresetBC
    variable = disp_y
    boundary = 2
    function = '7.280419e-4*(y+0.5)/((x+0.5)^2+(y+0.5)^2)*(-5.356835*((x+0.5)^2+(y+0.5)^2)-1.6+(2-0.448026/((x+0.5)^2+(y+0.5)^2))*(3-4*(y+0.5)*(y+0.5)/((x+0.5)^2+(y+0.5)^2)))'
  [../]
  [./top_x]
    type = FunctionPresetBC
    variable = disp_x
    boundary = 4
    function = '7.280419e-4*(x+0.5)/((x+0.5)^2+(y+0.5)^2)*(12.499281*((x+0.5)^2+(y+0.5)^2)+5.6+(2-0.448026/((x+0.5)^2+(y+0.5)^2))*(4*(x+0.5)*(x+0.5)/((x+0.5)^2+(y+0.5)^2)-3))'
  [../]
  [./top_y]
    type = FunctionPresetBC
    variable = disp_y
    boundary = 4
    function = '7.280419e-4*(y+0.5)/((x+0.5)^2+(y+0.5)^2)*(-5.356835*((x+0.5)^2+(y+0.5)^2)-1.6+(2-0.448026/((x+0.5)^2+(y+0.5)^2))*(3-4*(y+0.5)*(y+0.5)/((x+0.5)^2+(y+0.5)^2)))'
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
    youngs_modulus = 1.0e6
    poissons_ratio = 0.3
  [../]
  [./strain]
    type = CorrespondencePlaneSmallStrain
  [../]
  [./stress]
    type = ComputeLinearElasticStress
  [../]
[]

[Postprocessors]
  [./disp_mag_diff]
    type = PDDisplacementL2Difference
    function_0 = '7.280419e-4*(x+0.5)/((x+0.5)^2+(y+0.5)^2)*(12.499281*((x+0.5)^2+(y+0.5)^2)+5.6+(2-0.448026/((x+0.5)^2+(y+0.5)^2))*(4*(x+0.5)*(x+0.5)/((x+0.5)^2+(y+0.5)^2)-3))'
    function_1 = '7.280419e-4*(y+0.5)/((x+0.5)^2+(y+0.5)^2)*(-5.356835*((x+0.5)^2+(y+0.5)^2)-1.6+(2-0.448026/((x+0.5)^2+(y+0.5)^2))*(3-4*(y+0.5)*(y+0.5)/((x+0.5)^2+(y+0.5)^2)))'
  [../]
  [./disp_mag_exact]
    type = NodalFunctionsL2Norm
    function_0 = '7.280419e-4*(x+0.5)/((x+0.5)^2+(y+0.5)^2)*(12.499281*((x+0.5)^2+(y+0.5)^2)+5.6+(2-0.448026/((x+0.5)^2+(y+0.5)^2))*(4*(x+0.5)*(x+0.5)/((x+0.5)^2+(y+0.5)^2)-3))'
    function_1 = '7.280419e-4*(y+0.5)/((x+0.5)^2+(y+0.5)^2)*(-5.356835*((x+0.5)^2+(y+0.5)^2)-1.6+(2-0.448026/((x+0.5)^2+(y+0.5)^2))*(3-4*(y+0.5)*(y+0.5)/((x+0.5)^2+(y+0.5)^2)))'
  [../]
  [./disp_mag_error]
    type = RatioPostprocessor
    dividend = disp_mag_diff
    divisor = disp_mag_exact
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

  end_time = 1.0
[]

[Outputs]
  exodus = true
[]
