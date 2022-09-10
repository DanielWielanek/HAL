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
  UnigenEventInterface::UnigenEventInterface() : Hal::EventInterfaceAdvanced(new UnigenTrackInterface()), fEvent(nullptr) {
    fEvent = new UEvent();
  }

  void UnigenEventInterface::Compress(Int_t* map, Int_t map_size) {
    Int_t track_pos = 0;
    for (int i = 0; i < map_size; i++) {
      Int_t good_track = map[i];
      for (int j = track_pos; j < good_track; j++) {
        fEvent->RemoveAt(j);
      }
      track_pos = good_track + 1;
    }
  }

  void UnigenEventInterface::CopyData(Hal::EventInterface* s) {
#ifdef UNIGEN_OLD
    CopyUnigen(((HalCbmUnigenEventInterface*) s)->fEvent, fEvent);
#else
    *fEvent = *((UnigenEventInterface*) s)->fEvent;
#endif
  }

  void UnigenEventInterface::CopyAndCompress(Hal::EventInterface* s, Int_t* map, Int_t map_size) {
    UnigenEventInterface* ev = (UnigenEventInterface*) s;
    fEvent->SetB(ev->fEvent->GetB());
    fEvent->SetPhi(ev->fEvent->GetPhi());
    fEvent->SetNes(ev->fEvent->GetNes());
    fEvent->SetStepNr(ev->fEvent->GetStepNr());
    fEvent->SetStepT(ev->fEvent->GetStepT());
#ifdef UNIGEN_OLD
    fEvent->GetParticleList()->Clear();
#else
    TString comment;
    ev->fEvent->GetComment(comment);
    fEvent->SetComment(comment);
    fEvent->Clear();
#endif
    for (int i = 0; i < map_size; i++) {
      fEvent->AddParticle(*ev->fEvent->GetParticle(map[i]));
    }
  }

  void UnigenEventInterface::Boost(Double_t vx, Double_t vy, Double_t vz) {
    for (int i = 0; i < fEvent->GetNpa(); i++) {
      UParticle* p       = fEvent->GetParticle(i);
      TLorentzVector mom = p->GetMomentum();
      TLorentzVector pos = p->GetPosition();
      mom.Boost(vx, vy, vz);
      pos.Boost(vx, vy, vz);
      p->SetMomentum(mom);
      p->SetPosition(pos);
    }
  }

  UnigenEventInterface::~UnigenEventInterface() {
    if (fCanDeleteEvent && fEvent) {
      if (fEvent) delete fEvent;
    }
  }

  void UnigenEventInterface::Register(Bool_t write) {
    if (fEvent == nullptr) fEvent = new UEvent();
    Hal::DataManager* manager = Hal::DataManager::Instance();
    manager->Register("UEvent.", "", (TNamed*) fEvent, write);
  }

  void UnigenEventInterface::FillTrackInterface(Hal::TrackInterface* track, Int_t index) {
    track->SetRawTrack(fEvent->GetParticle(index));
  }

  void UnigenEventInterface::ConnectToTreeInternal(eMode mode) {
    Hal::DataManager* manager = Hal::DataManager::Instance();

    switch (mode) {
      case Hal::EventInterface::eMode::kRead: {
        std::vector<TString> names;
        names.push_back("UEvent.");
        names.push_back("UEvent");
        names.push_back("event");
        TList* l = manager->GetBranchNameList();
        for (int i = 0; i < l->GetEntries(); i++) {
          TObjString* o = (TObjString*) l->At(i);
        }
        for (auto i : names) {
          fEvent = (UEvent*) manager->GetObject(i);
          if (fEvent != nullptr) { return; }
        }
      } break;
      case Hal::EventInterface::eMode::kWrite: {
        fEvent = new UEvent();
        manager->Register("UEvent.", "unigen", (TNamed*) fEvent, kTRUE);
      } break;
      case Hal::EventInterface::eMode::kWriteVirtual: {
        fEvent = new UEvent();
        manager->Register("UEvent.", "unigen", (TNamed*) fEvent, kFALSE);
        fCanDeleteEvent = kTRUE;

      } break;
    }
  }

}  // namespace HalUni
