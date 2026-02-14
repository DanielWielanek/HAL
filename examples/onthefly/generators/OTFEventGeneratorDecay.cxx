/*
 * OTFReaderDecay.cxx
 *
 *  Created on: 28 mar 2024
 *      Author: daniel
 */

#include "OTFEventGeneratorDecay.h"

#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TParticlePDG.h>
#include <TRandom.h>
#include <TString.h>
#include <iostream>

#include "Cout.h"
#include "McTrack.h"
#include "OTFEventGenerator.h"
#include "Std.h"
#include "Track.h"


namespace HalOTF {
  EventGeneratorDecay::EventGeneratorDecay() {}

  Bool_t EventGeneratorDecay::Init() {
    auto init = HalOTF::EventGenerator::Init();
    if (!init) return init;
    if (!fDecayer) {
      Hal::Cout::PrintInfo("Lack of decayer", Hal::EInfo::kError);
      return kFALSE;
    }
    auto stat = fDecayer->Init();
    if (fDecayer->GetMotherPdg() != fPids) {
      Hal::Cout::PrintInfo(Form("Wrong decayer, expected PID=%i got %i", fPids, fDecayer->GetMotherPdg()), Hal::EInfo::kError);
      return kFALSE;
    }
    if (!stat) {
      Hal::Cout::PrintInfo("Cannot initialize decayer", Hal::EInfo::kError);
      return kFALSE;
    }
    for (int i = 0; i < 3; i++) {
      fDaughters.push_back(new Hal::McTrack());
    }
    return init;
  }

  void EventGeneratorDecay::SetDecay(Hal::Decay decay) {
    if (fDecayer) delete fDecayer;
    fDecayer = new Hal::Decay(decay);
  }

  void EventGeneratorDecay::Decay() {
    Int_t shift      = fMcEvent->GetNTracks();
    Int_t start      = shift - fCurrrentMult;
    TDatabasePDG* db = TDatabasePDG::Instance();

    auto makeReco = [&](OTF::McTrack& tr) {
      OTF::RecoTrack rtr;
      double px  = tr.GetMomentum().Px();
      double py  = tr.GetMomentum().Py();
      double pz  = tr.GetMomentum().Pz();
      Double_t e = TMath::Sqrt(px * px + py * py + pz * pz + fMass * fMass);
      rtr.SetMom(px, py, pz, e);
      rtr.SetNHitsA(2);
      rtr.SetNHitsB(2);
      rtr.SetNHitsC(2);
      rtr.SetPidHypo(tr.GetPdgCode());

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

    int indexCount = 0;

    for (int i = start; i < shift; i++) {
      auto track = fMcEvent->GetTrack(i);
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
        daughter.SetGeneratorId(track->GetGeneratorId());
        fMcEvent->AddTrack(daughter);
        auto reco = makeReco(daughter);
        reco.SetGeneratorId(track->GetGeneratorId());
        reco.SetMcIndex(fMcEvent->GetNTracks() - 1);
        reco.SetPidHypo(dau->GetPdg());
        reco.SetNHitsA(2);
        reco.SetNHitsB(2);
        reco.SetNHitsC(2);
        fRecoEvent->AddTrack(reco);
      }
    }
  }

  EventGeneratorDecay::~EventGeneratorDecay() {
    for (auto i : fDaughters)
      delete i;
    if (fDecayer) delete fDecayer;
  }

  void EventGeneratorDecay::GenerateEvent() {
    HalOTF::EventGenerator::GenerateEvent();
    Decay();
    if (fDebug) {
      std::cout << "MC Tracks" << std::endl;
      Hal::Cout::Database({"Idx", "Px", "Pid", "MomId"});
      for (int i = 0; i < fMcEvent->GetNTracks(); i++) {
        auto mcTrack = fMcEvent->GetTrack(i);
        Hal::Cout::Database({Form("%i", i),
                             Form("%4.2f", mcTrack->GetMomentum().Px()),
                             Form("%i", mcTrack->GetPdgCode()),
                             Form("%i", mcTrack->GetMotherId())});
      }
      std::cout << "Reco Tracks" << std::endl;
      Hal::Cout::Database({"Idx", "Px(reco)", "Px(sim)", "Pid", "McId"});
      for (int i = 0; i < fRecoEvent->GetNTracks(); i++) {
        auto recoTrack = fRecoEvent->GetTrack(i);
        //  std::cout << recoTrack->GetMcIndex() << std::endl;
        auto simTrack = fMcEvent->GetTrack(recoTrack->GetMcIndex());

        Hal::Cout::Database({Form("%i", i),
                             Form("%4.2f", recoTrack->GetMom().Px()),
                             Form("%4.2f", simTrack->GetMomentum().Px()),
                             Form("%i", simTrack->GetPdgCode()),
                             Form("%i", recoTrack->GetMcIndex())});
      }
      std::cout << "-----" << std::endl;
    }
  }

}  // namespace HalOTF
