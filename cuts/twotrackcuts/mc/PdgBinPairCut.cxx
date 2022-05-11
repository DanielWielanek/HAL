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
#include "Track.h"
#include "TwoTrack.h"

#include <utility>
#include <vector>

#include <RtypesCore.h>
#include <TString.h>
namespace Hal {
  PdgBinPairCut::PdgBinPairCut() : TwoTrackCut(2) {
    SetUnitName("First paritcle [AU]", 0);
    SetUnitName("Second paritcle [AU]", 1);
  }

  Bool_t PdgBinPairCut::Pass(TwoTrack* pair) {
    McTrack* track1 = (McTrack*) pair->GetTrack1();
    McTrack* track2 = (McTrack*) pair->GetTrack2();
    SetValue(PidToID(track1), 0);
    SetValue(PidToID(track2), 1);
    return Validate();
  }

  Int_t PdgBinPairCut::PidToID(McTrack* track) const {
    Int_t val = 0;
    Int_t pdg = track->GetPdg();
    switch (pdg) {
      case -11: val = 2; break;
      case 211: val = 3; break;
      case 321: val = 4; break;
      case 2212: val = 5; break;
      case 11: val = -2; break;
      case -211: val = -3; break;
      case -321: val = -4; break;
      case -2212: val = -5; break;
      case 13: val = -6; break;
      case -13: val = 6; break;
      default: {
        Int_t charge = track->GetCharge();
        switch (charge) {
          case 0: val = 0; break;
          case 1: val = 1; break;
          case -1: val = -1; break;
          default: val = 7; break;
        }
      } break;
    };
    return val;
  }

  PdgBinPairCut::~PdgBinPairCut() {
    // TODO Auto-generated destructor stub
  }

  std::vector<std::pair<TString, Double_t>> PdgBinPairCut::GetBinLabels(Int_t /*int1*/) const {
    std::vector<std::pair<TString, Double_t>> res;
    res.push_back(std::pair<TString, Double_t>("0", 0));
    res.push_back(std::pair<TString, Double_t>("+", 1));
    res.push_back(std::pair<TString, Double_t>("-", -1));
    res.push_back(std::pair<TString, Double_t>("unknown", 7));
    res.push_back(std::pair<TString, Double_t>("e^{+}", 2));
    res.push_back(std::pair<TString, Double_t>("#pi^{+}", 3));
    res.push_back(std::pair<TString, Double_t>("K^{+}", 4));
    res.push_back(std::pair<TString, Double_t>("p", 5));
    res.push_back(std::pair<TString, Double_t>("#mu^{+}", 6));
    res.push_back(std::pair<TString, Double_t>("e^{-}", -2));
    res.push_back(std::pair<TString, Double_t>("#pi^{-}", -3));
    res.push_back(std::pair<TString, Double_t>("K^{-}", -4));
    res.push_back(std::pair<TString, Double_t>("#bar{p}", -5));
    res.push_back(std::pair<TString, Double_t>("#mu^{-}", -6));
    return res;
  }
}  // namespace Hal
