/*
 * TrackPdgBinCut.cxx
 *
 *  Created on: 27 gru 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackPdgBinCut.h"

#include "McTrack.h"
#include "Package.h"
#include "Parameter.h"

namespace Hal {

  TrackPdgBinCut::TrackPdgBinCut(Bool_t autoload) : TrackCut(1) {
    SetUnitName("Pdg type [int]");
    SafelyAddToMap(81, "other neutral");
    SafelyAddToMap(82, "other positive");
    SafelyAddToMap(83, "other negative");
    SafelyAddToMap(84, "unknown");
    if (autoload) {
      SafelyAddToMap(Hal::Const::PionPlusPID(), "");
      SafelyAddToMap(-Hal::Const::PionPlusPID(), "");
      SafelyAddToMap(Hal::Const::KaonPlusPID(), "");
      SafelyAddToMap(-Hal::Const::KaonPlusPID(), "");
      SafelyAddToMap(Hal::Const::ProtonPID(), "");
      SafelyAddToMap(-Hal::Const::ProtonPID(), "");
      SafelyAddToMap(Hal::Const::ElectronPID(), "");
      SafelyAddToMap(-Hal::Const::ElectronPID(), "");
      SafelyAddToMap(Hal::Const::MuonPID(), "");
      SafelyAddToMap(-Hal::Const::MuonPID(), "");
    }
  }

  Bool_t TrackPdgBinCut::Pass(Track* track) {
    McTrack* tr      = (McTrack*) track;
    Int_t val        = 0;
    Int_t pdg        = tr->GetPdg();
    Int_t index_flag = 0;
    auto it          = fMap.find(pdg);
    if (it == fMap.end()) {
      index_flag = -1;
    } else {
      index_flag = fMap[pdg];
    }
    if (index_flag == -1) {  // unknown particle
      Int_t charge = track->GetCharge();
      switch (charge) {
        case 0: {
          val = fMap[81];
        } break;
        case 1: {
          val = fMap[82];
        } break;
        case -1: {
          val = fMap[83];
        } break;
        default: {
          val = fMap[84];
        } break;
      }
    } else {  // known particle
      val = index_flag;
    }
    SetValue(val);
    return Validate();
  }

  std::vector<std::pair<TString, Double_t>> TrackPdgBinCut::GetBinLabels(Int_t /*par*/) const {
    std::vector<std::pair<TString, Double_t>> res;
    for (int i = 0; i < (int) fNames.size(); i++) {
      res.push_back(std::pair<TString, Double_t>(fNames[i], fIndexes[i]));
    }
    return res;
  }

  void TrackPdgBinCut::SafelyAddToMap(Int_t pid, TString name) {
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

  Hal::Package* TrackPdgBinCut::Report() const {
    auto report = TrackCut::Report();
    for (unsigned int i = 0; i < fIndexes.size(); i++) {
      report->AddObject(new Hal::ParameterInt(fNames[i], i));
    }

    return report;
  }

}  // namespace Hal
