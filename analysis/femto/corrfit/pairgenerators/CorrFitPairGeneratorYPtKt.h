/*
 * CorrFitPairGeneratorYPtKt.h
 *
 *  Created on: 10 kwi 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCorrFitPairGeneratorYPtKt_H_
#define HALCorrFitPairGeneratorYPtKt_H_

#include "CorrFitPairGenerator.h"

#include "FemtoMiniPair.h"

#include <utility>

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH2.h>

/**
 * generate pair file by using distribution of partcles from rapidity-pt histogram
 */


class TFile;
class TTree;
class TClonesArray;
namespace Hal {
  class FemtoMicroPair;
  class CorrFitPairGeneratorYPtKt : public CorrFitPairGenerator {
  protected:
    Float_t f2Kt2[2];
    TH2D fHist1, fHist2;
    FemtoMicroPair fPair;
    struct fourVect {
      int t, x, y, z;
    };
    void GenerateFreeRandomPair();
    Int_t GeneratePairOneDim();
    fourVect GeneratePairThreeDim();
    virtual void GenerateEvent();

  public:
    CorrFitPairGeneratorYPtKt();
    CorrFitPairGeneratorYPtKt& operator=(const CorrFitPairGeneratorYPtKt&) = delete;

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
    virtual ~CorrFitPairGeneratorYPtKt();
    ClassDef(CorrFitPairGeneratorYPtKt, 1)
  };
}  // namespace Hal
#endif /* HALCorrFitPairGeneratorYPtKt_H_ */
