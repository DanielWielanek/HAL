/*
 * TrackTPC.cxx
 *
 *  Created on: 28 mar 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TpcTrack.h"
namespace Hal {
  TpcTrack::TpcTrack() :
    fTpcNHits(0),
    fTpcSigmaPion(0),
    fTpcSigmaKaon(0),
    fTpcSigmaProton(0),
    fTpcSigmaElectron(0),
    fTpcPidProbPion(0),
    fTpcPidProbKaon(0),
    fTpcPidProbProton(0),
    fTpcPidProbElectron(0),
    fTpcDeDx(0) {}

  TpcTrack::~TpcTrack() {}

  void TpcTrack::SetSigma(Float_t pion, Float_t kaon, Float_t proton, Float_t electron) {
    fTpcSigmaPion     = pion;
    fTpcSigmaKaon     = kaon;
    fTpcSigmaProton   = proton;
    fTpcSigmaElectron = electron;
  }

  void TpcTrack::SetPidProb(Float_t pion, Float_t kaon, Float_t proton, Float_t electron) {
    fTpcPidProbPion     = pion;
    fTpcPidProbKaon     = kaon;
    fTpcPidProbProton   = proton;
    fTpcPidProbElectron = electron;
  }
}  // namespace Hal
