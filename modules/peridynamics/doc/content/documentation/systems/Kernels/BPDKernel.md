# BPDKernel

!syntax description /Kernels/BPDKernel

## Description

The Kernel `BPDKernel` computes the residual and jacobian of the force density integral for 2D, and 3D bond-based peridynamic models under infinitesimal strain assumptions.

## Force function

The force state for each _Edge2_ element, i.e., bond, is

\begin{equation}
  \mathbf{f}\left(\mathbf{X}, \mathbf{X}^{\prime}, t\right) = cs\mathbf{M}
\end{equation}
where $c$ is the micro-modulus, $s$ is the bond stretch, and $\mathbf{M}$ being the unit vector in the direction of the deformed bond from $\mathbf{X}$ to $\mathbf{X}^{\prime}$.

More details on the residual and jacobian formulation can be found in [citet:Chen2016bondimplicit].

!syntax parameters /Kernels/BPDKernel

!syntax inputs /Kernels/BPDKernel

!syntax children /Kernels/BPDKernel
