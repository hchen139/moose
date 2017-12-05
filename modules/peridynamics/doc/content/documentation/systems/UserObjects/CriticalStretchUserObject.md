# CriticalStretchUserObject

!syntax description /UserObjects/CriticalStretchUserObject

## Description

The UserObject `CriticalStretchUserObject` is used to assign values for the `critical_stretch` aux variable. The values can be uniform for all bond or follow normal distribution.

The usage of GeneralUserObject rather than AuxKernel for this purpose is to avoid repeating the same sampling procedure on each processors.

!syntax parameters /UserObjects/CriticalStretchUserObject

!syntax inputs /UserObjects/CriticalStretchUserObject

!syntax children /UserObjects/CriticalStretchUserObject
