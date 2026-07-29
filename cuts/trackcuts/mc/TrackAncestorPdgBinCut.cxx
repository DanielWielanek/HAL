/*
 * TrackMotherPdgBinCut.cxx
 *
 *  Created on: 27 mar 2026
 *      Author: daniel
 */

#include "TrackAncestorPdgBinCut.h"

#include "Const.h"
#include "Cut.h"
#include "Event.h"
#include "McTrack.h"
#include "Package.h"
#include "Parameter.h"
#include "Track.h"

namespace Hal {

  TrackAncestorPdgBinCut::TrackAncestorPdgBinCut(Int_t pdg) : TrackMcCut(1) {
    SetUnitName("Pdg type [int]");
    SafelyAddToMap(0, "primary");
    SafelyAddToMap(1, "unknown");
    SafelyAddToMap(2, "bad");
    if (pdg == Hal::Const::DummyPID()) return;
    // TODO
  }

  Bool_t TrackAncestorPdgBinCut::Pass(Track* track) {
    McTrack* tr = (McTrack*) track;
    if (tr->IsPrimary()) {
      SetValue(0);
      return Validate();
    }
    Int_t motherId = tr->GetMotherIndex();
    if (motherId < 0) {
      SetValue(2);
      return Validate();
    }
    McTrack* mother = nullptr;
    do {
      mother   = (McTrack*) tr->GetEvent()->GetTrack(motherId);
      motherId = mother->GetMotherIndex();
      if (motherId < 0 && !mother->IsPrimary()) {
        SetValue(2);
        return Validate();
      }
    } while (!mother->IsPrimary());
    int index_flag = -1;
    int pdg        = mother->GetPdg();
    auto it        = fMap.find(pdg);
    if (it == fMap.end()) {
      index_flag = 1;
    } else {
      index_flag = fMap[pdg];
    }
    SetValue(index_flag);
    return Validate();
  }

  std::vector<TString> TrackAncestorPdgBinCut::GetAxisBinLabels(Int_t /*par*/) const {
    std::vector<TString> res;
    for (int i = 0; i < (int) fNames.size(); i++) {
      res.push_back(fNames[i]);
    }
    return res;
  }

  void TrackAncestorPdgBinCut::SafelyAddToMap(Int_t pid, TString name) {
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

  Hal::Package* TrackAncestorPdgBinCut::Report() const {
    auto report = TrackCut::Report();
    for (unsigned int i = 0; i < fIndexes.size(); i++) {
      report->AddObject(new Hal::ParameterInt(fNames[i], i));
    }

    return report;
  }


} /* namespace Hal */
