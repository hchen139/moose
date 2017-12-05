//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef PDNODALINTEGRALPOSTPROCESSOR_H
#define PDNODALINTEGRALPOSTPROCESSOR_H

#include "PDNodalPostprocessor.h"

class PDNodalIntegralPostprocessor;

template <>
InputParameters validParams<PDNodalIntegralPostprocessor>();

/**
 * This postprocessor computes a volume integral of the specified variable.
 *
 * Note that specializations of this integral are possible by deriving from this
 * class and overriding computeNodalIntegral().
 */
class PDNodalIntegralPostprocessor : public PDNodalPostprocessor
{
public:
  PDNodalIntegralPostprocessor(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual void threadJoin(const UserObject & uo) override;
  virtual Real getValue() override;

protected:
  virtual Real computeNodalValue() = 0;

  Real _integral_value;
};

#endif // PDNODALINTEGRALPOSTPROCESSOR
