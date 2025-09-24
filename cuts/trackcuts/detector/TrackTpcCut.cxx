/*
 * TrackTpcBasicCut.cxx
 *
 *  Created on: 21 cze 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackTpcCut.h"


#include <TString.h>

#include "Cout.h"
#include "Cut.h"
#include "DataFormat.h"
#include "ExpTrack.h"
#include "Package.h"
#include "Parameter.h"
#include "StdString.h"
#include "TpcTrack.h"
#include "Track.h"

namespace Hal {


  TrackTpcCut::TrackTpcCut() : TrackExpCut(7) {
    SetUnitName("TpcHits [N]", TpcHits);
    SetUnitName("TpcCharge [q]", Charge);
    SetUnitName("#sigma#pi", SigmaPion);
    SetUnitName("#sigmaK", SigmaKaon);
    SetUnitName("#sigmaP", SigmaProton);
    SetUnitName("#sigmae", SigmaElectron);
    SetUnitName("dEdX [kV/cm]", DeDx);
    fParticleType = SigmaPion;
    SetMinMax(-1E+29, 1E+29, SigmaPion);
    SetMinMax(-1E+29, 1E+29, SigmaKaon);
    SetMinMax(-1E+29, 1E+29, SigmaProton);
    SetMinMax(-1E+29, 1E+29, SigmaElectron);
    SetMinMax(0, 1E+29, DeDx);
    SetMinMax(-1, 1, Charge);
    SetMinMax(0, 90, TpcHits);
    fMode = kNotBad;
  }

  Bool_t TrackTpcCut::Init(Int_t task_id) {
    Bool_t stat = TrackExpCut::Init(task_id);
    if (stat == kFALSE) return kFALSE;
    if (TpcAvaiable(task_id)) return kTRUE;
    return kFALSE;
  }

  void TrackTpcCut::SetSigma(Double_t min, Double_t max, TString opt) {
    if (Hal::Std::FindParam(opt, "pi")) {
      SetMinMax(min, max, SigmaPion);
    } else if (Hal::Std::FindParam(opt, "K")) {
      SetMinMax(min, max, SigmaKaon);
    } else if (Hal::Std::FindParam(opt, "p")) {
      SetMinMax(min, max, SigmaProton);
    } else if (Hal::Std::FindParam(opt, "e")) {
      SetMinMax(min, max, SigmaElectron);
    } else {
      Cout::Text("Wrong SetSigma flag please use pi/K/p/e", "L", kOrange);
    }
  }

  void TrackTpcCut::SetCharge(Int_t i) { SetMinAndMax(i, Charge); }

  void TrackTpcCut::SetNHits(Int_t min, Int_t max) { SetMinMax(min, max, TpcHits); }

  Package* TrackTpcCut::Report() const {
    Package* pack = TrackCut::Report();
    TString sigma_name;
    switch (fParticleType) {
      case SigmaPion: sigma_name = "SigmaPion"; break;
      case SigmaKaon: sigma_name = "SigmaKaon"; break;
      case SigmaProton: sigma_name = "SigmaProton"; break;
      case SigmaElectron: sigma_name = "SigmaElectron"; break;
      default: sigma_name = "UnknownSigma"; break;
    }
    ParameterString* str = new ParameterString("ActiveSigma", sigma_name);
    pack->AddObject(str);
    switch (fMode) {
      case kNotBad: {
        ParameterString* stt = new ParameterString("Mode", "NotBad");
        pack->AddObject(stt);
      } break;
      case kGood: {
        ParameterString* stt = new ParameterString("Mode", "Good");
        pack->AddObject(stt);
      } break;
    }
    return pack;
  }

  void TrackTpcCut::SetDeDx(Double_t min, Double_t max) { SetMinMax(min, max, DeDx); }

  TrackTpcCut::~TrackTpcCut() {}

  Bool_t TrackTpcCut::Pass(Track* track) {
    TpcTrack* tpc = (TpcTrack*) ((ExpTrack*) track)->GetDetTrack(DetectorID::kTPC);
    if (tpc == NULL) return ForcedUpdate(kFALSE);
    SetValue(track->GetCharge(), Charge);
    SetValue(tpc->GetDeDx(), DeDx);
    SetValue(tpc->GetNHits(), TpcHits);
    SetValue(tpc->GetSigmaPion(), SigmaPion);
    SetValue(tpc->GetSigmaElectron(), SigmaElectron);
    SetValue(tpc->GetSigmaKaon(), SigmaKaon);
    SetValue(tpc->GetSigmaProton(), SigmaProton);
    return ForcedUpdate(Verify());
  }

  Bool_t TrackTpcCut::Verify() {
    switch (fMode) {
      case kGood: {
        for (int i = SigmaPion; i < SigmaPion + 4; i++) {
          if (i == fParticleType) {  // inside "banana bounds"
            if (GetValue(i) < GetMin(i)) return kFALSE;
            if (GetValue(i) > GetMax(i)) return kFALSE;
          } else {
            // must be outside "banana"
            if (GetValue(i) > GetMin(i) && GetValue(i) < GetMax(i)) return kFALSE;
          }
        }
      } break;
      case kNotBad: {
        // inside banana
        if (GetValue(fParticleType) < GetMin(fParticleType)) return kFALSE;
        if (GetValue(fParticleType) > GetMax(fParticleType)) return kFALSE;
      } break;
    }
    // tpc dedx
    if (GetValue(DeDx) < GetMin(DeDx)) return kFALSE;
    if (GetValue(DeDx) > GetMax(DeDx)) return kFALSE;
    // hits
    if (GetValue(TpcHits) < GetMin(TpcHits)) return kFALSE;
    if (GetValue(TpcHits) > GetMax(TpcHits)) return kFALSE;
    // charge
    if (GetValue(Charge) < GetMin(Charge)) return kFALSE;
    if (GetValue(Charge) > GetMax(Charge)) return kFALSE;
    return kTRUE;
  }

  TrackTpcCut::TrackTpcCut(const TrackTpcCut& other) :
    TrackExpCut(other), fParticleType(other.fParticleType), fMode(other.fMode) {}

  void TrackTpcCut::SetActiveSigma(TString flag) {
    if (Hal::Std::FindParam(flag, "pi")) {
      fParticleType = SigmaPion;
    } else if (Hal::Std::FindParam(flag, "K")) {
      fParticleType = SigmaKaon;
    } else if (Hal::Std::FindParam(flag, "p")) {
      fParticleType = SigmaProton;
    } else if (Hal::Std::FindParam(flag, "e")) {
      fParticleType = SigmaElectron;
    } else {
      Cout::Text("Wrong SetActiveSigma flag please use pi/K/p/e", "L", kOrange);
    }
  }
}  // namespace Hal
