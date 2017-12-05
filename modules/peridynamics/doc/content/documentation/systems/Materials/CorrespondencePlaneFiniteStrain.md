
# CorrespondencePlaneFiniteStrain

!syntax description /Materials/CorrespondencePlaneFiniteStrain

## Description

The Material `CorrespondencePlaneFiniteStrain` calculates the strain and rotation increments used in self-stabilized non-ordinary state-based peridynamic mechanics model under finite strain assumptions for two dimensional analysis. The class first constructs the planar bond-associated [deformation gradient](peridynamics/DeformationGradients.md) and uses the [Rashid scheme](http://onlinelibrary.wiley.com/doi/10.1002/nme.1620362302/abstract) to formulate a incremental corotational finite strain model.

Note that Material `CorrespondencePlaneFiniteStrain` must be used for _weak_ plane stress and generalized plane strain models to incorporate the out-of-plane strain components.

!syntax parameters /Materials/CorrespondencePlaneFiniteStrain

!syntax inputs /Materials/CorrespondencePlaneFiniteStrain

!syntax children /Materials/CorrespondencePlaneFiniteStrain
