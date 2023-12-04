/*
 * NicaUnigen.cxx
 *
 *  Created on: 23-06-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "UnigenEvent.h"

#include "DataManager.h"
#include "McTrack.h"
#include "Track.h"
#include "UnigenEventInterface.h"

#include <RtypesCore.h>
#include <TClonesArray.h>
#include <TDatabasePDG.h>
#include <TObject.h>
#include <TParticlePDG.h>

namespace HalUni {

  UnigenEvent::UnigenEvent() : Hal::McEvent("HalUni::UnigenTrack") {}

  void UnigenEvent::Update(Hal::EventInterface* interface) {

    UEvent* temp   = ((UnigenEventInterface*) interface)->fEvent;
    fB             = temp->GetB();
    fPhi           = temp->GetPhi();
    fTotalTracksNo = temp->GetNpa();
    fTracks->Clear();
    for (int i = 0; i < fTotalTracksNo; i++) {
      UParticle* particle    = temp->GetParticle(i);
      TParticlePDG* pdg_part = fPDG->GetParticle(particle->GetPdg());
      Double_t charge        = 0;
      if (pdg_part) { charge = pdg_part->Charge() / 3.0; }
      Hal::McTrack* target_track = (Hal::McTrack*) fTracks->ConstructedAt(i);
      target_track->ResetTrack(i, this);
      target_track->SetCharge(charge);
      target_track->SetPdg(particle->GetPdg());
      if (particle->GetParent() < 0) {
        target_track->SetPrimary();
      } else {
        target_track->SetMotherIndex(particle->GetParent());
      }
      target_track->SetMomentum(particle->Px(), particle->Py(), particle->Pz(), particle->E());
      target_track->SetFreezoutPosition(particle->X(), particle->Y(), particle->Z(), particle->T());
      target_track->SetStatus(particle->GetStatus());
    }
  }

  void UnigenEvent::Clear(Option_t* opt) { Hal::McEvent::Clear(opt); }

  UnigenEvent::UnigenEvent(const UnigenEvent& other) : Hal::McEvent(other) {}

  Hal::EventInterface* UnigenEvent::CreateInterface() const { return new UnigenEventInterface(); }

  UnigenEvent::~UnigenEvent() {}

  TString UnigenEvent::GetFormatName() const { return "UnigenFormat"; }

  Bool_t UnigenEvent::ExistInTree() const {
    Hal::DataManager* manager = Hal::DataManager::Instance();
    if (manager->CheckBranch("UEvent.")) { return kTRUE; };
    if (manager->CheckBranch("UEvent")) { return kTRUE; };
    if (manager->CheckBranch("events")) { return kTRUE; };
    if (manager->CheckBranch("event")) { return kTRUE; };
    return kFALSE;
  }
}  // namespace HalUni
