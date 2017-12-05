# Peridynamic Mechanics Model

!media peridynamics/peridynamic_models.png style=width:200px;padding-left:20px;float:top;
       caption=Schematics of bond-based (a), ordinary (b) and non-ordinary (c) state based peridynamic material response

The first peridynamic model, termed the bond-based peridynamic model (BPD), was proposed by Silling in the year of 2000 [citet:Silling2000bond]. In BPD, material points interact in a pair-wise fashion with neighboring material points that fall within their horizon. The interaction between two material points depends only on their own deformations.

Later, Silling et al. [citet:Silling2007states] generalized BPD model in what is termed state-based peridynamic models (SPD) by introducing the concept of states. In SPD, the force state between two material points depends not only on their own deformation states, but also on the deformation states of other material points within their horizons. Depending on the direction of force state between a material point pair, SPD models can be classified into ordinary state-based peridynamic (OSPD) and non-ordinary state-based peridynamic (NOSPD) models.

SPD models overcome several issues within BPD model, such as only permitting a fixed Poisson’s ratio of 0.25, inconsistency in modeling plastic deformation for metals, and requiring a complete recast of standard continuum material models in terms of pairwise force function to permit their use within peridynamics. A review of BPD and SPD and their applications can be found in [citet:Bobaru2016handbook].

In peridynamics theory, the Equation of Motion (EOM) for a material point $\mathbf{X}$ in the reference configuration at time $t$ is given by

\begin{equation}
  \rho\left(\mathbf{X}\right) \ddot{\mathbf{u}}\left( \mathbf{X},t \right) = \int_{\mathcal{H}_{\mathbf{X}}}\mathbf{f}\left(\mathbf{X},\mathbf{X}^{\prime},t \right)dV_{X^{\prime}} + \mathbf{b}\left( \mathbf{X},t \right), \hspace{20 pt} \forall\left(\mathbf{X},t\right) \in \Omega_{r} \times \left(0, T\right)
\end{equation}
where $\rho\left(\mathbf{X} \right)$ is the mass density, $\mathbf{f}\left(\mathbf{X}, \mathbf{X}^{\prime}, t\right)$ is the force density function, and $\mathbf{b}\left(\mathbf{X},t\right)$ is external force density vector.

Depending on different peridynamics models, the force density function has different formulation. Various formulations for force density functions are outlined below.

## Bond-based model

\begin{equation}
  \mathbf{f}\left(\mathbf{X}, \mathbf{X}^{\prime}, t\right) = cs\mathbf{M}
\end{equation}
with $\textit{micro-modulus}$ $c$ has values of

for $\textbf{regular uniform}$ spatial discretization

