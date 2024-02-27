/*
 * CorrFitPairGeneratorYPt.h
 *
 *  Created on: 10 kwi 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCORRFITPAIRGENERATORYPT_H_
#define HALCORRFITPAIRGENERATORYPT_H_

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
  class CorrFitPairGeneratorYPt : public CorrFitPairGenerator {
    Float_t fKt[2];
    TH2D fHist1, fHist2;
    FemtoMicroPair fPair;
    Int_t GeneratePair();
    virtual void GenerateEvent();

  public:
    CorrFitPairGeneratorYPt();
    CorrFitPairGeneratorYPt& operator=(const CorrFitPairGeneratorYPt&) = delete;

    /**
     * set kt range for simulated pairs
     * @param min
     * @param max
     */
    void SetKt(Double_t min, Double_t max) {
      fKt[0] = min * min * 4.0;
      fKt[1] = max * max * 4.0;
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
    virtual ~CorrFitPairGeneratorYPt();
    ClassDef(CorrFitPairGeneratorYPt, 1)
  };
}  // namespace Hal
#endif /* HALCORRFITPAIRGENERATORYPT_H_ */
