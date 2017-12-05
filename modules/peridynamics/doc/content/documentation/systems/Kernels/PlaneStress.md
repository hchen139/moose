# PlaneStress

!syntax description /Kernels/PlaneStress

## Description

The Kernel `PlaneStress` calculates the residual and jacobian entries for the out-of-plane strain variable in weak formulation of plane stress analysis. The in-plane displacement variables is governed by the `SmallSNOSPDKernel` for small strain analysis and the `FiniteSNOSPDKernel` for finite strain analysis.

!syntax parameters /Kernels/PlaneStress

!syntax inputs /Kernels/PlaneStress

!syntax children /Kernels/PlaneStress
