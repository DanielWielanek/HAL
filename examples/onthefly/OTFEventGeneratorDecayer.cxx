/*
 * OTFGeneratorDecayer.cxx
 *
 *  Created on: 18 mar 2025
 *      Author: daniel
 */

#include "OTFEventGeneratorDecayer.h"

#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TParticlePDG.h>
#include <TRandom.h>
#include <TString.h>
#include <vector>

#include "Cout.h"
#include "McTrack.h"
#include "OTFData.h"
#include "Std.h"
#include "Track.h"

namespace HalOTF {

  void EventGeneratorDecayer::GenerateEvent() {
    TDatabasePDG* db = TDatabasePDG::Instance();
    auto makeReco    = [&](OTF::McTrack& tr) {
      OTF::RecoTrack rtr;
      double px  = tr.GetMomentum().Px();
      double py  = tr.GetMomentum().Py();
      double pz  = tr.GetMomentum().Pz();
      px         = px + gRandom->Gaus(0, fSmear) * px;
      py         = py + gRandom->Gaus(0, fSmear) * py;
      pz         = pz + gRandom->Gaus(0, fSmear) * pz;
      Double_t e = TMath::Sqrt(px * px + py * py + pz * pz + fMass * fMass);
      rtr.SetMom(px, py, pz, e);
      rtr.SetNHits(5);

      auto pid = db->GetParticle(tr.GetPdgCode());
      if (pid) {
        rtr.SetCharge(pid->Charge() / 3.0);
      } else {
        rtr.SetCharge(0);
      }
      return rtr;
    };
    auto makeSim = [](Hal::McTrack tr) {
      OTF::McTrack trx;
      trx.SetMomentum(tr.GetMomentum());
      trx.SetFreezeout(tr.GetFreezeoutPosition());
      trx.SetPdgCode(tr.GetPdg());
      trx.SetMotherId(tr.GetMotherIndex());
      return trx;
    };
    int tracks = fMcEvent->GetNTracks();
    for (int i = 0; i < tracks; i++) {
      auto track = fMcEvent->GetTrack(i);
      if (track->GetPdgCode() != fDecayer->GetMotherPdg()) continue;  // stable particle
      Hal::McTrack mommy;
      mommy.SetMomentum(track->GetMomentum().X(), track->GetMomentum().Y(), track->GetMomentum().Z(), track->GetMomentum().T());
      mommy.SetFreezeoutPosition(
        track->GetFreezeout().X(), track->GetFreezeout().Y(), track->GetFreezeout().Z(), track->GetFreezeout().T());
      mommy.SetPdg(track->GetPdgCode());
      int nDau = fDecayer->DecayParticle(mommy, fDaughters, kFALSE);

      for (int j = 0; j < nDau; j++) {
        auto dau = fDaughters[j];
        dau->SetMotherIndex(i);
        auto daughter = makeSim(*dau);
        fMcEvent->AddTrack(daughter);
        auto reco = makeReco(daughter);
        reco.SetMcIndex(fMcEvent->GetNTracks() - 1);
        fRecoEvent->AddTrack(reco);
      }
    }
  }

  void EventGeneratorDecayer::SetDecay(Hal::Decay decay) {
    if (fDecayer) delete fDecayer;
    fDecayer = new Hal::Decay(decay);
  }

  Bool_t EventGeneratorDecayer::Init() {
    if (!fDecayer) {
      Hal::Cout::PrintInfo("Lack of decayer", Hal::EInfo::kError);
      return kFALSE;
    }
    auto stat = fDecayer->Init();
    if (!stat) {
      Hal::Cout::PrintInfo("Cannot initialize decayer", Hal::EInfo::kError);
      return kFALSE;
    }
    for (int i = 0; i < 3; i++) {
      fDaughters.push_back(new Hal::McTrack());
    }
    return kTRUE;
  }

  EventGeneratorDecayer::~EventGeneratorDecayer() {
    if (fDecayer) delete fDecayer;
  }

} /* namespace HalOTF */
