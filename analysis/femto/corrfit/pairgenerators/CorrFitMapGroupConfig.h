/*
 * CorrFitMapGroupConfig2.h
 *
 *  Created on: 27 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITMAPGROUPCONFIG_H_
#define HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITMAPGROUPCONFIG_H_


#include "FemtoMiniPair.h"

#include "Object.h"

namespace Hal {

  class CorrFitMapGroupConfig : public Object {
    Int_t fMode = {0};
    Int_t fBins;
    Double_t fMin, fMax, fStep;
    Femto::EKinematics fFrame = {Femto::EKinematics::kLCMS};

  public:
    CorrFitMapGroupConfig();
    void SetGroupByKLong() { fMode = 1; };
    void SetGroupByKStar() { fMode = 0; };
    void SetAxis(Int_t bins, Double_t min, Double_t max);
    Femto::EKinematics GetFrame() const { return fFrame; }
    void SetFrame(Femto::EKinematics frame) { fFrame = frame; }
    Int_t GetBin(const FemtoPair* pair) const;
    Int_t GetNbins() const { return fBins; }
    Bool_t GroupByLong() const;
    Bool_t GroupByKStar() const;
    Double_t GetMin() const { return fMin; };
    Double_t GetMax() const { return fMax; };
    virtual void Add(const Object* pack);
    std::vector<TString> GetBranchesByValue(Double_t min, Double_t max, Bool_t signal = kTRUE) const;
    std::vector<TString> GetBranchesByIndex(Int_t min, Int_t max, Bool_t signal = kTRUE) const;
    virtual ~CorrFitMapGroupConfig() {};
    ClassDef(CorrFitMapGroupConfig, 1)
  };
} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITMAPGROUPCONFIG2_H_ */
