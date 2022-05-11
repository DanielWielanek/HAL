/*
 * FemtoSourceModelExp.h
 *
 *  Created on: 1 mar 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTOSOURCEMODELEXP_H_
#define HALFEMTOSOURCEMODELEXP_H_

#include "FemtoPair.h"
#include "FemtoSourceModel.h"

/**
 * class that generate 1-dim exponential source with r = exp(-r/tau)
 */
namespace Hal {
  class FemtoSourceModelExp1D : public FemtoSourceModel1D {
  public:
    FemtoSourceModelExp1D();
    /**
     * copy ctor
     * @param model
     */
    FemtoSourceModelExp1D(const FemtoSourceModelExp1D& model);
    FemtoSourceModel* MakeCopy() const;
    void GenerateCoordinates(FemtoPair* Pair);
    virtual ~FemtoSourceModelExp1D();
    ClassDef(FemtoSourceModelExp1D, 1)
  };
}  // namespace Hal
#endif /* HALFEMTOSOURCEMODELEXP_H_ */
