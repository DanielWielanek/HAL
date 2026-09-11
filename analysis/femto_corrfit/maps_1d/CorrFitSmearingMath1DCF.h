/*
 * CorrFitSmearingMath1DCF.h
 *
 *  Created on: 11 wrz 2026
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFITSMEARINGMATH1DCF_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFITSMEARINGMATH1DCF_H_

#include <RtypesCore.h>
#include <TObject.h>
class TLorentzVector;
namespace Hal {
  class LorentzSmearing;
  class FemtoPair;
} /* namespace Hal */
class TH2D;
namespace Hal {

  class CorrFitSmearingMath1DCF : public TObject {
  protected:
    const Int_t fNtracks;
    Bool_t fOpenMP              = {kFALSE};
    Bool_t fId                  = {kTRUE};
    Int_t fPdg1                 = {211};
    Int_t fPdg2                 = {211};
    Int_t fBins                 = {100};
    Bool_t fExpYields           = {kFALSE};
    Double_t fLow               = {0};
    Double_t fHigh              = {1};
    Double_t fKtCut[2]          = {0, 1};
    LorentzSmearing* fSmearing1 = {nullptr};
    LorentzSmearing* fSmearing2 = {nullptr};
    TH2D* fOutput               = {nullptr};
    struct LorentzVectors {
      TLorentzVector *tracks_raw1 = {nullptr}, *tracks_raw2 = {nullptr};
      TLorentzVector *tracks_sme1 = {nullptr}, *tracks_sme2 = {nullptr};
      TLorentzVector *PSim1 = {nullptr}, *PSim2 = {nullptr};
      TLorentzVector *PRec1 = {nullptr}, *PRec2 = {nullptr};
      void Init(bool expFields, int tracksNo);
      void Destroy();
    };
    virtual Bool_t CheckPair(Hal::FemtoPair* pair) const;
    virtual void DoMathSingleCore(Int_t events, LorentzVectors data);
    virtual void DoMathMT(Int_t events, LorentzVectors data);
    virtual Bool_t Init();
    virtual void GenerateTracks(TLorentzVector*, Int_t type) = 0;

  public:
    CorrFitSmearingMath1DCF(Int_t tracks = 5000);
    /**
     * set kT cut
     * @param lo
     * @param hi
     */
    void SetKtCut(Double_t lo, Double_t hi) {
      fKtCut[0] = lo;
      fKtCut[1] = hi;
    }
    /**
     * do calculatsions
     * @param nEvents
     */
    virtual void Calculate(Int_t nEvents);
    /**
     * sets smear algo
     * @param smear
     * @param type
     */
    void SetSmearingAlgo(const Hal::LorentzSmearing& smear);
    /**
     * set smear algo for nonidentical
     * @param smear1
     * @param smear2
     */
    void SetSmearingAlgo(const Hal::LorentzSmearing& smear1, const Hal::LorentzSmearing& smear2);
    /**
     * set smearing matrix histogram parameters
     * @param bins
     * @param lo
     * @param hi
     */
    void SetAxis(Int_t bins, Double_t lo, Double_t hi);
    /**
     * return smearing map
     * @return
     */
    TH2D* GetMap() const { return fOutput; }
    /**
     * assume that measured yields are reconstructed
     */
    void UseExpFiels() { fExpYields = kTRUE; }
    /**
     * use threads
     */
    void UseMT() { fOpenMP = true; }
    virtual ~CorrFitSmearingMath1DCF();
    ClassDef(CorrFitSmearingMath1DCF, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFITSMEARINGMATH1DCF_H_ */
