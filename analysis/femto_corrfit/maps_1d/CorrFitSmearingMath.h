/*
 * CorrFitSmearingMatrixCalculator.h
 *
 *  Created on: 23 sty 2026
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFITSMEARINGMATH_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFITSMEARINGMATH_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH2.h>

#include "Object.h"

class TLorentzVector;
namespace Hal {
  class LorentzSmearing;
} /* namespace Hal */

namespace Hal {
  class FemtoPair;
} /* namespace Hal */

class TH2D;


namespace Hal {
  /**
   * class for calculation smearing map
   * requires
   * - yields (pt-y or pt-eta)
   * - smearing algo
   */
  class CorrFitSmearingMath : public Object {
    TH2D* fYield1 = {nullptr};
    TH2D* fYield2 = {nullptr};
    TH2D* fOutput = {nullptr};
    const Int_t fNtracks;

    Int_t fPdg1                 = {211};
    Int_t fPdg2                 = {211};
    Int_t fBins                 = {100};
    Bool_t fExpYields           = {kFALSE};
    Double_t fLow               = {0};
    Double_t fHigh              = {1};
    Double_t fKtCut[2]          = {0, 1};
    LorentzSmearing* fSmearing1 = {nullptr};
    LorentzSmearing* fSmearing2 = {nullptr};
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

  public:
    CorrFitSmearingMath(Int_t tracks = 5000);
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
     * sets smear algo
     * @param smear
     * @param type
     */
    void SetSmearingAlgo(const Hal::LorentzSmearing& smear, Int_t type);
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
     * @param type - 0 or 1
     */
    void SetYield(TH2D& yield, Int_t type);
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
    virtual ~CorrFitSmearingMath();
    ClassDef(CorrFitSmearingMath, 0)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFITSMEARINGMATH_H_ */
