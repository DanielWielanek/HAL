/*
 * TwoTrackAncestorBinCut.cxx
 *
 *  Created on: 27 mar 2026
 *      Author: daniel
 */

#include "TwoTrackAncestorPdgBinCut.h"

#include "Cut.h"
#include "Event.h"
#include "McTrack.h"
#include "Package.h"
#include "Parameter.h"
#include "Track.h"
#include "TwoTrack.h"

namespace Hal {

  TwoTrackAncestorPdgBinCut::TwoTrackAncestorPdgBinCut(Int_t pid) : TwoTrackMcCut(2) {
    SetUnitName("First particle [AU]", 0);
    SetUnitName("Second particle [AU]", 1);
    SafelyAddToMap(0, "primary");
    SafelyAddToMap(1, "unkown");
    SafelyAddToMap(2, "bad");
    if (pid == Hal::Const::ProtonPID()) {
      SafelyAddToMap(Hal::Const::LambdaPID(), "");
      SafelyAddToMap(Hal::Const::SigmaPlusPID(), "");
      SafelyAddToMap(Hal::Const::SigmaZeroPID(), "");
      SafelyAddToMap(Hal::Const::XiMinusPID(), "");
      SafelyAddToMap(Hal::Const::XiZeroPID(), "");
    }
    if (pid == -Hal::Const::ProtonPID()) {
      SafelyAddToMap(-Hal::Const::LambdaPID(), "");
      SafelyAddToMap(-Hal::Const::SigmaPlusPID(), "");
      SafelyAddToMap(-Hal::Const::SigmaZeroPID(), "");
      SafelyAddToMap(-Hal::Const::XiMinusPID(), "");
      SafelyAddToMap(-Hal::Const::XiZeroPID(), "");
    }
  }

  Bool_t TwoTrackAncestorPdgBinCut::Pass(TwoTrack* pair) {
    McTrack* track1 = (McTrack*) pair->GetTrack1();
    McTrack* track2 = (McTrack*) pair->GetTrack2();
    SetValue(GetPid(track1), 0);
    SetValue(GetPid(track2), 1);
    return Validate();
  }

  std::vector<std::pair<TString, Double_t>> TwoTrackAncestorPdgBinCut::GetBinLabels(Int_t /*int1*/) const {
    std::vector<std::pair<TString, Double_t>> res;
    for (int i = 0; i < fNames.size(); i++) {
      res.push_back(std::pair<TString, Double_t>(fNames[i], fIndexes[i]));
    }
    return res;
  }

  void TwoTrackAncestorPdgBinCut::SafelyAddToMap(Int_t pid, TString name) {
    auto it = fMap.find(pid);
    if (it != fMap.end()) { return; }
    int idx   = fMap.size();
    fMap[pid] = idx;
    if (name.Length()) {
      fNames.push_back(name);
      fIndexes.push_back(idx);
    } else {
      name = Hal::Const::PdgToName(pid);
      fNames.push_back(name);
      fIndexes.push_back(idx);
    }
  }

  Hal::Package* TwoTrackAncestorPdgBinCut::Report() const {
    auto report = TwoTrackCut::Report();
    for (unsigned int i = 0; i < fIndexes.size(); i++) {
      report->AddObject(new Hal::ParameterInt(fNames[i], i));
    }
    return report;
  }

  Int_t TwoTrackAncestorPdgBinCut::GetPid(Hal::Track* track) {
    McTrack* tr = (McTrack*) track;
    if (tr->IsPrimary()) return 0;
    int motherId = tr->GetMotherIndex();
    if (motherId < 0) return 2;
    McTrack* mother = nullptr;
    do {
      mother   = (McTrack*) tr->GetEvent()->GetTrack(motherId);
      motherId = mother->GetMotherIndex();
      if (motherId < 0 && !mother->IsPrimary()) return 2;
    } while (!mother->IsPrimary());

    int pdg = mother->GetPdg();
    auto it = fMap.find(pdg);
    if (it == fMap.end()) {
      return 1;
    } else {
      return fMap[pdg];
    }
  }


} /* namespace Hal */
