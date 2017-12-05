# NodalFunctionsL2Norm
!syntax description /Postprocessors/NodalFunctionsL2Norm

## Description

The Postprocessor `NodalFunctionsL2Norm` calculates the L2 norm of functions using the discrete summation formulation.

\begin{equation}
\text{L2} = \int_{\Omega} {\left| \mathbf{f} \right|} d{\Omega} = \sum V_{\mathbf{X}} \cdot {\left| \bar{\mathbf{f}}_{\mathbf{X}} \right|}
\end{equation}

where $V_{\mathbf{X}}$ is the area or volume of material point $\mathbf{X}$, and $\bar{\mathbf{f}}_{\mathbf{X}}$ is the evaluation of functions $\mathbf{f}$ at material point $\mathbf{X}$.

!syntax parameters /Postprocessors/NodalFunctionsL2Norm

!syntax inputs /Postprocessors/NodalFunctionsL2Norm

!syntax children /Postprocessors/NodalFunctionsL2Norm
