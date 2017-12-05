# FailureIndexUserObject

!syntax description /UserObjects/FailureIndexUserObject

## Description

The UserObject `FailureIndexUserObject` is used to calculate the failure index at each material point. It's a local damage parameter which based on the status of each bond connected at the material point.

\begin{equation}
\phi \left( \mathbf{X}^{\prime} - \mathbf{X}, t \right) = 1 -\frac{\int_{\mathcal{H}_{\mathbf{X}}} \mu \left( \mathbf{X}^{\prime} - \mathbf{X}, t \right) dV^{\prime}}{\int_{\mathcal{H}_{\mathbf{X}}} 1 dV^{\prime}}
\end{equation}

where $\mu$ is the bond status parameter whose value is computed using AuxKernel `BondStatusAux`.

!syntax parameters /UserObjects/FailureIndexUserObject

!syntax inputs /UserObjects/FailureIndexUserObject

!syntax children /UserObjects/FailureIndexUserObject
