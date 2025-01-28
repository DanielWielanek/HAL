/*
 * FemtoWeightGeneratorDummy.h
 *
 *  Created on: 8 lip 2024
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_WEIGHTS_FEMTOWEIGHTGENERATORDUMMY_H_
#define HAL_ANALYSIS_FEMTO_WEIGHTS_FEMTOWEIGHTGENERATORDUMMY_H_


#include "FemtoWeightGenerator.h"
namespace Hal {

  class FemtoWeightGeneratorK0K0 : public FemtoWeightGenerator {
    Double_t fMf0           = {0};
    Double_t fGamma_f0KK    = {0};
    Double_t fMa0           = {0};
    Double_t fGamma_f0pipi  = {0};
    Double_t fGamma_a0pieta = {0};
    Double_t fGamma_a0KK    = {0};
    Double_t fAlpha         = {0};

  public:
    FemtoWeightGeneratorK0K0();
    virtual FemtoWeightGenerator* MakeCopy() const { return new FemtoWeightGeneratorK0K0(*this); };
    virtual Double_t GenerateWeight(FemtoPair* pair);
    void SetDefParams(Int_t opt);

    Double_t GetAlpha() const { return fAlpha; }
    Double_t GetMf0() const { return fMf0; }
    Double_t GetGammaA0Kk() const { return fGamma_a0KK; }
    Double_t GetGammaA0pieta() const { return fGamma_a0pieta; }
    Double_t GetGammaF0KK() const { return fGamma_f0KK; }
    Double_t GetGammaF0pipi() const { return fGamma_f0pipi; }
    Double_t GetMa0() const { return fMa0; }

    void SetAsymmetry(Double_t Assymetry) { this->fAlpha = 0.5 * (1. - Assymetry * Assymetry); }
    void SetAlpha(Double_t alpha) { fAlpha = alpha; }
    void SetMf0(Double_t f0) { this->fMf0 = f0; }
    void SetGammaA0Kk(Double_t GammaA0Kk) { fGamma_a0KK = GammaA0Kk; }
    void SetGammaA0pieta(Double_t GammaA0pieta) { fGamma_a0pieta = GammaA0pieta; }
    void SetGammaF0KK(Double_t GammaF0Kk) { fGamma_f0KK = GammaF0Kk; }
    void SetGammaF0pipi(Double_t GammaF0pipi) { fGamma_f0pipi = GammaF0pipi; }
    void SetMa0(Double_t Ma0) { this->fMa0 = Ma0; }
    virtual ~FemtoWeightGeneratorK0K0() {};
    ClassDef(FemtoWeightGeneratorK0K0, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_WEIGHTS_FEMTOWEIGHTGENERATORDUMMY_H_ */
