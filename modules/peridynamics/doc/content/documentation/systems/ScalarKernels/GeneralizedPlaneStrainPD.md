# GeneralizedPlaneStrainPD

!syntax description /ScalarKernels/GeneralizedPlaneStrainPD

## Description

The ScalarKernel `GeneralizedPlaneStrainPD` computes the residual and jacobian entry corresponding to the scalar variable. The values of residual and jacobian are retrieved from the `GeneralizedPlaneStrainPDUserObject` UserObjects.

## Residual

\begin{equation}
  r = \int_{\mathcal{A}} \sigma_{zz} d\mathcal{A}
\end{equation}

!syntax parameters /ScalarKernels/GeneralizedPlaneStrainPD

!syntax inputs /ScalarKernels/GeneralizedPlaneStrainPD

!syntax children /ScalarKernels/GeneralizedPlaneStrainPD
