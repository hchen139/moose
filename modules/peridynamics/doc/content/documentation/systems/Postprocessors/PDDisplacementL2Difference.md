# PDDisplacementL2Difference
!syntax description /Postprocessors/PDDisplacementL2Difference

## Description

The Postprocessor `PDDisplacementL2Difference` is used to calculate the L2 difference between displacement(s) and it/their analytical solution(s) using the discrete summation formulation.

\begin{equation}
\text{L2 difference} = \int_{\Omega} {\left| \mathbf{u}_{prediction} - \mathbf{u}_{analytical} \right|} d{\Omega} = \sum V_{\mathbf{X}} \cdot {\left| \mathbf{u}_{prediction}^{\mathbf{X}} - \mathbf{u}_{analytical}^{\mathbf{X}} \right|}
\end{equation}

where $V_{\mathbf{X}}$ is the area or volume of material point $\mathbf{X}$, and $\mathbf{u}_{prediction}^{\mathbf{X}}$ and $\mathbf{u}_{analytical}^{\mathbf{X}}$ are the prediction and analytical solution at material point $\mathbf{X}$.

!syntax parameters /Postprocessors/PDDisplacementL2Difference

!syntax inputs /Postprocessors/PDDisplacementL2Difference

!syntax children /Postprocessors/PDDisplacementL2Difference
