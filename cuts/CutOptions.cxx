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
      if (dynamic_cast<const Hal::EventBinningCut*>(&x)) {
        Hal::Cout::PrintInfo(Form("%s %i: cannot add binned cut with im flag", __FILE__, __LINE__), EInfo::kError);
        return nullptr;
      }
      return MakeCutCopy(x, "re", kFALSE);
    } else if (fIm) {
      if (dynamic_cast<const Hal::EventBinningCut*>(&x)) {
        Hal::Cout::PrintInfo(Form("%s %i: cannot add binned cut with im flag", __FILE__, __LINE__), EInfo::kError);
        return nullptr;
      }
      return MakeCutCopy(x, "im", fAcceptNull);
    }
    return x.MakeCopy();
  }

  Hal::CutMonitor* CutOptions::MakeMonitorCopy(const Hal::CutMonitor& x) const {
    auto MakeComplexAxis = [](CutMonitor* moni, const Int_t axis, const Int_t flag) {
      if (flag == 0) return;
      TString cut_name = moni->GetCutName(axis);
      TClass* clas     = TClass::GetClass(cut_name, kTRUE, kTRUE);
      if (!clas) {
        Hal::Cout::PrintInfo(
          Form("Cannot find %s class for monitoring, probably you mixed options of adding/creating cut monitor e.g.,"
               "you create cut monitor with im/re option and added with im/re options",
               cut_name.Data()),
          EInfo::kError);
        return;
      }
      TString pattern = "";
      if (clas->InheritsFrom("Hal::EventCut")) {
        if (flag == -1) {  // im
          pattern = "Hal::EventImaginaryCut";
        } else if (flag == 1) {  // re
          pattern = "Hal::EventRealCut";
        }
      } else if (clas->InheritsFrom("Hal::TrackCut")) {
        if (flag == -1) {  // im
          pattern = "Hal::TrackImaginaryCut";
        } else if (flag == 1) {  // re
          pattern = "Hal::TrackRealCut";
        }
      } else {
        if (flag == -1) {  // im
          pattern = "Hal::TwoTrackImaginaryCut";
        } else if (flag == 1) {  // re
          pattern = "Hal::TwoTrackRealCut";
        }
      }
      moni->fCutNames[axis] = Form("%s(%s)", pattern.Data(), moni->GetCutName(axis).Data());
    };
    Hal::CutMonitor* res = x.MakeCopy();
    Int_t size           = res->GetAxisNo();
    Int_t flag           = 0;
    if (fRe) flag = 1;
    if (fIm) flag = -1;
    for (int i = 0; i < size; i++)
      MakeComplexAxis(res, i, flag);
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
    Hal::Cut* res = nullptr;
    auto upd      = cut.GetUpdateRatio();
    if (flag == "re") {  // real stuff
      switch (upd) {
        case ECutUpdate::kEvent: {
          res = new EventRealCut(static_cast<const EventCut&>(cut));
        } break;
        case ECutUpdate::kTrack: {
          res = new TrackRealCut(static_cast<const TrackCut&>(cut));
        } break;
        case ECutUpdate::kTwoTrack: {
          res = new TwoTrackRealCut(static_cast<const TwoTrackCut&>(cut));
        } break;
        case ECutUpdate::kTwoTrackBackground: {
          res = new TwoTrackRealCut(static_cast<const TwoTrackCut&>(cut));
        } break;
        default: return nullptr; break;
      }
    } else {  // imaginary stuff
      switch (upd) {
        case ECutUpdate::kEvent: {
          res = new EventImaginaryCut(static_cast<const EventCut&>(cut));
          if (acceptNulls) static_cast<EventImaginaryCut*>(res)->AcceptNulls(kTRUE);
        } break;
        case ECutUpdate::kTrack: {
          res = new TrackImaginaryCut(static_cast<const TrackCut&>(cut));
          if (acceptNulls) static_cast<TrackImaginaryCut*>(res)->AcceptNulls(kTRUE);
        } break;
        case ECutUpdate::kTwoTrack: {
          res = new TwoTrackImaginaryCut(static_cast<const TwoTrackCut&>(cut));
          if (acceptNulls) static_cast<TwoTrackImaginaryCut*>(res)->AcceptNulls(kTRUE);
        } break;
        case ECutUpdate::kTwoTrackBackground: {
          res = new TwoTrackImaginaryCut(static_cast<const TwoTrackCut&>(cut));
          if (acceptNulls) static_cast<TwoTrackImaginaryCut*>(res)->AcceptNulls(kTRUE);
        } break;
        default: return nullptr; break;
      }
    }
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
