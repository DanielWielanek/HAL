/*
 * CutOptions.cxx
 *
 *  Created on: 14 sty 2025
 *      Author: daniel
 */

#include "CutOptions.h"

#include "Cout.h"
#include "CutMonitor.h"
#include "EventBinningCut.h"
#include "Std.h"
#include "StdString.h"

#include "CutMonitor.h"
#include "EventComplexCut.h"
#include "EventCut.h"
#include "TrackComplexCut.h"
#include "TrackCut.h"
#include "TwoTrackComplexCut.h"
#include "TwoTrackCut.h"

#include <TClass.h>
#include <TRegexp.h>

namespace Hal {
  CutOptions::CutOptions(TString opt, Int_t defCol) {
    if (Hal::Std::FindParam(opt, "re", kFALSE)) fRe = kTRUE;
    if (Hal::Std::FindParam(opt, "im", kFALSE)) fIm = kTRUE;
    if (!Hal::Std::FindParam(opt, "sig", kFALSE) && !Hal::Std::FindParam(opt, "bckg", kFALSE)) { fSig = fBckg = kTRUE; }
    if (Hal::Std::FindParam(opt, "null", kFALSE)) fAcceptNull = kTRUE;
    if (Hal::Std::FindParam(opt, "double", kFALSE)) fAcceptDouble = kTRUE;
    if (Hal::Std::FindParam(opt, "sig", kFALSE)) fSig = kTRUE;
    if (Hal::Std::FindParam(opt, "bckg", kFALSE)) fBckg = kTRUE;
    if (Hal::Std::FindParam(opt, "fast", kFALSE)) fFast = kTRUE;
    if (Hal::Std::FindParam(opt, "both", kFALSE)) {
      fSig  = kTRUE;
      fBckg = kTRUE;
    }
    fDefCol      = defCol;
    fCollections = GetCollectionsFlags(fDefCol, opt);
    if (fCollections.size() == 0) fCollections.push_back(fDefCol);  // collections where not overwriten
  }

  void CutOptions::ClearFlag(TString flag) {
    if (Hal::Std::FindParam(flag, "re", kFALSE)) fRe = kFALSE;
    if (Hal::Std::FindParam(flag, "im", kFALSE)) fIm = kFALSE;
    if (Hal::Std::FindParam(flag, "sig", kFALSE)) fSig = kFALSE;
    if (Hal::Std::FindParam(flag, "bckg", kFALSE)) fBckg = kFALSE;
    if (Hal::Std::FindParam(flag, "fast", kFALSE)) fFast = kFALSE;
  }

  Hal::Cut* CutOptions::MakeCutCopy(const Hal::Cut& x) const {
    if (fRe) {
      return MakeCutCopy(x, "re", kFALSE);
    } else if (fIm) {
      return MakeCutCopy(x, "im", fAcceptNull);
    }
    return x.MakeCopy();
  }

  Hal::CutMonitor* CutOptions::MakeMonitorCopy(const Hal::CutMonitor& x) const {
    TString innerOpt = "";
    if (fRe)
      innerOpt = "re";
    else if (fIm)
      innerOpt = "im";
    Hal::CutMonitor* res = x.MakeCopy(innerOpt);
    return res;
  }
  std::vector<Int_t> CutOptions::GetCollectionsFlags(Int_t startCol, TString option) const {
    std::vector<Int_t> res;
    Int_t single      = -2;
    Bool_t single_exp = Hal::Std::FindExpressionSingleValue(option, single, kTRUE);
    Int_t n, jump;
    Bool_t two_exp = Hal::Std::FindExpressionTwoValues(option, n, jump, kTRUE);
    if (single_exp && two_exp) {  // found {}+{x}
      for (int i = 0; i < n; i++) {
        res.push_back(single);
        single += jump;
      }
      return res;
    }
    if (two_exp) {  //{x}
      single = startCol;
      for (int i = 0; i < n; i++) {
        res.push_back(single);
        single += jump;
      }
      return res;
    }
    if (single_exp) res.push_back(single);
    while (Hal::Std::FindExpressionSingleValue(option, single, kTRUE)) {
      res.push_back(single);
    }
    if (res.size() == 0) res.push_back(startCol);
    return res;
  }

  Hal::Cut* CutOptions::MakeCutCopy(const Hal::Cut& cut, TString flag, Bool_t acceptNulls) const {
    TString opt = flag;
    if (acceptNulls) opt = opt + "+null";
    Hal::Cut* res = cut.MakeCopy(opt);
    if (res) res->SetCollectionID(cut.GetCollectionID());
    return res;
  }
  TString CutOptions::GetCutUpdateRatioName(ECutUpdate upd) const {
    TString update_ratio_name;
    switch (upd) {
      case ECutUpdate::kEvent: update_ratio_name = "event"; break;
      case ECutUpdate::kTrack: update_ratio_name = "track"; break;
      case ECutUpdate::kTwoTrack: update_ratio_name = "two_track"; break;
      case ECutUpdate::kTwoTrackBackground: update_ratio_name = "two_track<background>"; break;
      default:
        Cout::PrintInfo("Unknown update ratio", EInfo::kLowWarning);
        update_ratio_name = "unknown";
        break;
    }
    return update_ratio_name;
  }

} /* namespace Hal */
