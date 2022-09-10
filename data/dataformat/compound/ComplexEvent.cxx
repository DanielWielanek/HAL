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

#include "ComplexEventInterface.h"
#include "ComplexTrack.h"
#include "CompressionMap.h"
#include "DataFormat.h"
#include "Track.h"

namespace Hal {
  CompressionMap ComplexEvent::fgCompressionMap;

  ComplexEvent::ComplexEvent(Event* real, Event* img) : ComplexEvent("Hal::ComplexTrack", real, img) {}

  ComplexEvent::ComplexEvent(TString track_class, Event* real, Event* img) :
    Event(track_class), fRealEvent(real), fImgEvent(img) {}

  void ComplexEvent::Update(EventInterface* interface) {
    ComplexEventInterface* source = (ComplexEventInterface*) interface;
    fImgEvent->Update(source->GetReal());
    fRealEvent->Update(source->GetImag());
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

  Bool_t ComplexEvent::ExistInTree() const {
    if (fImgEvent && fRealEvent) {
      if (fImgEvent->ExistInTree() && fRealEvent->ExistInTree()) { return kTRUE; }
    }
    return kFALSE;
  }

  ComplexEvent::ComplexEvent(const ComplexEvent& other) : Event(other) {
    fImgEvent  = other.fImgEvent->GetNewEvent();
    fRealEvent = other.fRealEvent->GetNewEvent();
    fImgEvent->CopyData(other.fImgEvent);
    fRealEvent->CopyData(other.fRealEvent);
  }

  void ComplexEvent::Clear(Option_t* /*opt*/) {
    Event::Clear();
    fRealEvent->Clear();
    fImgEvent->Clear();
  }

  void ComplexEvent::ShallowCopyEvent(Event* event) {
    fRealEvent->ShallowCopyEvent(((ComplexEvent*) event)->GetRealEvent());
    fImgEvent->ShallowCopyEvent(((ComplexEvent*) event)->GetImgEvent());
    Event::ShallowCopyEvent(((ComplexEvent*) event)->GetRealEvent());
  }

  EventInterface* ComplexEvent::CreateInterface() const {
    EventInterface* re = fRealEvent->CreateInterface();
    EventInterface* im = fImgEvent->CreateInterface();
    return new ComplexEventInterface(re, im);
  }

  ComplexEvent::~ComplexEvent() {
    if (fImgEvent) delete fImgEvent;
    if (fRealEvent) delete fRealEvent;
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

  void ComplexEvent::ShallowCopyCompressTracks(Event* event, const CompressionMap& map) {
    fRealEvent->ShallowCopyCompressTracks(((ComplexEvent*) event)->GetRealEvent(), map);
    CalculateCompressionMapImg(map, (ComplexEvent*) event);
    fImgEvent->ShallowCopyCompressTracks(((ComplexEvent*) event)->GetImgEvent(), fgCompressionMap);
    ComplexEvent* mc_event = (ComplexEvent*) event;
    fTotalTracksNo         = map.GetNewSize();
    for (int i = 0; i < fTotalTracksNo; i++) {
      ComplexTrack* to   = (ComplexTrack*) fTracks->ConstructedAt(i);
      ComplexTrack* from = (ComplexTrack*) mc_event->fTracks->UncheckedAt(map.GetOldIndex(i));
      to->ResetTrack(i, this);
      to->CopyData(from);
      to->TranslateLinks(map);
      to->SetRealTrack(fRealEvent->GetTrack(i));
      if (from->GetMatchID() < 0) { continue; }
      to->SetMatchID(fgCompressionMap.GetNewIndex(from->GetMatchID()));
      Int_t match_id = to->GetMatchID();
      to->SetImgTrack(fImgEvent->GetTrack(match_id));
    }
  }

  void ComplexEvent::CalculateCompressionMapImg(const CompressionMap& map, ComplexEvent* event) {
    Event* img_event    = event->GetImgEvent();
    const Int_t sizeImg = img_event->GetTotalTrackNo();
    const Int_t newSize = TMath::Max(sizeImg, map.GetSize());
    fgCompressionMap.Reset(newSize);
    std::vector<int> links;
    links.resize(GetImgEvent()->GetMaxExpectedLinks());
    for (int i = 0; i < map.GetNewSize(); i++) {
      ComplexTrack* tr = (ComplexTrack*) event->GetTrack(map.GetOldIndex(i));
      Int_t matchId    = tr->GetMatchID();
      if (matchId >= 0) {
        // fgCompressionMap.MarkAsGood(matchId);
        Track* mcTrack = event->GetImgEvent()->GetTrack(matchId);
        int linksNo    = mcTrack->GetLinksFast(links, kTRUE);
        for (int iLink = 0; iLink < linksNo; iLink++) {
          fgCompressionMap.MarkAsGood(links[iLink]);
        }
      }
    }
    fgCompressionMap.Recalculate();
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
      case DataFieldID::Event::EComplex::kDeltaRPhi: return GetImgEvent()->GetPhi() - GetRealEvent()->GetPhi(); break;
      case DataFieldID::Event::EComplex::kDeltaVertexXY: {
        Double_t dx = GetImgEvent()->GetVertex()->X() - GetRealEvent()->GetVertex()->X();
        Double_t dy = GetImgEvent()->GetVertex()->Y() - GetRealEvent()->GetVertex()->Y();
        return TMath::Sqrt(dx * dx + dy * dy);
      } break;
      case DataFieldID::Event::EComplex::kDeltaVertexX:
        return GetImgEvent()->GetVertex()->X() - GetRealEvent()->GetVertex()->X();
        break;
      case DataFieldID::Event::EComplex::kDeltaVertexY:
        return GetImgEvent()->GetVertex()->Y() - GetRealEvent()->GetVertex()->Y();
        break;
      case DataFieldID::Event::EComplex::kDeltaVertexZ:
        return GetImgEvent()->GetVertex()->Z() - GetRealEvent()->GetVertex()->Z();
        break;
      case DataFieldID::Event::EComplex::kDeltaVertexT:
        return GetImgEvent()->GetVertex()->T() - GetRealEvent()->GetVertex()->T();
        break;
      case DataFieldID::Event::EComplex::kDeltaVertex: {
        Double_t dx = GetImgEvent()->GetVertex()->X() - GetRealEvent()->GetVertex()->X();
        Double_t dy = GetImgEvent()->GetVertex()->Y() - GetRealEvent()->GetVertex()->Y();
        Double_t dz = GetImgEvent()->GetVertex()->Z() - GetRealEvent()->GetVertex()->Z();
        return TMath::Sqrt(dx * dx + dy * dy + dz * dz);
      } break;
      case DataFieldID::Event::EComplex::kDeltaTracksNo:
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
      case DataFieldID::Event::EComplex::kDeltaRPhi: return "#Delta #phi [rad]"; break;
      case DataFieldID::Event::EComplex::kDeltaVertexXY: return "#Delta V_{XY} [cm]"; break;
      case DataFieldID::Event::EComplex::kDeltaVertexX: return "#Delta V_{X} [cm]"; break;
      case DataFieldID::Event::EComplex::kDeltaVertexY: return "#Delta V_{Y} [cm]"; break;
      case DataFieldID::Event::EComplex::kDeltaVertexZ: return "#Delta V_{Z} [cm]"; break;
      case DataFieldID::Event::EComplex::kDeltaVertexT: return "#Delta V_{T} [ns]"; break;
      case DataFieldID::Event::EComplex::kDeltaVertex: return "#Delta V_{XYZ} [cm]"; break;
      case DataFieldID::Event::EComplex::kDeltaTracksNo: return "#Delta N_{tracksNo} [AU]"; break;
    }
    return Event::GetFieldName(fieldID);
  }
}  // namespace Hal
