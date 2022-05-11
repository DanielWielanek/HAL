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

namespace Hal{

TrackPdgBinCut::TrackPdgBinCut() : TrackCut(1) { SetUnitName("Pdg type [int["); }

Bool_t TrackPdgBinCut::Pass(Track* track) {
  McTrack* tr = (McTrack*) track;
  Int_t val       = 0;
  Int_t pdg       = tr->GetPdg();
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
  SetValue(val);
  return Validate();
}

TrackPdgBinCut::~TrackPdgBinCut() {
  // TODO Auto-generated destructor stub
}

std::vector<std::pair<TString, Double_t>> TrackPdgBinCut::GetBinLabels(Int_t par) const {
  std::vector<std::pair<TString, Double_t>> res;
  res.push_back(std::pair<TString, Double_t>("neutral", 0));
  res.push_back(std::pair<TString, Double_t>("pos", 1));
  res.push_back(std::pair<TString, Double_t>("e^{+}", 2));
  res.push_back(std::pair<TString, Double_t>("#pi^{+}", 3));
  res.push_back(std::pair<TString, Double_t>("K^{+}", 4));
  res.push_back(std::pair<TString, Double_t>("p", 5));
  res.push_back(std::pair<TString, Double_t>("#mu^{+}", 6));
  res.push_back(std::pair<TString, Double_t>("unknown", 7));
  res.push_back(std::pair<TString, Double_t>("neg", -1));
  res.push_back(std::pair<TString, Double_t>("e^{-}", -2));
  res.push_back(std::pair<TString, Double_t>("#pi^{-}", -3));
  res.push_back(std::pair<TString, Double_t>("K^{-}", -4));
  res.push_back(std::pair<TString, Double_t>("#bar{p}", -5));
  res.push_back(std::pair<TString, Double_t>("#mu^{-}", -6));

  return res;
}
}
