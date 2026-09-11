/*
 * CorrFitSmearingMatrixCalculator.h
 *
 *  Created on: 23 sty 2026
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFITSMEARINGMATH2DYIELD_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFITSMEARINGMATH2DYIELD_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH2.h>

#include "CorrFitSmearingMath1DCF.h"
#include "Object.h"


namespace Hal {
  /**
   * class for calculation smearing map
   * requires
   * - yields (pt-y or pt-eta)
   * - smearing algo
   */
  class CorrFitSmearingMath2DYield : public CorrFitSmearingMath1DCF {
    TH2D* fYield1 = {nullptr};
    TH2D* fYield2 = {nullptr};

    enum class EMode {
      kEtaPt,
      kYPt,
    };
    EMode fMode = {EMode::kYPt};

  protected:
    virtual void GenerateTracks(TLorentzVector*, Int_t type);
    virtual Bool_t Init();

  public:
    CorrFitSmearingMath2DYield(Int_t tracks = 5000);
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
     */
    void SetYield(TH2D& yield);
    /**
     * set yields for nonid
     * @param yield1
     * @param yield2
     */
    void SetYields(TH2D& yield1, TH2D& yield2);
    virtual ~CorrFitSmearingMath2DYield();
    ClassDef(CorrFitSmearingMath2DYield, 0)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFITSMEARINGMATH_H_ */
