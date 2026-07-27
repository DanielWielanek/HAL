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
#include <algorithm>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <unordered_set>
#include <vector>

#include "Cout.h"
#include "McTrack.h"
#include "OTFData.h"
#include "Std.h"
#include "Track.h"

namespace HalOTF {

  void EventGeneratorDecayer::GenerateEvent() {
    for (auto decayer : fDecays) {
      int tracks = fMcEvent->GetNTracks();
      for (int i = 0; i < tracks; i++) {
        auto track = fMcEvent->GetTrack(i);
        if (track->GetPdgCode() == decayer->GetMotherPdg()) {
          Hal::McTrack mommy = OTF::Std::OTFMcToHalMc(*track);
          int nDau           = decayer->DecayParticle(mommy, fDaughters, kFALSE);

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
    }
    // int tracks = fMcEvent->GetNTracks();
  }

  void EventGeneratorDecayer::SetDecay(Hal::Decay decay) { fDecays.push_back(new Hal::Decay(decay)); }

  Bool_t EventGeneratorDecayer::Init() {

    for (int i = 0; i < 3; i++) {
      fDaughters.push_back(new Hal::McTrack());
    }
    for (auto x : fDecays) {
      if (!x) {
        Hal::Cout::PrintInfo("Lack of decayer", Hal::EInfo::kError);
        return kFALSE;
      }
      auto stat = x->Init();
      if (!stat) {
        Hal::Cout::PrintInfo("Cannot initialize decayer", Hal::EInfo::kError);
        return kFALSE;
      }
    }
    SortDecays();

    return kTRUE;
  }
  void EventGeneratorDecayer::SortDecays() {
    auto decays = fDecays;
    std::unordered_map<int, std::vector<int>> graph;  // mother -> daughters
    std::unordered_map<int, int> indegree;
    std::unordered_set<int> nodes;

    // --- build graph ---
    for (auto* d : decays) {
      int mother = d->GetMotherPdg();
      nodes.insert(mother);

      if (!indegree.count(mother)) indegree[mother] = 0;

      for (int dau : d->GetAllDecayDaughters()) {
        graph[dau].push_back(mother);
        indegree[mother]++;
        nodes.insert(dau);
        if (!indegree.count(dau)) indegree[dau] = 0;
      }
    }

    // --- Kahn topo sort ---
    std::queue<int> q;

    for (int n : nodes) {
      if (indegree[n] == 0) q.push(n);
    }

    std::vector<int> topo;
    topo.reserve(nodes.size());

    while (!q.empty()) {
      int n = q.front();
      q.pop();

      topo.push_back(n);

      for (int child : graph[n]) {
        if (--indegree[child] == 0) q.push(child);
      }
    }

    // --- cycle check ---
    if (topo.size() != nodes.size()) throw std::runtime_error("Cycle detected in decay graph");

    // --- reverse = correct decay execution order ---
    std::reverse(topo.begin(), topo.end());

    // --- map back to decay pointers ---
    std::unordered_map<int, Hal::Decay*> decayByMother;
    decayByMother.reserve(decays.size());

    for (auto* d : decays)
      decayByMother[d->GetMotherPdg()] = d;

    std::vector<Hal::Decay*> sorted;
    sorted.reserve(decays.size());

    for (int pdg : topo) {
      auto it = decayByMother.find(pdg);
      if (it != decayByMother.end()) sorted.push_back(it->second);
    }
    fDecays = sorted;
  }
  EventGeneratorDecayer::~EventGeneratorDecayer() {
    for (auto x : fDecays)
      delete x;
  }

} /* namespace HalOTF */
