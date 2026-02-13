/*
 * OTFMomentumSmearing.cxx
 *
 *  Created on: 13 lut 2026
 *      Author: daniel
 */

#include "OTFMomentumSmearing.h"

#include <RtypesCore.h>
#include <TString.h>

#include "ComplexTrack.h"
#include "Cout.h"
#include "LorentzSmearing.h"
#include "StdTypes.h"

namespace HalOTF {

  void MomentumSmearing::SetSmearingAlgo(const Hal::LorentzSmearing& algo) {
    if (fSmearing) delete fSmearing;
    fSmearing = algo.MakeCopy();
  }

  void MomentumSmearing::ProcessTrack(Hal::ComplexTrack* track) {
    auto simtrack  = track->GetImgTrack();
    auto recotrack = track->GetRealTrack();
    TLorentzVector result;
    switch (fMode) {
      case eMode::kReco: {
        fSmearing->Smear(result, recotrack->GetMomentum());
        simtrack->SetMomentum(result);
      } break;
      case eMode::kSim: {
        fSmearing->Smear(result, simtrack->GetMomentum());
        recotrack->SetMomentum(result);
      } break;
    }
  }

  Bool_t MomentumSmearing::Init() {
    if (!fSmearing) {
      Hal::Cout::PrintInfo("MomentumSmearing - no smearing algo!", Hal::EInfo::kError);
      return false;
    }
    return fSmearing->Init();
  }

  MomentumSmearing::~MomentumSmearing() {
    if (fSmearing) delete fSmearing;
  }

} /* namespace HalOTF */
