/*
 * CorrFitSmearingMath3DYield.h
 *
 *  Created on: 11 wrz 2026
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFITSMEARINGMATH3DYIELD_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFITSMEARINGMATH3DYIELD_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH2.h>

#include "CorrFitSmearingMath1DCF.h"
#include "Object.h"

class TH3D;
namespace Hal {

  class CorrFitSmearingMath3DYield : public CorrFitSmearingMath1DCF {
    TH3D* fYield1 = {nullptr};
    TH3D* fYield2 = {nullptr};

    enum class EMode {
      kEtaPtPhi,
      kYPtPhi,
    };
    EMode fMode = {EMode::kYPtPhi};

  protected:
    virtual void GenerateTracks(TLorentzVector*, Int_t type);
    virtual Bool_t Init();

  public:
    CorrFitSmearingMath3DYield(Int_t ntracks = 500);
    /**
     * assume that yields contains y on x and pt on y and phi on z
     */
    void SetModeYPtPhi() { fMode = EMode::kYPtPhi; };
    /**
     * assume that yields contains eta on x and pt on y and phi on z
     */
    void SetModeEtaPtPhi() { fMode = EMode::kEtaPtPhi; }
    /**
     * set yields
     * @param yield
     */
    void SetYield(TH3D& yield);
    /**
     * set yields for nonid
     * @param yield1
     * @param yield2
     */
    void SetYields(TH3D& yield1, TH3D& yield2);
    virtual ~CorrFitSmearingMath3DYield();
    ClassDef(CorrFitSmearingMath3DYield, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_MAPS_1D_CORRFITSMEARINGMATH3DYIELD_H_ */
