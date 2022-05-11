/*
 * FemtoWeightGeneratorResidual.h
 *
 *  Created on: 30 sie 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_ANALYSIS_FEMTO_WEIGHTS_HALFEMTOWEIGHTGENERATORRESIDUAL_H_
#define HALFEMTO_ANALYSIS_FEMTO_WEIGHTS_HALFEMTOWEIGHTGENERATORRESIDUAL_H_

#include "FemtoWeightGenerator.h"


/**
 * basic class for generation of resitual correlations TODO - test this class
 */
namespace Hal {
  class FemtoPair;
  class McTrack;
  class FemtoWeightGeneratorResidual : FemtoWeightGenerator {
    FemtoWeightGeneratorResidual();
    FemtoWeightGenerator* fMainWeight;
    FemtoPair* fResPair;
    enum class eWeightType { kLednicky, kOther };
    eWeightType fWeightType;
    Bool_t fComplexFormat;
    void SetPairData(McTrack* tr1, McTrack* tr2);

  public:
    FemtoWeightGeneratorResidual(const FemtoWeightGenerator& w);
    FemtoWeightGeneratorResidual(const FemtoWeightGeneratorResidual& other);
    virtual Bool_t Init(Int_t task_id, FemtoPair* pair);
    virtual Double_t GenerateWeight(FemtoPair* pair);
    virtual Package* Report() const;
    FemtoWeightGeneratorResidual& operator=(const FemtoWeightGeneratorResidual& other);
    virtual ~FemtoWeightGeneratorResidual();
    ClassDef(FemtoWeightGeneratorResidual, 1)
  };
}  // namespace Hal


#endif /* HALFEMTO_ANALYSIS_FEMTO_WEIGHTS_HALFEMTOWEIGHTGENERATORRESIDUAL_H_ */
