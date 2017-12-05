
# CorrespondenceFiniteStrain

!syntax description /Materials/CorrespondenceFiniteStrain

## Description

The Material `CorrespondenceFiniteStrain` calculates the strain and rotation increments used in self-stabilized non-ordinary state-based peridynamic mechanics model under finite strain assumptions. The class first constructs the bond-associated [deformation gradient](peridynamics/DeformationGradients.md) and uses the [Rashid scheme](http://onlinelibrary.wiley.com/doi/10.1002/nme.1620362302/abstract) to formulate a incremental corotational finite strain model.

Note that Material `CorrespondenceFiniteStrain` can be used in general 3D and plane strain models.

!syntax parameters /Materials/CorrespondenceFiniteStrain

!syntax inputs /Materials/CorrespondenceFiniteStrain

!syntax children /Materials/CorrespondenceFiniteStrain
