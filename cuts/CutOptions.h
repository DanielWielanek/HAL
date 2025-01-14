/*
 * CutOptions.h
 *
 *  Created on: 14 sty 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_CUTOPTIONS_H_
#define HAL_CUTS_CUTOPTIONS_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>
#include <vector>

#include "Object.h"

namespace Hal {
  class Cut;
  class CutMonitor;

  class CutOptions : public Object {
    std::vector<Int_t> fCollections;
    Bool_t fRe           = {kFALSE};
    Bool_t fIm           = {kFALSE};
    Bool_t fSig          = {kFALSE};
    Bool_t fBckg         = {kFALSE};
    Bool_t fFast         = {kFALSE};
    Bool_t fAcceptNull   = {kFALSE};
    Bool_t fAcceptDouble = {kFALSE};
    Int_t fDefCol        = {-1};

  public:
    CutOptions(TString opt = "", Int_t defCol = -1);
    void ClearFlag(TString flag);
    Bool_t Re() const { return fRe; }
    Bool_t Im() const { return fIm; }
    Bool_t Sig() const { return fSig; }
    Bool_t Bckg() const { return fBckg; }
    Bool_t Fast() const { return fFast; }
    Bool_t KeepDouble() const { return fAcceptDouble; }
    std::vector<Int_t> GetCollectionsIds() const { return fCollections; };
    Hal::Cut* MakeCutCopy(const Hal::Cut& x) const;
    Hal::CutMonitor* MakeMonitorCopy(const Hal::CutMonitor& x) const;
    virtual ~CutOptions() {};
    ClassDef(CutOptions, 1)
  };


} /* namespace Hal */

#endif /* HAL_CUTS_CUTOPTIONS_H_ */
