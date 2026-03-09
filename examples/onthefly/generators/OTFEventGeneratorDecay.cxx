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
#include "OTFData.h"
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
    int indexCount   = 0;

    for (int i = start; i < shift; i++) {
      auto track         = fMcEvent->GetTrack(i);
      Hal::McTrack mommy = OTF::Std::OTFMcToHalMc(*track);
      int nDau           = fDecayer->DecayParticle(mommy, fDaughters, kFALSE);

      for (int j = 0; j < nDau; j++) {
        auto dau = fDaughters[j];
        dau->SetMotherIndex(i);
        auto daughter = OTF::Std::HalMcToOTFMc(*dau);
        daughter.SetGeneratorId(track->GetGeneratorId());
        fMcEvent->AddTrack(daughter);
        auto reco = OTF::Std::HalMcToOTFReco(daughter);
        reco.SetMcIndex(fMcEvent->GetNTracks() - 1);
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
