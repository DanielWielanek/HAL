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

#include <TH1.h>
#include <TH2.h>
#include <TString.h>

/**
 * generate pair file by using distribution of partcles from rapidity-pt histogram
 */


class TFile;
class TTree;
class TClonesArray;
namespace Hal {
  class FemtoMicroPair;
  class CorrFitPairGeneratorYPt : public TObject {
    Int_t fBins;
    Int_t fNEvents;
    Int_t fPid1, fPid2;
    Int_t fMultiplicity;
    Int_t fCutOff;
    Float_t fMin;
    Float_t fMax;
    Float_t fM1, fM2;
    Float_t fKt[2];
    Float_t* fRanges;  //!
    TH2D fHist1, fHist2;
    TH1D fPairsControl;
    TFile* fFile;
    TTree* fTree;
    FemtoMicroPair* fPair;
    TClonesArray* fArray;
    TString fName;
    Bool_t GeneratePair();

  public:
    CorrFitPairGeneratorYPt();
    CorrFitPairGeneratorYPt& operator=(const CorrFitPairGeneratorYPt&) = delete;
    /**
     * set name of file with pairs
     * @param name
     */
    void SetPairFileName(TString name) { fName = name; };
    /**
     * set axis of "control histogram" this histogram is used to limit number of pairs with large k*
     * thanks to this we avoid calculation of "flat" part of the CF
     * @param bins
     * @param min
     * @param max
     */
    void SetAxis(Int_t bins, Float_t min, Float_t max);
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
     * set maximum number of events
     * @param pairs
     */
    void SetNEvents(Int_t pairs) { fNEvents = pairs; };
    /**
     * set PDG codes of simulated particles
     * @param pdg1
     * @param pdg2
     */
    void SetPdg(Int_t pdg1, Int_t pdg2) {
      fPid1 = pdg1;
      fPid2 = pdg2;
    };
    /**
     * set initial number of pairs per event
     * @param mult
     */
    void SetMultiplicity(Int_t mult) { fMultiplicity = mult; };
    /**
     * set maximum number of pairs per bin of "control histogram"
     * @param cutof
     */
    void SetCutOff(Int_t cutof) { fCutOff = cutof; };
    /**
     * initialize this code
     * @return
     */
    Bool_t Init();
    /**
     * make pairs
     */
    void Exec();
    virtual ~CorrFitPairGeneratorYPt();
    ClassDef(CorrFitPairGeneratorYPt, 1)
  };
}  // namespace Hal
#endif /* HALCORRFITPAIRGENERATORYPT_H_ */