\begin{equation}
  c = \left\{\begin{matrix}
    12K^{\prime} / \pi h_2 \delta^{3} & \text{two dimensional} \\
    18K / \pi \delta^{4} & \text{three dimensional}
  \end{matrix}\right.
\end{equation}

for $\textbf{irregular non-uniform}$ spatial discretization

\begin{equation}
  c = \left\{ \begin{matrix}
    4K^{\prime} / \left| \boldsymbol{\xi} \right| \cdot c_v & \text{two dimensional} \\
    9K / \left| \boldsymbol{\xi} \right| \cdot c_v & \text{three dimensional}
    \end{matrix} \right.
\end{equation}
with
\begin{equation}
  c_v = \frac{\int_{\mathcal{H}_{\mathbf{X}}}dV +  \int_{\mathcal{H}_{\mathbf{X}^{\prime}}} dV} {\int_{\mathcal{H}_{\mathbf{X}}} dV \cdot \int_{\mathcal{H}_{\mathbf{X}^{\prime}}} dV}
\end{equation}
where $K$ is the bulk modulus and $K^{\prime}$ is the two-dimensional bulk modulus, $E$ is the Young's modulus, $h_2$ is the plane thickness and $h_1$ is the cross-sectional area.
\begin{equation}
  K^{\prime} = \left\{ \begin{matrix}
    E/2\left(1-v\right) & \text{plane stress} \\
    E/2(1-v-2v^2) & \text{plane strain}
  \end{matrix} \right.
\end{equation}
where $v$ is the Poisson's Ratio.

$s$ being the $\textit{bond stretch}$ and defined as the change in bond length divided by initial bond length

\begin{equation}
  s = \frac{e}{\left| \boldsymbol{\xi} \right|} = \frac{\left( \left| \mathbf{x}^{\prime}\left(\mathbf{X}^{\prime}, t \right) - \mathbf{x} \left( \mathbf{X},t\right)\right| - \left| \mathbf{X}^{\prime} - \mathbf{X} \right|\right)}{\left| \mathbf{X}^{\prime} - \mathbf{X} \right|}  = \frac{\left| \boldsymbol{\xi} + \boldsymbol{\eta} \right| - \left| \boldsymbol{\xi} \right|}{\left| \boldsymbol{\xi} \right|}
\end{equation}

$\mathbf{M}$ being the unit vector in the direction of the deformed bond from $\mathbf{X}$ to $\mathbf{X}^{\prime}$

\begin{equation}
  \mathbf{M} = \frac{\mathbf{x}^{\prime} \left( \mathbf{X}^{\prime}, t \right) - \mathbf{x} \left( \mathbf{X}, t \right)}{\left| \mathbf{x}^{\prime} \left( \mathbf{X}^{\prime}, t \right) - \mathbf{x} \left( \mathbf{X}, t \right) \right|} = \frac{\boldsymbol{\xi} + \boldsymbol{\eta}}{\left| \boldsymbol{\xi} + \boldsymbol{\eta} \right|}
\end{equation}

## Ordinary state-based model

\begin{equation}
  \mathbf{f} \left( \mathbf{X}, \mathbf{X}^{\prime}, t \right) = \left( \underline{\mathbf{T}} \left( \mathbf{X}, t \right) - \underline{\mathbf{T}} \left(\mathbf{X}^{\prime}, t \right) \right) \cdot \mathbf{M}
\end{equation}
where $\mathbf{M}$ is the unit vector in deformed configuration.

\begin{equation}
  \underline{mathbf{T}} \left( \mathbf{X}, t \right) = 2 \underline{\omega} \left\langle \boldsymbol{\xi} \right\rangle \left( d_{\mathbf{X}} \cdot a \cdot \theta_{\mathbf{X}} + b \cdot \boldsymbol{\xi} \cdot s \right)
\end{equation}

\begin{equation}
  \underline{\mathbf{T}} \left( \mathbf{X}, t \right) = -2 \underline{\omega} \left\langle \boldsymbol{\xi} \right\rangle \left( d_{\mathbf{X}^{\prime}} \cdot a \cdot \theta_{\mathbf{X}^{\prime}} + b \cdot \boldsymbol{\xi} \cdot s \right)
\end{equation}
where $s$ is the bond stretch, $\theta$ is the dilatation at a material point which can be calculated as

\begin{equation}
  \theta = \int_{\mathcal{H}} d \cdot \omega \left\langle \boldsymbol{\xi} \right\rangle \cdot s \cdot \boldsymbol{\xi} dV
\end{equation}

and

\begin{equation}
  a = \left\{\begin{matrix}
    \left( K^{\prime} - 2 \mu \right) / 2 & \text{two dimensional} \\
    \left( K - 5 \mu / 3 \right) / 2 & \text{three dimensional}
  \end{matrix}\right.
\end{equation}

for $\textbf{regular uniform}$ spatial discretization

\begin{equation}
  d = \left\{\begin{matrix}
    2 / \pi h_2 \delta^3 & \text{two dimensional} \\
    9 / 4 \pi \delta^4 & \text{three dimensional}
  \end{matrix}\right.
\end{equation}

\begin{equation}
  b = \left\{\begin{matrix}
    6 \mu / \pi h_2 \delta^4 & \text{two dimensional} \\
    15 \mu / 2 \pi \delta^5 & \text{three dimensional}
  \end{matrix}\right.
\end{equation}

for $\textbf{irregular non-uniform}$ spatial discretization

\begin{equation}
  d = \left\{\begin{matrix}
    2 \bigg/ \left(\delta \int_{\mathcal{H}} dA \right) & \text{two dimensional} \\
    3 \bigg/ \left(\delta \int_{\mathcal{H}} dV \right) & \text{three dimensional}
  \end{matrix}\right.
\end{equation}

\begin{equation}
  \delta_{\mathbf{X}} \cdot b_{\mathbf{X}} + \delta_{\mathbf{X}^{\prime}} \cdot b_{\mathbf{X}^{\prime}} = \left\{ \begin{matrix}
    2 \left( K^{\prime} / 2 - a \right) / \left| \boldsymbol{\xi} \right| & \text{two dimensional} \\
    3 \left( K / 2 - a \right) / \left| \boldsymbol{\xi} \right| & \text{three dimensional}
  \end{matrix} \right\} \cdot \left( \delta_{\mathbf{X}} \cdot d_{\mathbf{X}} + \delta_{\mathbf{X}^{\prime}} \cdot d_{\mathbf{X}^{\prime}} \right)
\end{equation}
with $\mu$ is the shear modulus.

## Non-ordinary state-based model

The general expression for force density function can be written as:
\begin{equation}
  \mathbf{f} \left( \mathbf{X}, \mathbf{X}^{\prime}, t \right) = \underline{\mathbf{T}} \left[ \mathbf{X}, t\right] \left\langle \mathbf{X}^{\prime} - \mathbf{X} \right\rangle - \underline{\mathbf{T}}\left[ \mathbf{X}^{\prime}, t \right] \left\langle \mathbf{X} - \mathbf{X}^{\prime} \right\rangle
\end{equation}
where $\underline{\mathbf{T}} \left[ \mathbf{X}, t \right] \left\langle \mathbf{X}^{\prime} - \mathbf{X} \right\rangle$, in short $\underline{\mathbf{T}} \left\langle \boldsymbol{\xi} \right\rangle$, is the force density state exerted on material point $\mathbf{X}$ from $\mathbf{X}^{\prime}$, while $\underline{\mathbf{T}}\left[\mathbf{X}^{\prime}, t \right] \left\langle \mathbf{X} - \mathbf{X}^{\prime} \right\rangle$, in short $\underline{\mathbf{T}} \left\langle  - \boldsymbol{\xi} \right\rangle$, is the force density state exerted on material point $\mathbf{X}^{\prime}$ from $\mathbf{X}$.

### Conventional correspondence material model

\begin{equation}
  \underline{\mathbf{T}} \left\langle \boldsymbol{\xi} \right\rangle = \underline{\omega} \left\langle \boldsymbol{\xi} \right\rangle \mathbf{P}_{\mathbf{X}} \mathbf{K}_{\mathbf{X}}^{-1} \boldsymbol{\xi}
\end{equation}

\begin{equation}
  \underline{\mathbf{T}} \left\langle - \boldsymbol{\xi} \right\rangle = - \underline{\omega} \left\langle \boldsymbol{\xi} \right\rangle \mathbf{P}_{\mathbf{X}^{\prime}} \mathbf{K}_{\mathbf{X}^{\prime}}^{-1} \boldsymbol{\xi}
\end{equation}
where $\mathbf{P}$ is the first Piola-Kirchhoff stress tensor and $\mathbf{K}$ is the shape tensor. Definition of shape tensor can be found on [Deformation Gradients](peridynamics/DeformationGradients.md) page.

### Bond-associated correspondence material model

\begin{equation}
  \underline{\mathbf{T}} \left\langle \boldsymbol{\xi} \right\rangle = \frac{\int_{\mathcal{H}_{\mathbf{X}} \cap h_{\mathbf{X}, \boldsymbol{\xi}}} 1 dV_{\mathbf{X}^{\prime}}}{\int_{\mathcal{H}_{\mathbf{X}}} 1 dV_{\mathbf{X}^{\prime}}} \underline{\omega} \left\langle \boldsymbol{\xi} \right\rangle \mathbf{P}_{\mathbf{X, \boldsymbol{\xi}}} \mathbf{K}_{\mathbf{X, \boldsymbol{\xi}}}^{-1} \boldsymbol{\xi}
\end{equation}

\begin{equation}
  \underline{\mathbf{T}} \left\langle - \boldsymbol{\xi} \right\rangle = - \frac{\int_{\mathcal{H}_{\mathbf{X}^{\prime}} \cap h_{\mathbf{X}^{\prime}, - \boldsymbol{\xi}}} 1 dV_{\mathbf{X}}}{\int_{\mathcal{H}_{\mathbf{X}^{\prime}}} 1 dV_{\mathbf{X}}} \underline{\omega} \left\langle \boldsymbol{\xi} \right\rangle \mathbf{P}_{\mathbf{X}^{\prime}, - \boldsymbol{\xi}} \mathbf{K}_{\mathbf{X}^{\prime}, - \boldsymbol{\xi}}^{-1} \boldsymbol{\xi}
\end{equation}

It should be noted that the First Piola-Kirchhoff stress tensor $\mathbf{P}$ and shape tensor $\mathbf{K}$ are all bond-associated quatities.

# Peridynamic Heat Conduction Model

The peridynamic heat conduction equation is

\begin{equation}
  \rho\left(\mathbf{X} \right) C \dot{T} \left( \mathbf{X}, t \right) = \int_{\mathcal{H}_{\mathbf{X}}} f \left( \mathbf{X},\mathbf{X}^{\prime}, t \right)dV_{X^{\prime}} + q \left( \mathbf{X},t \right), \hspace{20 pt} \forall \left( \mathbf{X}, t \right) \in \Omega_{r} \times \left( 0, T \right)
\end{equation}
where $C$ is the specific heat capacity, $f$ is the response function, and $q$ is specific heat source or sink.

## Bond-based model

\begin{equation}
  f \left(\mathbf{X}, \mathbf{X}^{\prime}, t \right) = k \frac{T \left( \mathbf{X}^{\prime}, t \right) - T \left( \mathbf{X}, t \right)}{\left| \mathbf{\xi} \right|}
\end{equation}
with $\textit{micro-conductivity}$ $k$ has values of

for $\textbf{regular uniform}$ spatial discretization

\begin{equation}
  k = \left\{\begin{matrix}
    6 \kappa / \pi h_2 \delta^{3} & \text{two dimensional} \\
    6 \kappa / \pi \delta^{4} & \text{three dimensional}
  \end{matrix}\right.
\end{equation}

for $\textbf{irregular non-uniform}$ spatial discretization

\begin{equation}
  k = \left\{ \begin{matrix}
    2 \kappa / \left| \boldsymbol{\xi} \right| \cdot c_v & \text{two dimensional} \\
    3 \kappa / \left| \boldsymbol{\xi} \right| \cdot c_v & \text{three dimensional}
  \end{matrix} \right.
\end{equation}
where $\kappa$ is the material thermal conductivity, $c_v$ is the volume coefficient which has the same expression as in bond-based mechanics model.
