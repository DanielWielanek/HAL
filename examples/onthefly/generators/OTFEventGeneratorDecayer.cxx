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
    int tracks       = fMcEvent->GetNTracks();
    for (int i = 0; i < tracks; i++) {
      auto track = fMcEvent->GetTrack(i);
      if (track->GetPdgCode() != fDecayer->GetMotherPdg()) continue;  // stable particle
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
