/*
 * CutOptions.cxx
 *
 *  Created on: 14 sty 2025
 *      Author: daniel
 */

#include "CutOptions.h"

#include "Cout.h"
#include "CutFuncs.h"
#include "CutMonitor.h"
#include "EventBinningCut.h"
#include "Std.h"
#include "StdString.h"

#include <TClass.h>

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
    fCollections = Hal::Cuts::GetCollectionsFlags(fDefCol, opt);
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
      return Hal::Cuts::MakeCutCopy(x, "re", kFALSE);
    } else if (fIm) {
      if (dynamic_cast<const Hal::EventBinningCut*>(&x)) {
        Hal::Cout::PrintInfo(Form("%s %i: cannot add binned cut with im flag", __FILE__, __LINE__), EInfo::kError);
        return nullptr;
      }
      return Hal::Cuts::MakeCutCopy(x, "im", fAcceptNull);
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

} /* namespace Hal */
