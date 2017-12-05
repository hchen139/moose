# GeneralizedPlaneStrainOSPDUserObject

!syntax description /UserObjects/GeneralizedPlaneStrainOSPDUserObject

## Description

The UserObject `GeneralizedPlaneStrainOSPDUserObject` is used to provide the residual and jacobian for the scalar out-of-plane strain field variable in a generalized plane strain formulation using the Ordinary State-based PeriDynamic theory. The out-of-plane stress component is calculated using the `NodalStressStrainAux` AuxKernel class. Integral of this component over the whole simulation domain is accomplished by summation of product of material point area and the component value. Residual is the difference between stress integral and the applied force in the our-of-plane direction. Jacobian is the derivative of residual with respect to the out-of-plane strain.

!syntax parameters /UserObjects/GeneralizedPlaneStrainOSPDUserObject

!syntax inputs /UserObjects/GeneralizedPlaneStrainOSPDUserObject

!syntax children /UserObjects/GeneralizedPlaneStrainOSPDUserObject
