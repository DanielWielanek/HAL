/*
 * FemtoWeightGeneratorBasic.h
 *  derived from ALICE AliFemtoWeightGeneratorBasic
 *  Created on: 21-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALFEMTOWEIGHTGENERATORBASIC_H_
#define HALFEMTOWEIGHTGENERATORBASIC_H_

#include "FemtoWeightGenerator.h"
/**
 * basic weight generator that take into account only quantum statistic, code
 * derived form AliROOT
 */
namespace Hal {
  class FemtoWeightGeneratorBasic : public FemtoWeightGenerator {
  private:
    FemtoWeightGenerator* GetGenerator() const;

  public:
    /**
     * default ctor
     */
    FemtoWeightGeneratorBasic();
    /**
     * copy ctor
     * @param aModel object to copy
     */
    FemtoWeightGeneratorBasic(const FemtoWeightGeneratorBasic& aModel);
    /**
     * operator =
     * @param aModel
     * @return
     */
    FemtoWeightGeneratorBasic& operator=(const FemtoWeightGeneratorBasic& aModel);
    virtual Double_t GenerateWeight(FemtoPair* track);
    virtual FemtoWeightGeneratorBasic* MakeCopy() const { return new FemtoWeightGeneratorBasic(*this); };
    virtual FemtoWeightGenerator* Clone(const char* newame = "") const;
    virtual Package* Report() const;
    virtual ~FemtoWeightGeneratorBasic();
    ClassDef(FemtoWeightGeneratorBasic, 1)
  };
}  // namespace Hal
#endif /* HALFEMTOWEIGHTGENERATORBASIC_H_ */
