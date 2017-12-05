# GeneralizedPlaneStrainPD Action

Sets up a generalized plane strain model, including kernel to provide coupled off-diagonal jacobian entries, scalar kernel to provide residual and diagonal jacobian, and user object to compute residual and diagonal jacobian for scalar variable.

## Subblocks

The subblocks of the GeneralizedPlaneStrain action are what triggers MOOSE objects to be built.
If none of the mechanics is subdomain restricted a single subblock will be used

```
[Modules/Peridynamics]
  [./GeneralizedPlaneStrain]
    ...
  [../]
[]
```

If different mechanics models are needed, multiple subblocks with subdomain restrictions
can be used.

```
[Modules/Peridynamics/GeneralizedPlaneStrain]
  [./block_a]
    ...
  [../]
  [./block_b]
    ...
  [../]
[]
```

## Toplevel parameters

Parameters supplied at the `[Modules/Peridynamics/GeneralizedPlaneStrain]` level act as defaults for the Master action subblocks.

!syntax list /Modules/Peridynamics/GeneralizedPlaneStrainPD objects=True actions=False subsystems=False

!syntax list /Modules/Peridynamics/GeneralizedPlaneStrainPD objects=False actions=False subsystems=True

!syntax list /Modules/Peridynamics/GeneralizedPlaneStrainPD objects=False actions=True subsystems=False
