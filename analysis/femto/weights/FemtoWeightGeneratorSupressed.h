/*
 * FemtoWeightSupressed.h
 *
 *  Created on: 22 lis 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTOWEIGHTGENERATORSUPRESSED_H_
#define HALFEMTOWEIGHTGENERATORSUPRESSED_H_

#include "FemtoWeightGenerator.h"

/**
 * class for setting weight with supression factor (1-lambda)
 */
namespace Hal {
  class FemtoWeightGeneratorSupressed : public FemtoWeightGenerator {
    FemtoWeightGenerator* fWeightGenerator;
    Double_t fSupressionFactor;

  public:
    FemtoWeightGeneratorSupressed() : fWeightGenerator(nullptr), fSupressionFactor(0.0) {};
    /**
     * default c-tor
     * @param weight - code for real weight calculation
     */
    FemtoWeightGeneratorSupressed(const FemtoWeightGenerator& weight);
    /**
     * copy constructor
     * @param other
     */
    FemtoWeightGeneratorSupressed(const FemtoWeightGeneratorSupressed& other);
    /**
     * set supression factor (1-lambda)
     * @param factor
     */
    void SetSupressionFactor(Double_t factor);
    virtual Bool_t Init(Int_t task_id, FemtoPair* pair);
    virtual Double_t GenerateWeight(FemtoPair* pair);
    FemtoWeightGeneratorSupressed& operator=(const FemtoWeightGeneratorSupressed& other);
    virtual FemtoWeightGeneratorSupressed* MakeCopy() const { return new FemtoWeightGeneratorSupressed(*this); };
    virtual Package* Report() const;
    virtual ~FemtoWeightGeneratorSupressed();
    ClassDef(FemtoWeightGeneratorSupressed, 1)
  };
}  // namespace Hal

#endif /* HALFEMTOWEIGHTGENERATORSUPRESSED_H_ */
