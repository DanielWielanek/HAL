/*
 * CorrFitSmearingMatrixCalculator.h
 *
 *  Created on: 23 sty 2026
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFITSMEARINGMATRIXCALCULATOR_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFITSMEARINGMATRIXCALCULATOR_H_

#include <RtypesCore.h>
#include <TH2.h>
#include <TLorentzVector.h>
#include <array>

#include "Object.h"

namespace Hal {
  class FemtoPair;
} /* namespace Hal */

class TH2D;


namespace Hal {

  class CorrFitSmearingMatrixCalculator : public Object {
    TH2D* fYield1 = {nullptr};
    TH2D* fYield2 = {nullptr};
    TH2D* fOutput = {nullptr};
    std::vector<TH1D*> fResoP[2];
    std::vector<TH1D*> fResoPhi[2];
    std::vector<TH1D*> fResoTheta[2];
    TH2D** fResoP2d        = {nullptr};
    TH2D** fResoPhi2d      = {nullptr};
    TH2D** fResoTheta2d    = {nullptr};
    TLorentzVector* fPSim1 = {nullptr};
    TLorentzVector* fPSim2 = {nullptr};
    TLorentzVector* fPRec1 = {nullptr};
    TLorentzVector* fPRec2 = {nullptr};
    TF1* fFuncP[2]         = {nullptr, nullptr};
    TF1* fFuncPhi[2]       = {nullptr, nullptr};
    TF1* fFuncTheta[2]     = {nullptr, nullptr};
    const Int_t fNtracks;

    Int_t fPdg1        = {211};
    Int_t fPdg2        = {211};
    Int_t fBins        = {100};
    Bool_t fUseFunc    = {kFALSE};
    Bool_t fExpYields  = {kFALSE};
    Double_t fLow      = {0};
    Double_t fHigh     = {1};
    Double_t fM1       = {0};
    Double_t fM2       = {0};
    Double_t fKtCut[2] = {0, 1};
    enum class EMode {
      kEtaPt,
      kYPt,
    };
    EMode fMode = {EMode::kYPt};
    void GenerateTracks(TLorentzVector*, Int_t type);
    void Clean(Int_t type);


  protected:
    /**
     * check reconstructed pair parameters
     * @param pair
     * @return
     */
    virtual Bool_t CheckPair(Hal::FemtoPair* pair) const;
    /**
     * make smearing
     * @param sim_tracks
     * @param reco_tracks
     * @param type
     */
    virtual void MakeSmearTracksTH(TLorentzVector* smeared_tracks, TLorentzVector* unsmeared_tracks, Int_t type);
    /**
     * make smearing according to function
     * @param sim_tracks
     * @param reco_tracks
     * @param type
     */
    virtual void MakeSmearTracksTF(TLorentzVector* smeared_tracks, TLorentzVector* unsmeared_tracks, Int_t type);

  public:
    CorrFitSmearingMatrixCalculator(Int_t tracks = 5000);
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
     * set resolutions NOTE - on X axis there is p_reco, all histograms should have the same ranges and number of bins on X-axis
     * @param p p_reco vs dp (sim-reco)
     * @param phi phi_reco (sim-reco)
     * @param theta theta reco (sim-reco)
     * @param type 0 or 1 (for non-id)
     */
    void SetResolution(const TH2D& p, const TH2D& phi, const TH2D& theta, Int_t type);
    /**
     * set sigma of momentum ,phi ,theta as function of reconstructed momentum
     * @param p
     * @param phi
     * @param theta
     * @param type
     */
    void SetResolution(const TF1& p, const TF1& phi, const TF1& theta, Int_t type);
    /**
     * set smearing matrix histogram parameters
     * @param bins
     * @param lo
     * @param hi
     */
    void SetAxis(Int_t bins, Double_t lo, Double_t hi);
    /**
     * assume that yields contains y on x and pt on y
     */
    void SetModeYPt() { fMode = EMode::kYPt; };
    /**
     * assume that yields contains eta on x and pt on y
     */
    void SetModeEtaPt() { fMode = EMode::kEtaPt; }
    /**
     * set yields
     * @param yield
     * @param pdg code of firs track
     * @param type - 0 or 1
     */
    void SetYield(TH2D& yield, Int_t pdg, Int_t type);
    /**
     * do the calculations
     * @param nEvents
     */
    void Calculate(Int_t nEvents);
    /**
     * return smearing map
     * @return
     */
    TH2D* GetMap() const { return fOutput; }
    /**
     * assume that measured yields are reconstructed
     */
    void UseExpFiels() { fExpYields = kTRUE; }
    virtual ~CorrFitSmearingMatrixCalculator();
    ClassDef(CorrFitSmearingMatrixCalculator, 0)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFITSMEARINGMATRIXCALCULATOR_H_ */
