/*
 * CorrFitPairGeneratorConvolutionYPt.h
 *
 *  Created on: 20 sty 2025
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORCONVOLUTIONYPT_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORCONVOLUTIONYPT_H_

#include <RtypesCore.h>
#include <TH2.h>
#include <TH3.h>

#include "ConvolutionPairGenerator.h"
#include "CorrFitPairGenerator.h"
#include "FemtoMiniPair.h"

class TFile;
class TTree;
class TClonesArray;
namespace Hal {
  class FemtoMicroPair;

  class CorrFitPairGeneratorConvolutionYPt : public CorrFitPairGenerator {
  protected:
    Int_t fPairsPerBin    = {100};
    Int_t fOneDimBin      = {0};
    Int_t fModuloX        = {0};
    Int_t fModuloY        = {0};
    Int_t fModuloZ        = {0};
    Int_t fBinCFZ         = {0};
    Int_t fBinSmearFactor = {0};
    ConvolutionPairGenerator fConvolution;
    FemtoMicroPair fPair;
    std::vector<double> fOut, fSide, fLong;
    Double_t fX = {0}, fY = {0}, fZ = {0};

    virtual void GeneratePairEvent();
    void SwapSignRandom(Double_t& x, Double_t& y, Double_t& z) const;
    virtual void GenerateEvent();

  public:
    CorrFitPairGeneratorConvolutionYPt();
    CorrFitPairGeneratorConvolutionYPt& operator=(const CorrFitPairGeneratorConvolutionYPt&) = delete;
    /**
     * set kt range for simulated pairs
     * @param min
     * @param max
     */
    void SetKt(Double_t min, Double_t max) { fConvolution.SetKt(min, max); };
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
    /**
     * smear pairs uniformly in bin range
     * @param factor
     */
    void SmearUniformly(Int_t factor) { fBinSmearFactor = factor; };
    void SetPairsPerBin(Int_t nPair) { fPairsPerBin = nPair; }
    /**
     * fill only fraction of map, usefull to calculate stuff on cluster
     * @param entries
     * @param nSamples - number of jobs
     * @param nNo - job number
     */
    virtual void RunFraction(Int_t entries, Int_t nSamples, Int_t nNo);
    virtual ~CorrFitPairGeneratorConvolutionYPt() {};
    ClassDef(CorrFitPairGeneratorConvolutionYPt, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITPAIRGENERATORCONVOLUTIONYPT_H_ */
