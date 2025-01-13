/*
 * CutFuncs.cxx
 *
 *  Created on: 25 lis 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CutFuncs.h"
#include "Cout.h"
#include "EventComplexCut.h"
#include "EventCut.h"
#include "Std.h"
#include "TrackComplexCut.h"
#include "TrackCut.h"
#include "TwoTrackComplexCut.h"
#include "TwoTrackCut.h"

namespace Hal {
  namespace Cuts {
    std::vector<Int_t> GetCollectionsFlags(Int_t startCol, TString option) {
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

    Hal::Cut* MakeCutCopy(const Hal::Cut& cut, TString flag, Bool_t acceptNulls) {
      if (flag == "re") {  // real stuff
        auto upd = cut.GetUpdateRatio();
        switch (upd) {
          case ECutUpdate::kEvent: {
            auto res = new EventRealCut(static_cast<const EventCut&>(cut));
            res->SetCollectionID(cut.GetCollectionID());
            return res;
          } break;
          case ECutUpdate::kTrack: {
            auto res = new TrackRealCut(static_cast<const TrackCut&>(cut));
            res->SetCollectionID(cut.GetCollectionID());
            return res;
          } break;
          case ECutUpdate::kTwoTrack: {
            auto res = new TwoTrackRealCut(static_cast<const TwoTrackCut&>(cut));
            res->SetCollectionID(cut.GetCollectionID());
            return res;
          } break;
          case ECutUpdate::kTwoTrackBackground: {
            auto res = new TwoTrackRealCut(static_cast<const TwoTrackCut&>(cut));
            res->SetCollectionID(cut.GetCollectionID());
            return res;
          } break;
          default: return nullptr; break;
        }
      } else {  // imaginary stuff
        auto upd = cut.GetUpdateRatio();
        switch (upd) {
          case ECutUpdate::kEvent: {
            auto tempcut = new EventImaginaryCut(static_cast<const EventCut&>(cut));
            if (acceptNulls) static_cast<EventImaginaryCut*>(tempcut)->AcceptNulls(kTRUE);
            tempcut->SetCollectionID(cut.GetCollectionID());
            return tempcut;
          } break;
          case ECutUpdate::kTrack: {
            auto tempcut = new TrackImaginaryCut(static_cast<const TrackCut&>(cut));
            if (acceptNulls) static_cast<TrackImaginaryCut*>(tempcut)->AcceptNulls(kTRUE);
            tempcut->SetCollectionID(cut.GetCollectionID());
            return tempcut;
          } break;
          case ECutUpdate::kTwoTrack: {
            auto tempcut = new TwoTrackImaginaryCut(static_cast<const TwoTrackCut&>(cut));
            if (acceptNulls) static_cast<TwoTrackImaginaryCut*>(tempcut)->AcceptNulls(kTRUE);
            tempcut->SetCollectionID(cut.GetCollectionID());
            return tempcut;
          } break;
          case ECutUpdate::kTwoTrackBackground: {
            auto tempcut = new TwoTrackImaginaryCut(static_cast<const TwoTrackCut&>(cut));
            if (acceptNulls) static_cast<TwoTrackImaginaryCut*>(tempcut)->AcceptNulls(kTRUE);
            tempcut->SetCollectionID(cut.GetCollectionID());
            return tempcut;
          } break;
          default: return nullptr; break;
        }
      }
      return nullptr;
    }
    TString GetCutUpdateRatioName(ECutUpdate upd) {
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
  }  // namespace Cuts
} /* namespace Hal */
