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


#include <Rtypes.h>
#include <RtypesCore.h>
#include <utility>

#include "FemtoConst.h"
#include "Object.h"


namespace Hal {

  class CorrFitMapGroupConfig : public Object {
    const Int_t kBitDataMode         = {0};
    const Int_t kBitBranchModeSignal = {1};
    const Int_t kBitBranchModeBckg   = {2};
    UInt_t fMode                     = {0};
    Int_t fBins;
    Int_t fBranchMode = {0};
    Double_t fMin, fMax, fStep;
    Femto::EKinematics fFrame = {Femto::EKinematics::kLCMS};

  public:
    CorrFitMapGroupConfig();
    void SetGroupByKLong() { SETBIT(fMode, kBitDataMode); };
    void SetGroupByKStar() { CLRBIT(fMode, kBitDataMode); };
    void SetAxis(Int_t bins, Double_t min, Double_t max);
    Femto::EKinematics GetFrame() const { return fFrame; }
    void SetFrame(Femto::EKinematics frame) { fFrame = frame; }
    Int_t GetBin(const FemtoPair* pair) const;
    Int_t GetNbins() const { return fBins; }
    inline Bool_t GroupByLong() const { return TESTBIT(fMode, kBitDataMode); };
    inline Bool_t GroupByKStar() const { return !GroupByLong(); };
    Double_t GetMin() const { return fMin; };
    Double_t GetMax() const { return fMax; };
    void EnableSignal() { SETBIT(fMode, kBitBranchModeSignal); };
    void EnableBackground() { SETBIT(fMode, kBitBranchModeBckg); };
    Bool_t HaveSignal() const { return TESTBIT(fMode, kBitBranchModeSignal); };
    Bool_t HaveBackground() const { return TESTBIT(fMode, kBitBranchModeSignal); };
    virtual void Add(const Object* pack);
    std::pair<Int_t, Int_t> GetBranchesByValue(Double_t min, Double_t max) const;
    virtual ~CorrFitMapGroupConfig() {};
    ClassDef(CorrFitMapGroupConfig, 2)
  };
} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_CORRFIT_PAIRGENERATORS_CORRFITMAPGROUPCONFIG2_H_ */
