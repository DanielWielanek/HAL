/*
 * NicaUnigenSource.cxx
 *
 *  Created on: 2 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "UnigenEventInterface.h"
#include "UEvent.h"
#include "UParticle.h"
#include "UnigenTrackInterface.h"

#include "DataManager.h"
#include "TrackInterface.h"

#include <iostream>
#include <vector>

#include <RtypesCore.h>
#include <TCollection.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TNamed.h>
#include <TObjString.h>
#include <TString.h>

namespace HalUni {
  UnigenEventInterface::UnigenEventInterface() : Hal::EventInterfaceAdvanced(new UnigenTrackInterface()) {}

  void UnigenEventInterface::Compress(Int_t* map, Int_t map_size) {
    Int_t track_pos = 0;
    for (int i = 0; i < map_size; i++) {
      Int_t good_track = map[i];
      for (int j = track_pos; j < good_track; j++) {
        GetUEvent()->RemoveAt(j);
      }
      track_pos = good_track + 1;
    }
  }

  void UnigenEventInterface::CopyData(Hal::EventInterface* s) {
#ifdef UNIGEN_OLD
    CopyUnigen(((HalCbmUnigenEventInterface*) s)->GetUEvent(), GetUEvent());
#else
    *GetUEvent() = *((UnigenEventInterface*) s)->GetUEvent();
#endif
  }

  void UnigenEventInterface::CopyAndCompress(Hal::EventInterface* s, Int_t* map, Int_t map_size) {
    UnigenEventInterface* ev = (UnigenEventInterface*) s;
    GetUEvent()->SetB(ev->GetUEvent()->GetB());
    GetUEvent()->SetPhi(ev->GetUEvent()->GetPhi());
    GetUEvent()->SetNes(ev->GetUEvent()->GetNes());
    GetUEvent()->SetStepNr(ev->GetUEvent()->GetStepNr());
    GetUEvent()->SetStepT(ev->GetUEvent()->GetStepT());
#ifdef UNIGEN_OLD
    GetUEvent()->GetParticleList()->Clear();
#else
    TString comment;
    ev->GetUEvent()->GetComment(comment);
    GetUEvent()->SetComment(comment);
    GetUEvent()->Clear();
#endif
    for (int i = 0; i < map_size; i++) {
      GetUEvent()->AddParticle(*ev->GetUEvent()->GetParticle(map[i]));
    }
  }

  void UnigenEventInterface::Boost(Double_t vx, Double_t vy, Double_t vz) {
    for (int i = 0; i < GetUEvent()->GetNpa(); i++) {
      UParticle* p       = GetUEvent()->GetParticle(i);
      TLorentzVector mom = p->GetMomentum();
      TLorentzVector pos = p->GetPosition();
      mom.Boost(vx, vy, vz);
      pos.Boost(vx, vy, vz);
      p->SetMomentum(mom);
      p->SetPosition(pos);
    }
  }

  UnigenEventInterface::~UnigenEventInterface() {
    if (fCanDeleteEvent && fEventPointer->GetPointer()) {
      if (fEventPointer->GetPointer()) delete fEventPointer->GetPointer();
    }
    if (fEventPointer) delete fEventPointer;
  }

  void UnigenEventInterface::Register(Bool_t write) {
    fEventPointer             = new Hal::ObjectDoublePointer(new UEvent(), false);
    Hal::DataManager* manager = Hal::DataManager::Instance();
    manager->Register("UEvent.", "", (TNamed*) fEventPointer->GetDoublePointer(), write);
  }

  void UnigenEventInterface::FillTrackInterface(Hal::TrackInterface* track, Int_t index) {
    track->SetRawTrack(GetUEvent()->GetParticle(index));
  }

  void UnigenEventInterface::ConnectToTreeInternal(eMode mode) {
    Hal::DataManager* manager = Hal::DataManager::Instance();

    switch (mode) {
      case Hal::EventInterface::eMode::kRead: {
        std::vector<TString> names;
        names.push_back("UEvent.");
        names.push_back("UEvent");
        names.push_back("event");
        names.push_back("events");
        for (auto i : names) {
          auto Event = dynamic_cast<UEvent*>(manager->GetTObject(i)->GetPointer());
          if (Event != nullptr) {
            fEventPointer = manager->GetTObject(i);
            return;
          }
        }
      } break;
      case Hal::EventInterface::eMode::kWrite: {

        fEventPointer = new Hal::ObjectDoublePointer(new UEvent(), false);
        // fEvent        = new UEvent();
        manager->Register("UEvent.", "unigen", (TNamed*) fEventPointer->GetDoublePointer(), kTRUE);
      } break;
      case Hal::EventInterface::eMode::kWriteVirtual: {
        fEventPointer = new Hal::ObjectDoublePointer(new UEvent(), false);
        manager->Register("UEvent.", "unigen", (TNamed*) fEventPointer->GetDoublePointer(), kFALSE);
        fCanDeleteEvent = kTRUE;

      } break;
    }
  }

}  // namespace HalUni
