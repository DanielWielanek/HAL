/*
 * FemtoWeightGeneratorScottPratt.h
 *
 *  Created on: 30 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef ANALYSIS_FEMTO_WEIGHTS_FEMTOWEIGHTGENERATORSCOTTPRATT_H_
#define ANALYSIS_FEMTO_WEIGHTS_FEMTOWEIGHTGENERATORSCOTTPRATT_H_


#include "FemtoWeightGenerator.h"

namespace Hal {
  namespace ScottPratt {
    class CWaveFunction;
  }  // namespace ScottPratt
  class FemtoWeightGeneratorScottPratt : public FemtoWeightGenerator {
    Hal::ScottPratt::CWaveFunction** fWaveSquares;  //!
    Hal::ScottPratt::CWaveFunction** fWavePhases;   //!
    Hal::ScottPratt::CWaveFunction** fWave;         //!
    Hal::ScottPratt::CWaveFunction* fCurWeight;     //!
    double fPa[4], fPb[4];
    double fXa[4], fXb[4];
    TString fParFile;
    void InitWeights();
    enum class EWeightApprox { kSquare, kPhase, kOther };
    EWeightApprox fPref = {EWeightApprox::kSquare};
    Hal::ScottPratt::CWaveFunction* GetWave(Int_t n, EWeightApprox w);
    Hal::ScottPratt::CWaveFunction* InitSquareWave(Int_t n);
    Hal::ScottPratt::CWaveFunction* InitPhaseWave(Int_t n);
    Hal::ScottPratt::CWaveFunction* InitOtherWave(Int_t n);

  public:
    FemtoWeightGeneratorScottPratt(TString pairFile = "");
    FemtoWeightGeneratorScottPratt(const FemtoWeightGeneratorScottPratt& other);
    /**
     * prefer approximation with square well potential
     */
    void PrefferSquareApprox() { fPref = EWeightApprox::kSquare; }
    /**
     * prefer pure partial-wave based algorithm
     */
    void PrefferPhaseAprrox() { fPref = EWeightApprox::kPhase; }
    /**
     * prefer other algorithm
     */
    void PrefferOtherApprox() { fPref = EWeightApprox::kOther; }
    virtual void SetPairType(Femto::EPairType aPairType);
    virtual FemtoWeightGenerator* MakeCopy() const { return new FemtoWeightGeneratorScottPratt(*this); };
    virtual Double_t GenerateWeight(FemtoPair* pair);
    virtual ~FemtoWeightGeneratorScottPratt();
    ClassDef(FemtoWeightGeneratorScottPratt, 1)
  };

} /* namespace Hal */

#endif /* ANALYSIS_FEMTO_WEIGHTS_FEMTOWEIGHTGENERATORSCOTTPRATT_H_ */
