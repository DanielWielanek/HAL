/*
 * Event.cxx
 *
 *  Created on: 23-06-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Event.h"
#include "DataFormat.h"
#include "EventInterfaceAdvanced.h"
#include "TrackInterface.h"
#include "V0Track.h"

#include "CompressionMap.h"
#include "Cout.h"
#include "DataManager.h"
#include "IOManager.h"
#include "Package.h"

#include <cstdarg>

#include <RtypesCore.h>
#include <TClass.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TParticlePDG.h>
#include <stddef.h>

namespace Hal {
  Event::Event() :
    TNamed(),
    fTotalV0s(0),
    fTracks(nullptr),
    fVertex(nullptr),
    fPhi(0),
    fPhiError(0),
    fEventId(0),
    fTotalTracksNo(0),
    fMultiplicity(0) {
    fPDG           = TDatabasePDG::Instance();
    fVertex        = new TLorentzVector();
    fV0sHiddenInfo = new TClonesArray("Hal::V0Track");
  }

  Event::Event(TString track_class, TString v0_class) :
    fTotalV0s(0), fPhi(0), fPhiError(0), fEventId(0), fTotalTracksNo(0), fMultiplicity(0) {
    fPDG           = TDatabasePDG::Instance();
    fVertex        = new TLorentzVector();
    fTracks        = new TClonesArray(track_class);
    fV0sHiddenInfo = new TClonesArray(v0_class);
  }

  Event::Event(const Event& other) : TNamed(other) {
    fPDG           = TDatabasePDG::Instance();
    fTracks        = new TClonesArray(*other.fTracks);
    fVertex        = new TLorentzVector(*other.fVertex);
    fPhi           = other.fPhi;
    fPhiError      = other.fPhiError;
    fTotalTracksNo = other.fTotalTracksNo;
    fTotalV0s      = other.fTotalV0s;
    fEventId       = other.fEventId;
    fMultiplicity  = other.fMultiplicity;
    fV0sHiddenInfo = new TClonesArray(*other.fV0sHiddenInfo);
  }

  Event::~Event() {
    if (fTracks) {
      delete fTracks;
      delete fVertex;
      delete fV0sHiddenInfo;
    }
  }

  void Event::Clear(Option_t* opt) {
    fTracks->Clear(opt);
    fV0sHiddenInfo->Clear(opt);
    fTotalTracksNo = 0;
    fTotalV0s      = 0;
  }

  void Event::Print(Option_t* /*opt*/) const {
    Cout::Database(2, "Event Phi", "Event Multiplicity");
    Cout::Database(2, Form("%4.2f", GetPhi()), Form("%i", GetTotalTrackNo()));
  }

  void Event::Build(Event* event, const CompressionMap& map) { CopyCompress(event, map); }

  void Event::Build(Event* event) { CopyData(event); }

  void Event::ShallowCopyEvent(Event* event) {
    fMultiplicity  = event->GetMutliplicity();
    *fVertex       = *event->GetVertex();
    fPhi           = event->fPhi;
    fPhiError      = event->fPhiError;
    fEventId       = event->GetEventID();
    fTotalV0s      = 0;  // this need to be set by user!
    fTotalTracksNo = 0;  // this also should be set by the user
  }

  void Event::ShallowCopyTracks(Event* event) {
    fTotalTracksNo = event->fTracks->GetEntriesFast();
    fV0sHiddenInfo->ExpandCreateFast(event->fTotalV0s);
    fTracks->ExpandCreateFast(fTotalTracksNo);
    for (int i = 0; i < fTotalTracksNo; i++) {
      Track* to   = (Track*) fTracks->UncheckedAt(i);
      Track* from = (Track*) event->fTracks->UncheckedAt(i);
      to->ResetTrack(i, this);
      to->CopyData(from);
    }
  }

  void Event::Boost(Double_t vx, Double_t vy, Double_t vz) {
    for (int i = 0; i < fTotalTracksNo; i++) {
      Track* track = (Track*) fTracks->UncheckedAt(i);
      track->Boost(vx, vy, vz);
    }
  }

  void Event::Compress(const CompressionMap& map) {
    Hal::Std::CompressArray(fTracks, map);
    fTotalTracksNo = fTracks->GetEntriesFast();
  }

  void Event::CopyData(Event* event) {
    ShallowCopyEvent(event);
    ShallowCopyTracks(event);
  }

  void Event::CopyCompress(Event* event, const CompressionMap& map) {
    ShallowCopyEvent(event);
    ShallowCopyCompressTracks(event, map);
  }

  Double_t Event::CalculateCharge(Int_t pdg) const {
    TParticlePDG* part = fPDG->GetParticle(pdg);
    if (part) {
      return part->Charge() / 3.0;
    } else {
      return 0;
    }
  }

  TString Event::GetFormatName() const { return ClassName(); }

  void Event::Update(EventInterface* interface) {
    fTracks->Clear();
    fV0sHiddenInfo->Clear();
    fTotalV0s                      = 0;
    EventInterfaceAdvanced* source = dynamic_cast<EventInterfaceAdvanced*>(interface);
    if (source) {
      fPhi                = source->GetPhi();
      fPhiError           = source->GetPhiError();
      TLorentzVector vert = source->GetVertex();
      fVertex->SetXYZT(vert.X(), vert.Y(), vert.Z(), vert.T());
      fTotalTracksNo = source->GetTotalTrackNo();
      fTracks->ExpandCreateFast(fTotalTracksNo);
      TrackInterface* trInterface = source->GetTrackInterface();
      for (int i = 0; i < fTotalTracksNo; i++) {
        Track* from = GetTrack(i);
        from->ResetTrack(i, this);
        from->SetID(trInterface->GetID());
        source->FillTrackInterface(trInterface, i);
        from->SetCharge(trInterface->GetCharge());
        from->SetMotherIndex(trInterface->GetMotherIndex());
        if (trInterface->IsPrimary()) {
          from->SetPrimary();
        } else {
          from->SetSecondary(kFALSE);
        }
        from->EnableV0(kFALSE, kFALSE);
        from->SetMomentum(trInterface->GetPz(), trInterface->GetPy(), trInterface->GetPz(), trInterface->GetE());
      }
    }
  }

  void Event::ShallowCopyCompressTracks(Event* event, const CompressionMap& map) {
    fV0sHiddenInfo->Clear();
    fTotalTracksNo = map.GetNewSize();
    fTracks->ExpandCreateFast(fTotalTracksNo);
    for (int i = 0; i < fTotalTracksNo; i++) {
      Track* to   = (Track*) fTracks->UncheckedAt(i);
      Track* from = (Track*) event->fTracks->UncheckedAt(map.GetOldIndex(i));
      to->ResetTrack(i, this);
      to->CopyData(from);
      to->TranslateLinks(map);
    }
  }

  Track* Event::GetNewTrack() const {
    TClass* cl = fTracks->GetClass();
    return (Track*) cl->New(TClass::kClassNew);
  }

  Track* Event::AddTrack() {
    Track* tr = (Track*) fTracks->ConstructedAt(fTotalTracksNo);
    tr->ResetTrack(fTotalTracksNo++, this);
    return tr;
  }

  Event* Event::GetNewEvent() const {
    TClass* cl = TClass::GetClass(this->ClassName(), 1, 1);
    Event* ev  = (Event*) cl->New(TClass::kClassNew);
    return ev;
  }

  Bool_t Event::CheckBranches(Int_t n...) const {
    va_list args;
    va_start(args, n);
    DataManager* mngr = DataManager::Instance();
    for (int i = 0; i < n; i++) {
      const char* name = va_arg(args, char*);
      if (!mngr->CheckBranch(name)) {
        Cout::PrintInfo(Form("Check branch: %s, branch not found", name), EInfo::kLowWarning);
        return kFALSE;
      }
    }
    return kTRUE;
  }

  Bool_t Event::IsCompatible(const Event* non_buffered) const {
    TString formatName = non_buffered->GetFormatName();
    TString thisName   = this->GetFormatName();
    if (formatName == thisName) return kTRUE;
    return kFALSE;
  }

  Package* Event::Report() const { return new Package(this); }

  Float_t Event::GetFieldVal(Int_t fieldID) const {
    switch (fieldID) {
      case DataFieldID::Event::EBasic::kEventPhi: return GetPhi(); break;
      case DataFieldID::Event::EBasic::kPhiError: return GetPhiError(); break;
      case DataFieldID::Event::EBasic::kTracksNo: return GetTotalTrackNo(); break;
      case DataFieldID::Event::EBasic::kVertexXY: return GetVertex()->Pt(); break;

      case DataFieldID::Event::EBasic::kVertexX: return GetVertex()->X(); break;
      case DataFieldID::Event::EBasic::kVertexY: return GetVertex()->Y(); break;
      case DataFieldID::Event::EBasic::kVertexZ: return GetVertex()->Z(); break;
      case DataFieldID::Event::EBasic::kVertexT: return GetVertex()->T(); break;
      case DataFieldID::Event::EBasic::kEventId: return GetEventID(); break;
      case DataFieldID::Event::EBasic::kMultiplicity: return GetMutliplicity(); break;
      case DataFieldID::Event::EBasic::kEventZero: return 0;
    }
    return -FLT_MAX;
  }

  TString Event::GetFieldName(Int_t fieldID) const {
    switch (fieldID) {
      case DataFieldID::Event::EBasic::kEventPhi: return "#phi [rad]"; break;
      case DataFieldID::Event::EBasic::kPhiError: return "#sigma#phi [rad]"; break;
      case DataFieldID::Event::EBasic::kTracksNo: return "N_{tracks} [AU]"; break;
      case DataFieldID::Event::EBasic::kVertexXY: return "V_{xy} [cm]"; break;

      case DataFieldID::Event::EBasic::kVertexX: return "V_{x} [cm]"; break;
      case DataFieldID::Event::EBasic::kVertexY: return "V_{y} [cm]"; break;
      case DataFieldID::Event::EBasic::kVertexZ: return "V_{z} [cm]"; break;
      case DataFieldID::Event::EBasic::kVertexT: return "V_{t} [s]"; break;
      case DataFieldID::Event::EBasic::kEventId: return "EventID [ID]"; break;
      case DataFieldID::Event::EBasic::kMultiplicity: return "Multiplicity [N_{tracks}]"; break;
    }
    Cout::PrintInfo(Form("Event::GetFieldName cannot find field with fieldID  %i", fieldID), EInfo::kLowWarning);
    return "[]";
  }

}  // namespace Hal
