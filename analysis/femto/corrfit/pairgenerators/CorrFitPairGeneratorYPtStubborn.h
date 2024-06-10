/*
 * CorrFitPairGeneratorYPtSubborn.h
 *
 *  Created on: 29 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORYPTSUBBORN_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORYPTSUBBORN_H_

#include "CorrFitPairGenerator.h"

#include "FemtoMiniPair.h"

#include <utility>

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH2.h>


class TFile;
class TTree;
class TClonesArray;
namespace Hal {
  class FemtoMicroPair;
  class CorrFitPairGeneratorYPtStubborn : public CorrFitPairGenerator {
    Float_t f2Kt2[2];
    TH2D fHist1, fHist2;
    FemtoMicroPair fPair;
    Bool_t GeneratePair(Int_t kstar);
    virtual void GenerateEvent();

  public:
    CorrFitPairGeneratorYPtStubborn();
    CorrFitPairGeneratorYPtStubborn& operator=(const CorrFitPairGeneratorYPtStubborn&) = delete;

    /**
     * set kt range for simulated pairs
     * @param min
     * @param max
     */
    void SetKt(Double_t min, Double_t max) {
      f2Kt2[0] = min * min * 4.0;
      f2Kt2[1] = max * max * 4.0;
    };
    /**
     * histograms with pt-rapidity distribution, rapidity is on X-axis pt is on Y-axis
     * @param hist1 pt-y distribution for 1st particle
     * @param hist2 pt-y distribution for 2nd particle
     */
    void SetHist(const TH2D& hist1, const TH2D& hist2);
    /**
     * initialize this code
     * @return
     */
    Bool_t Init();
    virtual ~CorrFitPairGeneratorYPtStubborn();
    ClassDef(CorrFitPairGeneratorYPtStubborn, 1)
  };
}  // namespace Hal


#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORYPTSUBBORN_H_ */
