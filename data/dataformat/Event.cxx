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
    fSource(nullptr),
    fTracks(nullptr),
    fVertex(nullptr),
    fPhi(0),
    fPhiError(0),
    fEventId(0),
    fTotalTracksNo(0),
    fTotalV0s(0) {
    fPDG           = TDatabasePDG::Instance();
    fVertex        = new TLorentzVector();
    fV0sHiddenInfo = new TClonesArray("Hal::V0Track");
  }

  Event::Event(TString track_class, TString v0_class) :
    fSource(nullptr), fPhi(0), fPhiError(0), fEventId(0), fTotalTracksNo(0), fTotalV0s(0) {
    fPDG           = TDatabasePDG::Instance();
    fVertex        = new TLorentzVector();
    fTracks        = new TClonesArray(track_class);
    fV0sHiddenInfo = new TClonesArray(v0_class);
  }

  Event::Event(const Event& other) : TNamed(other), fSource(NULL) {
    fPDG           = TDatabasePDG::Instance();
    fTracks        = new TClonesArray(*other.fTracks);
    fVertex        = new TLorentzVector(*other.fVertex);
    fPhi           = other.fPhi;
    fPhiError      = other.fPhiError;
    fTotalTracksNo = other.fTotalTracksNo;
    fTotalV0s      = other.fTotalV0s;
    fEventId       = other.fEventId;
    fV0sHiddenInfo = new TClonesArray(*other.fV0sHiddenInfo);
    if (other.fSource) { fSource = (EventInterface*) fSource->Clone(); }
  }

  TObject* Event::GetEventPointer() const {
    EventInterfaceAdvanced* source = dynamic_cast<EventInterfaceAdvanced*>(fSource);
    if (source) { return source->GetRawEventPointer(); }
    return nullptr;
  }

  TObject* Event::GetTrackPointer(Int_t index) const {
    EventInterfaceAdvanced* source = dynamic_cast<EventInterfaceAdvanced*>(fSource);
    if (source) { return source->GetRawTrackPointer(index); }
    return nullptr;
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
  }

  void Event::Print(Option_t* /*opt*/) const {
    Cout::Database(2, "Event Phi", "Event Multiplicity");
    Cout::Database(2, Form("%4.2f", GetPhi()), Form("%i", GetTotalTrackNo()));
  }

  void Event::Compress(Int_t* map, Int_t map_size) {
    Compress(fTracks, map, map_size);
    fTotalTracksNo = fTracks->GetEntriesFast();
  }

  void Event::Build(Event* event, Int_t* map, Int_t* mapID, Int_t map_size) {
    if (map) {  // with compression
      CopyCompress(event, map, mapID, map_size);
    } else {  // without compression
      CopyData(event);
    }
  }

  void Event::ShallowCopyEvent(Event* event) {
    fTotalTracksNo = event->GetTotalTrackNo();
    *fVertex       = *event->GetVertex();
    fPhi           = event->fPhi;
    fPhiError      = event->fPhiError;
    fEventId       = event->GetEventID();
  }

  void Event::ShallowCopyTracks(Event* event) {
    fTracks->Clear();
    fTotalTracksNo = event->fTracks->GetEntriesFast();
    fTracks->ExpandCreateFast(fTotalTracksNo);
    for (int i = 0; i < fTotalTracksNo; i++) {
      Track* to   = (Track*) fTracks->UncheckedAt(i);
      Track* from = (Track*) event->fTracks->UncheckedAt(i);
      to->ResetTrack(i, this);
      to->CopyData(from);
    }
  }

  void Event::DeleteSource() {
    if (fSource) {
      delete fSource;
      fSource = NULL;
    }
  }

  void Event::Register(Bool_t write) {
    DataManager* ioManager = DataManager::Instance();
    ioManager->Register(this->ClassName(), "", this, write);
  }

  void Event::Boost(Double_t vx, Double_t vy, Double_t vz) {
    for (int i = 0; i < fTotalTracksNo; i++) {
      Track* track = (Track*) fTracks->UncheckedAt(i);
      track->Boost(vx, vy, vz);
    }
  }

  void Event::Compress(TClonesArray* array, Int_t* map, Int_t map_size) {
    Int_t prim_pos = 0;
    for (int i = 0; i < map_size; i++) {
      for (int j = prim_pos; j < map[i]; j++) {
        array->RemoveAt(j);
      }
      prim_pos = map[i] + 1;
    }
    array->Compress();
  }

  void Event::CopyData(Event* event) {
    ShallowCopyEvent(event);
    ShallowCopyHiddenInfo(event);
    ShallowCopyTracks(event);
    if (fSource) {
      EventInterfaceAdvanced* source = dynamic_cast<EventInterfaceAdvanced*>(fSource);
      if (source) { source->CopyData(event->fSource); }
    }
  }

  void Event::CopyCompress(Event* event, Int_t* map, Int_t* mapID, Int_t map_size) {
    ShallowCopyEvent(event);
    ShallowCopyHiddenInfo(event);
    ShallowCopyCompressTracks(event, map, mapID, map_size);
    if (fSource) {
      EventInterfaceAdvanced* source = dynamic_cast<EventInterfaceAdvanced*>(fSource);
      if (source) source->CopyAndCompress(event->fSource, map, map_size);
    }
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

  void Event::Update() {
    fTracks->Clear();
    EventInterfaceAdvanced* source = dynamic_cast<EventInterfaceAdvanced*>(fSource);
    if (source) {
      fPhi                = source->GetPhi();
      fPhiError           = source->GetPhiError();
      TLorentzVector vert = source->GetVertex();
      fVertex->SetXYZT(vert.X(), vert.Y(), vert.Z(), vert.T());
      fTotalTracksNo = source->GetTotalTrackNo();
      fTracks->ExpandCreateFast(fTotalTracksNo);
      TrackInterface* interface = source->GetTrackInterface();
      for (int i = 0; i < fTotalTracksNo; i++) {
        Track* from = GetTrack(i);
        from->ResetTrack(i, this);
        from->SetID(interface->GetID());
        source->FillTrackInterface(interface, i);
        from->SetCharge(interface->GetCharge());
        from->SetMotherIndex(interface->GetMotherIndex());
        if (interface->IsPrimary()) {
          from->SetPrimary();
        } else {
          from->SetSecondary(kFALSE);
        }
        from->SetV0(kFALSE, kFALSE);
        from->SetMomentum(interface->GetPz(), interface->GetPy(), interface->GetPz(), interface->GetE());
      }
      delete interface;
    }
  }

  void Event::RegisterSource(Bool_t write) {
    if (write) {
      fSource->LinkWithTree(EventInterface::eMode::kWrite);
    } else {
      fSource->LinkWithTree(EventInterface::eMode::kWriteVirtual);
    }
  }

  void Event::ShallowCopyCompressTracks(Event* event, Int_t* map, Int_t* mapID, Int_t map_size) {
    fTracks->Clear();
    fTotalTracksNo = map_size;
    fTracks->ExpandCreateFast(fTotalTracksNo);
    for (int i = 0; i < map_size; i++) {
      Track* to   = (Track*) fTracks->UncheckedAt(i);
      Track* from = (Track*) event->fTracks->UncheckedAt(map[i]);
      to->ResetTrack(i, this);
      to->CopyData(from);
      to->TranslateLinks(mapID);
    }
  }

  Track* Event::GetNewTrack() const {
    TClass* cl = fTracks->GetClass();
    return (Track*) cl->New(TClass::kClassNew);
  }

  Event* Event::GetNewEvent() const {
    TClass* cl = TClass::GetClass(this->ClassName(), 1, 1);
    Event* ev  = (Event*) cl->New(TClass::kClassNew);
    ev->CopyHiddenSettings(this);
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
      case DataFieldID::EEvent::kEventPhi: return GetPhi(); break;
      case DataFieldID::EEvent::kPhiError: return GetPhiError(); break;
      case DataFieldID::EEvent::kTracksNo: return GetTotalTrackNo(); break;
      case DataFieldID::EEvent::kVertexXY: return GetVertex()->Pt(); break;

      case DataFieldID::EEvent::kVertexX: return GetVertex()->X(); break;
      case DataFieldID::EEvent::kVertexY: return GetVertex()->Y(); break;
      case DataFieldID::EEvent::kVertexZ: return GetVertex()->Z(); break;
      case DataFieldID::EEvent::kVertexT: return GetVertex()->T(); break;
      case DataFieldID::EEvent::kEventId: return GetEventID(); break;
      case DataFieldID::EEvent::kEventZero: return 0;
    }
    return -FLT_MAX;
  }

  TString Event::GetFieldName(Int_t fieldID) const {
    switch (fieldID) {
      case DataFieldID::EEvent::kEventPhi: return "#phi [rad]"; break;
      case DataFieldID::EEvent::kPhiError: return "#sigma#phi [rad]"; break;
      case DataFieldID::EEvent::kTracksNo: return "N_{tracks} [AU]"; break;
      case DataFieldID::EEvent::kVertexXY: return "V_{xy} [cm]"; break;

      case DataFieldID::EEvent::kVertexX: return "V_{x} [cm]"; break;
      case DataFieldID::EEvent::kVertexY: return "V_{y} [cm]"; break;
      case DataFieldID::EEvent::kVertexZ: return "V_{z} [cm]"; break;
      case DataFieldID::EEvent::kVertexT: return "V_{t} [s]"; break;
      case DataFieldID::EEvent::kEventId: return "EventID [ID]"; break;
    }
    Cout::PrintInfo(Form("Event::GetFieldName cannot find field with fieldID  %i", fieldID), EInfo::kLowWarning);
    return "[]";
  }

  void Event::ShallowCopyHiddenInfo(Event* event) {
    fV0sHiddenInfo->ExpandCreateFast(event->fTotalV0s);
    fTotalV0s = event->fTotalV0s;
    for (int i = 0; i < fTotalV0s; i++) {
      V0Track* from = (V0Track*) event->fV0sHiddenInfo->UncheckedAt(i);
      V0Track* to   = (V0Track*) fV0sHiddenInfo->UncheckedAt(i);
      to->CopyData(from);
    }
  }

}  // namespace Hal
