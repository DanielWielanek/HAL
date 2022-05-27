/*
 * EventCombined.cxx
 *
 *  Created on: 4 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "ComplexEvent.h"


#include <TClonesArray.h>
#include <TMathBase.h>
#include <TObjArray.h>

#include "ComplexTrack.h"
#include "DataFormat.h"
#include "Track.h"

namespace Hal {
  Int_t* ComplexEvent::fgIndexMap      = nullptr;
  Int_t* ComplexEvent::fgSumMap        = nullptr;
  Int_t ComplexEvent::fgIndexArraySize = 0;
  Int_t ComplexEvent::fgSumMapSize     = 0;
  ComplexEvent::ComplexEvent(Event* real, Event* img) : ComplexEvent("Hal::ComplexTrack", real, img) {
    fSource = fRealEvent->GetSource();
  }

  ComplexEvent::ComplexEvent(TString track_class, Event* real, Event* img) :
    Event(track_class), fRealEvent(real), fImgEvent(img) {
    fSource = fRealEvent->GetSource();
  }

  void ComplexEvent::Update() {
    fImgEvent->Update();
    fRealEvent->Update();
    fSource = fRealEvent->GetSource();
    Event::ShallowCopyEvent(fRealEvent);
    fTracks->Clear();
    if (fRealEvent->GetTotalTrackNo()) {
      fTotalTracksNo = fRealEvent->GetTotalTrackNo();
      for (int i = 0; i < fTotalTracksNo; i++) {
        Track* tr        = (Track*) fRealEvent->fTracks->UncheckedAt(i);
        ComplexTrack* to = (ComplexTrack*) fTracks->ConstructedAt(i);
        to->ResetTrack(i, this);
        to->Track::CopyData(tr);
        to->SetImgTrack((Track*) fImgEvent->GetTrack(i));
        to->SetRealTrack(tr);
      }
    }
  }

  void ComplexEvent::LinkWithTree() {
    if (fImgEvent->GetSource() == nullptr) fImgEvent->CreateSource();
    fImgEvent->LinkWithTree();
    if (fRealEvent->GetSource() == nullptr) fRealEvent->CreateSource();
    fRealEvent->LinkWithTree();
    fSource = fRealEvent->GetSource();
  }

  Bool_t ComplexEvent::ExistInTree() const {
    if (fImgEvent && fRealEvent) {
      if (fImgEvent->ExistInTree() && fRealEvent->ExistInTree()) { return kTRUE; }
    }
    return kFALSE;
  }

  ComplexEvent::ComplexEvent(const ComplexEvent& other) : Event(other) {
    fImgEvent   = other.fImgEvent->GetNewEvent();
    fRealEvent  = other.fRealEvent->GetNewEvent();
    *fImgEvent  = *other.fImgEvent;
    *fRealEvent = *other.fRealEvent;
    fSource     = fRealEvent->fSource;
  }

  void ComplexEvent::Clear(Option_t* /*opt*/) {
    Event::Clear();
    fRealEvent->Clear();
    fImgEvent->Clear();
  }

  void ComplexEvent::ClearSource(Bool_t /*del*/) {}

  void ComplexEvent::ShallowCopyEvent(Event* event) {
    fRealEvent->ShallowCopyEvent(((ComplexEvent*) event)->GetRealEvent());
    fImgEvent->ShallowCopyEvent(((ComplexEvent*) event)->GetImgEvent());
    Event::ShallowCopyEvent(((ComplexEvent*) event)->GetRealEvent());
  }

  void ComplexEvent::CreateSource() {
    fRealEvent->CreateSource();
    fImgEvent->CreateSource();
    fSource = fRealEvent->GetSource();
  }

  ComplexEvent::~ComplexEvent() {
    if (fImgEvent) delete fImgEvent;
    fRealEvent->fSource = NULL;
    if (fRealEvent) delete fRealEvent;
    if (fgIndexMap) {
      delete[] fgIndexMap;
      delete[] fgSumMap;
      fgIndexMap = fgSumMap = NULL;
      fgIndexArraySize      = 0;
    }
  }

  void ComplexEvent::ShallowCopyTracks(Event* event) {
    fRealEvent->ShallowCopyTracks(((ComplexEvent*) event)->GetRealEvent());
    fImgEvent->ShallowCopyTracks(((ComplexEvent*) event)->GetImgEvent());
    ComplexEvent* mc_event = (ComplexEvent*) event;
    fTotalTracksNo         = mc_event->fTracks->GetEntriesFast();
    for (int i = 0; i < fTotalTracksNo; i++) {
      ComplexTrack* to   = (ComplexTrack*) fTracks->ConstructedAt(i);
      ComplexTrack* from = (ComplexTrack*) mc_event->fTracks->UncheckedAt(i);
      to->ResetTrack(i, this);
      to->CopyData(from);
      to->SetRealTrack(fRealEvent->GetTrack(i));
      Int_t match_id = to->GetMatchID();
      if (match_id >= 0) to->SetImgTrack(fImgEvent->GetTrack(match_id));
    }
  }

  void ComplexEvent::ShallowCopyCompressTracks(Event* event, Int_t* map, Int_t* mapID, Int_t map_size) {
    fRealEvent->ShallowCopyCompressTracks(((ComplexEvent*) event)->GetRealEvent(), map, mapID, map_size);
    CalculateCompressionMapImg(map, map_size, (ComplexEvent*) event);
    fImgEvent->ShallowCopyCompressTracks(((ComplexEvent*) event)->GetImgEvent(), fgSumMap, fgIndexMap, fgSumMapSize);
    ComplexEvent* mc_event = (ComplexEvent*) event;
    fTotalTracksNo         = map_size;
    for (int i = 0; i < fTotalTracksNo; i++) {
      ComplexTrack* to   = (ComplexTrack*) fTracks->ConstructedAt(i);
      ComplexTrack* from = (ComplexTrack*) mc_event->fTracks->UncheckedAt(map[i]);
      to->ResetTrack(i, this);
      to->CopyData(from);
      to->TranslateLinks(mapID);
      to->SetRealTrack(fRealEvent->GetTrack(i));
      if (from->GetMatchID() < 0) { continue; }
      to->SetMatchID(fgIndexMap[from->GetMatchID()]);
      Int_t match_id = to->GetMatchID();
      to->SetImgTrack(fImgEvent->GetTrack(match_id));
    }
  }

  void ComplexEvent::CalculateCompressionMapImg(Int_t* map, Int_t map_size, ComplexEvent* event) {
    Event* img_event    = event->GetImgEvent();
    const Int_t sizeImg = img_event->GetTotalTrackNo();
    const Int_t newSize = TMath::Max(sizeImg, map_size);
    if (fgIndexMap == NULL) {
      fgIndexMap       = new Int_t[newSize];
      fgSumMap         = new Int_t[newSize];
      fgIndexArraySize = newSize;
    } else if (newSize > fgIndexArraySize) {
      delete[] fgIndexMap;
      delete[] fgSumMap;
      fgIndexArraySize = newSize * 1.2;
      fgIndexMap       = new Int_t[fgIndexArraySize];
      fgSumMap         = new Int_t[fgIndexArraySize];
    }
    for (int i = 0; i < sizeImg; i++) {
      fgIndexMap[i] = -1;
    }
    for (int i = 0; i < map_size; i++) {
      ComplexTrack* tr = (ComplexTrack*) event->GetTrack(map[i]);
      if (tr->GetMatchID() >= 0) { fgIndexMap[tr->GetMatchID()] = 0; }
    }
    fgSumMapSize = 0;
    for (int i = 0; i < sizeImg; i++) {
      if (fgIndexMap[i] == 0) {
        fgIndexMap[i]          = fgSumMapSize;
        fgSumMap[fgSumMapSize] = i;
        ++fgSumMapSize;
      }
    }
  }

  void ComplexEvent::CopyHiddenSettings(const Event* event) {
    fRealEvent->CopyHiddenSettings(((ComplexEvent*) event)->fRealEvent);
    fImgEvent->CopyHiddenSettings(((ComplexEvent*) event)->fImgEvent);
  }

  Bool_t ComplexEvent::HasHiddenSettings() const {
    if (fRealEvent->HasHiddenSettings()) return kTRUE;
    if (fImgEvent->HasHiddenSettings()) return kTRUE;
    return kFALSE;
  }

  Bool_t ComplexEvent::IsCompatible(const Event* non_buffered) const {
    const ComplexEvent* ev = dynamic_cast<const ComplexEvent*>(non_buffered);
    if (ev == nullptr) return kFALSE;
    if (Event::IsCompatible(non_buffered)) return kTRUE;
    if (!ev->GetImgEvent()->IsCompatible(ev->GetImgEvent())) return kFALSE;
    if (!ev->GetRealEvent()->IsCompatible(ev->GetRealEvent())) return kFALSE;
    return kTRUE;
  }

  Float_t ComplexEvent::GetFieldVal(Int_t fieldID) const {

    if (fieldID >= DataFieldID::Internal::ImStepEvent) {
      if (GetImgEvent() == nullptr) return FLT_MIN;
      return GetImgEvent()->GetFieldVal(fieldID - DataFieldID::ImStep);
    } else if (fieldID >= DataFieldID::Internal::ReStepEvent) {
      return GetRealEvent()->GetFieldVal(fieldID - DataFieldID::ReStep);
    }

    switch (fieldID) {
      case DataFieldID::EComplexEvent::kDeltaRPhi: return GetImgEvent()->GetPhi() - GetRealEvent()->GetPhi(); break;
      case DataFieldID::EComplexEvent::kDeltaVertexXY: {
        Double_t dx = GetImgEvent()->GetVertex()->X() - GetRealEvent()->GetVertex()->X();
        Double_t dy = GetImgEvent()->GetVertex()->Y() - GetRealEvent()->GetVertex()->Y();
        return TMath::Sqrt(dx * dx + dy * dy);
      } break;
      case DataFieldID::EComplexEvent::kDeltaVertexX:
        return GetImgEvent()->GetVertex()->X() - GetRealEvent()->GetVertex()->X();
        break;
      case DataFieldID::EComplexEvent::kDeltaVertexY:
        return GetImgEvent()->GetVertex()->Y() - GetRealEvent()->GetVertex()->Y();
        break;
      case DataFieldID::EComplexEvent::kDeltaVertexZ:
        return GetImgEvent()->GetVertex()->Z() - GetRealEvent()->GetVertex()->Z();
        break;
      case DataFieldID::EComplexEvent::kDeltaVertexT:
        return GetImgEvent()->GetVertex()->T() - GetRealEvent()->GetVertex()->T();
        break;
      case DataFieldID::EComplexEvent::kDeltaVertex: {
        Double_t dx = GetImgEvent()->GetVertex()->X() - GetRealEvent()->GetVertex()->X();
        Double_t dy = GetImgEvent()->GetVertex()->Y() - GetRealEvent()->GetVertex()->Y();
        Double_t dz = GetImgEvent()->GetVertex()->Z() - GetRealEvent()->GetVertex()->Z();
        return TMath::Sqrt(dx * dx + dy * dy + dz * dz);
      } break;
      case DataFieldID::EComplexEvent::kDeltaTracksNo:
        return GetImgEvent()->GetTotalTrackNo() - GetRealEvent()->GetTotalTrackNo();
        break;
    }

    return Event::GetFieldVal(fieldID);
  }

  TString ComplexEvent::GetFieldName(Int_t fieldID) const {
    if (fieldID >= DataFieldID::Internal::ImStepEvent) {
      if (GetImgEvent() == nullptr) return "xxx";
      return GetImgEvent()->GetFieldName(fieldID - DataFieldID::ImStep) + " (im)";
    } else if (fieldID >= DataFieldID::Internal::ReStepEvent) {
      return GetRealEvent()->GetFieldName(fieldID - DataFieldID::ReStep) + " (re)";
    }
    switch (fieldID) {
      case DataFieldID::EComplexEvent::kDeltaRPhi: "#Delta #phi [rad]"; break;
      case DataFieldID::EComplexEvent::kDeltaVertexXY: return "#Delta V_{XY} [cm]"; break;
      case DataFieldID::EComplexEvent::kDeltaVertexX: return "#Delta V_{X} [cm]"; break;
      case DataFieldID::EComplexEvent::kDeltaVertexY: return "#Delta V_{Y} [cm]"; break;
      case DataFieldID::EComplexEvent::kDeltaVertexZ: return "#Delta V_{Z} [cm]"; break;
      case DataFieldID::EComplexEvent::kDeltaVertexT: return "#Delta V_{T} [s]"; break;
      case DataFieldID::EComplexEvent::kDeltaVertex: return "#Delta V_{XYZ} [cm]"; break;
      case DataFieldID::EComplexEvent::kDeltaTracksNo: return "#Delta N_{tracksNo} [AU]"; break;
    }
    return Event::GetFieldName(fieldID);
  }
}  // namespace Hal
