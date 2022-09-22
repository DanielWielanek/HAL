/*
 * TrackYCut.cpp
 *
 *  Created on: 03-11-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackYCut.h"

#include "Cut.h"
#include "Package.h"
#include "Parameter.h"
#include "Track.h"

#include <RtypesCore.h>
#include <TDatabasePDG.h>
#include <TMath.h>

namespace Hal {

  TrackYCut::TrackYCut() : TrackCut(1) { SetUnitName("y"); }

  Bool_t TrackYCut::Pass(Track* track) {
    Double_t pz = track->GetPz();
    Double_t e  = track->GetE();
    Double_t y  = 0.5 * TMath::Log((e + pz) / (e - pz));
    SetValue(y);
    return Validate();
  }

  TrackYCut::~TrackYCut() {
    // TODO Auto-generated destructor stub
  }

  Bool_t TrackYAssumedCut::Pass(Track* track) {
    Double_t p = track->GetMomentum().P();
    Double_t E = TMath::Sqrt(p * p + fMass);
    Double_t y = 0.5 * TMath::Log((E + p) / (E - p));
    SetValue(y);
    return Validate();
  }

  void TrackYAssumedCut::SetPid(Int_t pid) {
    TDatabasePDG* pdg  = TDatabasePDG::Instance();
    TParticlePDG* part = pdg->GetParticle(pid);
    if (part) {
      fMass = part->Mass();
      fMass = fMass * fMass;
      fPdg  = pid;
    } else {
      fMass = -1;
    }
  }

  Bool_t TrackYAssumedCut::Init(Int_t taskId) {
    if (fMass < 0) return kFALSE;
    return TrackCut::Init(taskId);
  }

  Package* TrackYAssumedCut::Report() const {
    Package* pack = TrackCut::Report();
    pack->AddObject(new Hal::ParameterInt("PID", fPdg));
    return pack;
  }

}  // namespace Hal
