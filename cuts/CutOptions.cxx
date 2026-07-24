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
    if (defCol >= -1) { SETBIT(fFlag, fgStartColSet); }
    if (defCol == -2) defCol = 0;  // not set, set to zero
    fDefCol = defCol;
    if (opt.Length() != 0) {
      TString option = opt;
      TRegexp regexp("\\[([^\\]]+)\\]");
      fExtra = option(regexp);
      if (fExtra.Length()) { opt.ReplaceAll(Form("[%s]", fExtra.Data()), ""); }
      if (Hal::Std::FindParam(opt, "re", kFALSE)) SETBIT(fFlag, fgReFlagId);
      if (Hal::Std::FindParam(opt, "im", kFALSE)) SETBIT(fFlag, fgImFlagId);
      if (!Hal::Std::FindParam(opt, "sig", kFALSE) && !Hal::Std::FindParam(opt, "bckg", kFALSE)) {
        SETBIT(fFlag, fgSigFlagId);
        SETBIT(fFlag, fgBckgFlagId);
      }
      if (Hal::Std::FindParam(opt, "null", kFALSE)) SETBIT(fFlag, fgAccNullFlagId);
      if (Hal::Std::FindParam(opt, "double", kFALSE)) SETBIT(fFlag, fgAccDoubleFlagId);
      if (Hal::Std::FindParam(opt, "sig", kFALSE)) SETBIT(fFlag, fgSigFlagId);
      if (Hal::Std::FindParam(opt, "bckg", kFALSE)) SETBIT(fFlag, fgBckgFlagId);
      if (Hal::Std::FindParam(opt, "fast", kFALSE)) SETBIT(fFlag, fgFastFlagId);
      if (Hal::Std::FindParam(opt, "both", kFALSE)) {
        SETBIT(fFlag, fgSigFlagId);
        SETBIT(fFlag, fgBckgFlagId);
      }
      fCollections = GetCollectionsFlags(fDefCol, opt);
    } else {
      SetSig(true);
      SetBckg(true);
    }
    if (fCollections.size() == 0) fCollections.push_back(fDefCol);  // collections where not overwriten
  }

  void CutOptions::ClearFlag(TString flag) {
    if (Hal::Std::FindParam(flag, "re", kFALSE)) CLRBIT(fFlag, fgReFlagId);
    if (Hal::Std::FindParam(flag, "im", kFALSE)) CLRBIT(fFlag, fgImFlagId);
    if (Hal::Std::FindParam(flag, "sig", kFALSE)) CLRBIT(fFlag, fgSigFlagId);
    if (Hal::Std::FindParam(flag, "bckg", kFALSE)) CLRBIT(fFlag, fgBckgFlagId);
    if (Hal::Std::FindParam(flag, "fast", kFALSE)) CLRBIT(fFlag, fgFastFlagId);
  }

  Hal::Cut* CutOptions::MakeCutCopy(const Hal::Cut& x) const {
    CutOptions opt;
    if (IsRe()) {
      opt.SetRe(IsRe());
      opt.SetNull(false);
    } else if (IsIm()) {
      opt.SetIm(IsIm());
      opt.SetNull(IsNull());
    }
    auto copy = x.MakeCopy(opt);
    copy->SetCollectionID(x.GetCollectionID());
    return copy;
  }

  Hal::CutMonitor* CutOptions::MakeMonitorCopy(const Hal::CutMonitor& x) const {
    CutOptions opts;
    opts.SetRe(IsRe());
    opts.SetIm(IsIm());
    Hal::CutMonitor* res = x.MakeCopy(opts);
    return res;
  }
  std::vector<Int_t> CutOptions::GetCollectionsFlags(Int_t startCol, TString option) const {
    std::vector<Int_t> res;
    Int_t single      = -2;
    Bool_t single_exp = FindExpressionSingleValue(option, single, kTRUE);
    Int_t n, jump;
    Bool_t two_exp = FindExpressionTwoValues(option, n, jump, kTRUE);
    if (single_exp && two_exp) {  // found {}+{x}
      for (int i = 0; i < n; i++) {
        res.push_back(single);
        single += jump;
      }
      return res;
    }
    if (two_exp) {  //{x}
      single = TMath::Max(0, startCol);
      for (int i = 0; i < n; i++) {
        res.push_back(single);
        single += jump;
      }
      return res;
    }
    if (single_exp) res.push_back(single);
    while (FindExpressionSingleValue(option, single, kTRUE)) {
      res.push_back(single);
    }
    if (res.size() == 0) { res.push_back(startCol); }
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

  Bool_t CutOptions::FindExpressionSingleValue(TString& expression, Int_t& val, Bool_t remove) const {
    TString option = expression;
    TRegexp regexp("{[0-9]+}");
    TString expr = option(regexp);
    if (expr.Length() <= 0) { return kFALSE; }
    // found regular exprestion like {number}
    option.Remove(option.Index(regexp),
                  expr.Length());  // remove exprestion from string
    TRegexp number_expr("{[0-9]+}");
    TString first      = expr(number_expr);
    TString number_str = first(TRegexp("[0-9]+"));
    val                = number_str.Atoi();
    if (remove) { expression = option; }
    return kTRUE;
  }


  Bool_t CutOptions::FindExpressionTwoValues(TString& expression, Int_t& val1, Int_t& val2, Bool_t remove) const {
    TString option = expression;
    TRegexp regexp("{[0-9]+x[0-9]+}");
    TString expr = option(regexp);
    if (expr.Length() <= 0) { return kFALSE; }
    // found regular exprestion like {number x number}
    option.Remove(option.Index(regexp),
                  expr.Length());  // remove exprestion from string
    TRegexp number_expr("{[0-9]+x");
    TRegexp jump_expr("x[0-9]+}");
    TString first      = expr(number_expr);
    TString sec        = expr(jump_expr);
    TString number_str = first(TRegexp("[0-9]+"));
    TString jump_str   = sec(TRegexp("[0-9]+"));
    val1               = number_str.Atoi();
    val2               = jump_str.Atoi();
    if (remove) { expression = option; }
    return kTRUE;
  }

} /* namespace Hal */
