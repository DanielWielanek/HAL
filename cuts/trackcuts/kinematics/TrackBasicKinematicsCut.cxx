/*
 * TrackBasicKinematicsCut.cxx
 *
 *  Created on: 27 mar 2026
 *      Author: daniel
 */

#include "TrackBasicKinematicsCut.h"

#include <RtypesCore.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TParticlePDG.h>

#include "Cout.h"
#include "Cut.h"
#include "StdTypes.h"
#include "Track.h"

namespace Hal {

  TrackPtYCut::TrackPtYCut() : TrackCut(2) {
    SetUnitName("p_{T} [GeV/c]", 0);
    SetUnitName("y", 1);
  }

  Bool_t TrackPtYCut::Pass(Track* track) {
    SetValue(track->GetMomentum().Pt(), 0);
    SetValue(track->GetMomentum().Rapidity(), 1);
    return Validate();
  }

  //=====================================================================

  TrackPtYAssumedCut::TrackPtYAssumedCut() : TrackCut(2) {
    SetUnitName("p_{T} [GeV/c]", 0);
    SetUnitName("y_{assum}", 1);
  }

  Bool_t TrackPtYAssumedCut::Init(Int_t int1) {
    auto pid   = TDatabasePDG::Instance();
    auto track = pid->GetParticle(fAssumedPid);
    if (!track) {
      Hal::Cout::PrintInfo("Wrong assumed PID cut in TrackPtYAssumedCut", Hal::EInfo::kError);
      return kFALSE;
    }
    fMass2 = track->Mass() * track->Mass();
    return Hal::TrackCut::Init(int1);
  }

  Bool_t TrackPtYAssumedCut::Pass(Track* track) {
    double pz = track->GetMomentum().Pz();
    double p  = track->GetMomentum().P();
    double e  = TMath::Sqrt(p * p + fMass2);
    SetValue(track->GetMomentum().Pt(), 0);
    SetValue(0.5 * TMath::Log((e + pz) / (e - pz)), 1);
    return Validate();
  }

  //=====================================================================

  TrackPtEtaCut::TrackPtEtaCut() : TrackCut(2) {
    SetUnitName("p_{T} [GeV/c]", 0);
    SetUnitName("#eta", 1);
  }


  Bool_t TrackPtEtaCut::Pass(Track* track) {
    SetValue(track->GetMomentum().Pt(), 0);
    SetValue(track->GetMomentum().Eta(), 1);
    return Validate();
  }

} /* namespace Hal */
