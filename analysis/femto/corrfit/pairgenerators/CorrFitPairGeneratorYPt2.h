/*
 * CorrFitPairGeneratorYPt2.h
 *
 *  Created on: 28 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORYPT2_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORYPT2_H_
#include "CorrFitPairGenerator.h"

#include "FemtoMiniPair.h"

#include <utility>

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH2.h>
/**
 * similar to the CorrFitPairGeneratorYPt but generates pair in different way (faster?)
 */
class TFile;
class TTree;
class TClonesArray;
namespace Hal {
  class FemtoMicroPair;
  class CorrFitPairGeneratorYPt2 : public CorrFitPairGenerator {
    Float_t f2Kt[2];
    TH2D fHist1, fHist2;
    TH2D fConvolution;
    FemtoMicroPair fPair;
    Int_t GeneratePair(Double_t kstar);
    virtual void GenerateEvent();

  public:
    CorrFitPairGeneratorYPt2();
    CorrFitPairGeneratorYPt2& operator=(const CorrFitPairGeneratorYPt2&) = delete;

    /**
     * set kt range for simulated pairs
     * @param min
     * @param max
     */
    void SetKt(Double_t min, Double_t max) {
      f2Kt[0] = min * min * 4.0;
      f2Kt[1] = max * max * 4.0;
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
    virtual ~CorrFitPairGeneratorYPt2();
    ClassDef(CorrFitPairGeneratorYPt2, 1)
  };
} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORYPT2_H_ */
