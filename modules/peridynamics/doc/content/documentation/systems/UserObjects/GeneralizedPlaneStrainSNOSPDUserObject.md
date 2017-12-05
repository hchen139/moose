# GeneralizedPlaneStrainSNOSPDUserObject

!syntax description /UserObjects/GeneralizedPlaneStrainSNOSPDUserObject

## Description

The UserObject `GeneralizedPlaneStrainSNOSPDUserObject` is used to provide the residual and jacobian for the scalar out-of-plane strain field variable in a generalized plane strain formulation using the Self-stabilized Non-Ordinary State-based PeriDynamic theory. The out-of-plane stress component is retrieved from the `Compute***Stress` Material class. Integral of this component over the whole simulation domain is accomplished by summation of product of material point area and the component value. Residual is the difference between stress integral and the applied force in the our-of-plane direction. Jacobian is the derivative of residual with respect to the out-of-plane strain.

!syntax parameters /UserObjects/GeneralizedPlaneStrainSNOSPDUserObject

!syntax inputs /UserObjects/GeneralizedPlaneStrainSNOSPDUserObject

!syntax children /UserObjects/GeneralizedPlaneStrainSNOSPDUserObject
