/*
 * PdgBinPairCut.cxx
 *
 *  Created on: 2 sie 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "PdgBinPairCut.h"

#include "Cut.h"
#include "Package.h"
#include "Parameter.h"
#include "Track.h"
#include "TwoTrack.h"

#include <utility>
#include <vector>

#include <RtypesCore.h>
#include <TString.h>
namespace Hal {
  PdgBinPairCut::PdgBinPairCut(Bool_t autoload) : TwoTrackCut(2) {
    SetUnitName("First paritcle [AU]", 0);
    SetUnitName("Second paritcle [AU]", 1);
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

  Bool_t PdgBinPairCut::Pass(TwoTrack* pair) {
    McTrack* track1 = (McTrack*) pair->GetTrack1();
    McTrack* track2 = (McTrack*) pair->GetTrack2();
    SetValue(GetPid(track1), 0);
    SetValue(GetPid(track2), 1);
    return Validate();
  }

  PdgBinPairCut::~PdgBinPairCut() {
    // TODO Auto-generated destructor stub
  }

  std::vector<std::pair<TString, Double_t>> PdgBinPairCut::GetBinLabels(Int_t /*int1*/) const {
    std::vector<std::pair<TString, Double_t>> res;
    for (int i = 0; i < fNames.size(); i++) {
      res.push_back(std::pair<TString, Double_t>(fNames[i], fIndexes[i]));
    }
    return res;
  }

  void PdgBinPairCut::SafelyAddToMap(Int_t pid, TString name) {
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

  Hal::Package* PdgBinPairCut::Report() const {
    auto report = TwoTrackCut::Report();
    for (unsigned int i = 0; i < fIndexes.size(); i++) {
      report->AddObject(new Hal::ParameterInt(fNames[i], i));
    }
    return report;
  }

  Int_t PdgBinPairCut::GetPid(Hal::Track* track) {
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
    return val;
  }

}  // namespace Hal
