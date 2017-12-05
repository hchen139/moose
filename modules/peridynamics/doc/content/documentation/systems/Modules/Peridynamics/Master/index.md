# Master Action

The Peridynamics Master Action is a convenience object that simplifies part of the
mechanics system setup. It sets up force density integral Kernels for all displacements at once.

## Subblocks

The subblocks of the Master action are what triggers MOOSE objects to be built.
If none of the mechanics is subdomain restricted a single subblock will be used

```
[Modules/Peridynamics]
  [./Master]
    ...
  [../]
[]
```

If different mechanics models are needed, multiple subblocks with subdomain restrictions
can be used.

```
[Modules/Peridynamics/Master]
  [./block_a]
    ...
  [../]
  [./block_b]
    ...
  [../]
[]
```

## Toplevel parameters

Parameters supplied at the `[Modules/Peridynamics/Master]` level act as defaults for the Master action subblocks.

!syntax list /Modules/Peridynamics/Master objects=True actions=False subsystems=False

!syntax list /Modules/Peridynamics/Master objects=False actions=False subsystems=True

!syntax list /Modules/Peridynamics/Master objects=False actions=True subsystems=False
