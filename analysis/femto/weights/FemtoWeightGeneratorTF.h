/*
 * FemtoWeightGeneratorTF.h
 *
 *  Created on: 26 mar 2026
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_WEIGHTS_FEMTOWEIGHTGENERATORTF1_H_
#define HAL_ANALYSIS_FEMTO_WEIGHTS_FEMTOWEIGHTGENERATORTF1_H_

#include <RtypesCore.h>

#include "FemtoConst.h"
#include "FemtoWeightGenerator.h"

class TF1;
class TF3;


namespace Hal {
  /**
   * generates weights from TF1 by looking at k*
   */
  class FemtoWeightGeneratorTF1 : public FemtoWeightGenerator {
    std::vector<TF1*> fWeights;
    const Int_t fMaxPairsNo = {Hal::Femto::GetWeightPairSupportedNo()};

  public:
    FemtoWeightGeneratorTF1();
    FemtoWeightGeneratorTF1(const FemtoWeightGeneratorTF1& other);
    void SetWeightFunc(const TF1& func, Femto::EPairType type);
    virtual Double_t GenerateWeight(Hal::FemtoPair* pair);
    virtual ~FemtoWeightGeneratorTF1();
    virtual Hal::FemtoWeightGenerator* MakeCopy() const;
    ClassDef(FemtoWeightGeneratorTF1, 1)
  };
  /**
   * generates weights from TF3 by looking at k* out/side/long
   */
  class FemtoWeightGeneratorTF3 : public FemtoWeightGenerator {
    std::vector<TF3*> fWeights;
    const Int_t fMaxPairsNo = {Hal::Femto::GetWeightPairSupportedNo()};

  public:
    FemtoWeightGeneratorTF3();
    FemtoWeightGeneratorTF3(const FemtoWeightGeneratorTF3& other);
    void SetWeightFunc(const TF3& func, Femto::EPairType type);
    virtual Double_t GenerateWeight(Hal::FemtoPair* pair);
    virtual ~FemtoWeightGeneratorTF3();
    virtual Hal::FemtoWeightGenerator* MakeCopy() const;
    ClassDef(FemtoWeightGeneratorTF3, 1)
  };
} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_WEIGHTS_FEMTOWEIGHTGENERATORTF1_H_ */
