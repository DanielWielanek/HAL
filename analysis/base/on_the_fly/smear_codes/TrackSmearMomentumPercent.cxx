/*
 * TrackSmearMomentumPercent.cxx
 *
 *  Created on: 8 gru 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackSmearMomentumPercent.h"

#include <TLorentzVector.h>
#include <TMath.h>
#include <TRandom.h>

#include "ComplexTrack.h"
#include "McTrack.h"
#include "Package.h"
#include "Parameter.h"
#include "Track.h"


namespace Hal {
  TrackSmearMomentumPercent::TrackSmearMomentumPercent() :
    TrackSmear(), fPxSmear(0.0), fPySmear(0.0), fPzSmear(0.0), fPtSmear(0.0), fSmearMode(0) {}

  void TrackSmearMomentumPercent::SetMomentumSmear(Double_t pt) {
    fPtSmear = pt;
    fPzSmear = 0.0;
    fPxSmear = 0.0;
    fPySmear = 0.0;
  }

  void TrackSmearMomentumPercent::SetMomentumSmear(Double_t pt, Double_t pz) {
    fPtSmear = pt;
    fPzSmear = pz;
    fPxSmear = fPySmear = 0.0;
  }

  void TrackSmearMomentumPercent::SetMomentumSmear(Double_t px, Double_t py, Double_t pz) {
    fPtSmear = 0.0;
    fPxSmear = px;
    fPySmear = py;
    fPzSmear = pz;
  }

  Package* TrackSmearMomentumPercent::Report() const {
    Package* pack = TrackSmear::Report();
    Double_t px_smear;
    Double_t py_smear;
    if (fSmearMode == 1 || fSmearMode == 2) {
      px_smear = 0.0;
      py_smear = 0.0;
    } else {
      px_smear = fPxSmear;
      py_smear = fPySmear;
    }
    pack->AddObject(new ParameterDouble("Smear of px (%)", px_smear * 100.0));
    pack->AddObject(new ParameterDouble("Smear of py (%)", py_smear * 100.0));
    pack->AddObject(new ParameterDouble("Smear of pz (%)", fPzSmear * 100.0));
    pack->AddObject(new ParameterDouble("Smear of pt (%)", fPtSmear));
    return pack;
  }

  void TrackSmearMomentumPercent::ModifyTrack(ComplexTrack* mod) {
    Double_t px, py, pz, e;
    Hal::McTrack* mc = (Hal::McTrack*) mod->GetImgTrack();
    px               = mc->GetMomentum().Px();
    py               = mc->GetMomentum().Py();
    pz               = mc->GetMomentum().Pz();
    e                = mc->GetMomentum().E();
    Double_t m2      = e * e - px * px - py * py - pz * pz;
    switch (fSmearMode) {
      case 1:  // pt only
      {
        px = px * (1. + gRandom->Gaus(0, fPxSmear));
        py = py * (1. + gRandom->Gaus(0, fPySmear));
      } break;
      default: {
        px = px * (1. + gRandom->Gaus(0, fPxSmear));
        py = py * (1. + gRandom->Gaus(0, fPySmear));
        pz = pz * (1. + gRandom->Gaus(0, fPzSmear));
      } break;
    }
    e = TMath::Sqrt(m2 + px * px + py * py + pz * pz);
    mod->SetMomentum(px, py, pz, e);
    mod->GetRealTrack()->SetMomentum(px, py, pz, e);
  }

  Task::EInitFlag TrackSmearMomentumPercent::Init() {
    if (fPtSmear != 0) fSmearMode = 1;
    if (fPzSmear != 0) fSmearMode = 2;
    if (fPxSmear != 0) fSmearMode = 3;
    switch (fSmearMode) {
      case 1:  // pt only
      {
        fPxSmear = fPtSmear / TMath::Sqrt(2.0) / 100.0;
        fPySmear = fPtSmear / TMath::Sqrt(2.0) / 100.0;
      } break;
      case 2:  // pt and pz
      {
        fPxSmear = fPtSmear / TMath::Sqrt(2.0) / 100.0;
        fPySmear = fPtSmear / TMath::Sqrt(2.0) / 100.0;
      } break;
      case 3: {
        fPxSmear = fPxSmear / 100.0;
        fPySmear = fPySmear / 100.0;
        fPzSmear = fPzSmear / 100.0;
      } break;
    }
    return Task::EInitFlag::kSUCCESS;
  }

  TrackSmearMomentumPercent::TrackSmearMomentumPercent(const TrackSmearMomentumPercent& obj) :
    TrackSmear(obj),
    fPxSmear(obj.fPxSmear),
    fPySmear(obj.fPySmear),
    fPzSmear(obj.fPzSmear),
    fPtSmear(obj.fPtSmear),
    fSmearMode(obj.fSmearMode) {}

  TrackSmearMomentumPercent::~TrackSmearMomentumPercent() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
