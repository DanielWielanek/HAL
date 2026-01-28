/*
 * FemtoKernelMaker.h
 *
 *  Created on: 19 lip 2024
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_ROCO_FEMTOKERNELMAKER_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_ROCO_FEMTOKERNELMAKER_H_

#include <RtypesCore.h>
#include <TObject.h>
#include <TString.h>

#include "FemtoConst.h"


namespace Hal {
  class FastHist3D;
} /* namespace Hal */

namespace Hal {
  class FemtoPseudoKernel : public TObject {
    TString fWeightClassName   = {""};
    Femto::EPairType fPairType = Femto::EPairType::kUnknown;
    FastHist3D* fHist3d        = {nullptr};

  public:
    FemtoPseudoKernel() {};
    Femto::EPairType GetPairType() const { return fPairType; }
    FemtoPseudoKernel(Int_t kstarbins,
                      Double_t kstarLow,
                      Double_t kStarHigh,
                      Int_t rstarbins,
                      Double_t rLow,
                      Double_t rHigh,
                      Int_t thetabins);
    void SetWeightInfo(TString className, Femto::EPairType type);
    Double_t GetWeight(Double_t kstar, Double_t rstar, Double_t theta) const;
    void SetWeight(Double_t kstar, Double_t rstar, Double_t theta, Double_t w);
    const FastHist3D* GetHist() const { return fHist3d; }
    virtual ~FemtoPseudoKernel() {
      if (fHist3d) delete fHist3d;
    };
  };

  class FemtoPseudoKernelMaker : public Object {
    FemtoPair* fPair              = {nullptr};
    FemtoWeightGenerator* fWeight = {nullptr};
    FemtoPseudoKernel* fKernel    = {nullptr};
    Int_t fPid1                   = {0};
    Int_t fPid2                   = {0};
    Double_t fM1_2                = {0};
    Double_t fM2_2                = {0};
    void GeneratePair(Double_t k, Double_t r, Double_t theta);

  public:
    FemtoPseudoKernelMaker(Int_t kstarbins    = 100,
                           Double_t kstarLow  = 0,
                           Double_t kStarHigh = 1,
                           Int_t rstarbins    = 100,
                           Double_t rLow      = 0,
                           Double_t rHigh     = 20,
                           Int_t thetabins    = 100);
    void SetWeightGenerator(FemtoWeightGenerator& wCalc);
    void SetPairType(Int_t pid1, Int_t pid2) {
      fPid1 = pid1;
      fPid2 = pid2;
    };
    void Run(Int_t nSamples);
    void Save(TString file);
    virtual ~FemtoPseudoKernelMaker();
  };

}  // namespace Hal
#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_ROCO_FEMTOKERNELMAKER_H_ */
