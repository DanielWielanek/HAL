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
#include "HalStdString.h"
#include "Package.h"
#include "Parameter.h"
#include "TpcTrack.h"
#include "Track.h"

namespace Hal {
  const Int_t TrackTpcCut::fgTpcHitsId       = 0;
  const Int_t TrackTpcCut::fgChargeId        = 1;
  const Int_t TrackTpcCut::fgSigmaPionId     = 2;
  const Int_t TrackTpcCut::fgSigmaKaonId     = 3;
  const Int_t TrackTpcCut::fgSigmaProtonId   = 4;
  const Int_t TrackTpcCut::fgSigmaElectronId = 5;
  const Int_t TrackTpcCut::fgDeDxId          = 6;

  TrackTpcCut::TrackTpcCut() : TrackExpCut(7) {
    SetUnitName("TpcHits [N]", TpcHits());
    SetUnitName("TpcCharge [q]", Charge());
    SetUnitName("#sigma#pi", PionSigma());
    SetUnitName("#sigmaK", KaonSigma());
    SetUnitName("#sigmaP", ProtonSigma());
    SetUnitName("#sigmae", ElectronSigma());
    SetUnitName("dEdX [kV/cm]", DeDx());
    fParticleType = PionSigma();
    SetMinMax(-1E+29, 1E+29, PionSigma());
    SetMinMax(-1E+29, 1E+29, KaonSigma());
    SetMinMax(-1E+29, 1E+29, ProtonSigma());
    SetMinMax(-1E+29, 1E+29, ElectronSigma());
    SetMinMax(0, 1E+29, DeDx());
    SetMinMax(-1, 1, Charge());
    SetMinMax(0, 90, TpcHits());
    fMode = kNotBad;
  }

  Bool_t TrackTpcCut::Init(Int_t task_id) {
    Bool_t stat = TrackExpCut::Init(task_id);
    if (stat == kFALSE) return kFALSE;
    if (TpcAvaiable(task_id)) return kTRUE;
    return kFALSE;
  }

  void TrackTpcCut::SetSigma(Double_t min, Double_t max, TString opt) {
    if (HalStd::FindParam(opt, "pi")) {
      SetMinMax(min, max, PionSigma());
    } else if (HalStd::FindParam(opt, "K")) {
      SetMinMax(min, max, KaonSigma());
    } else if (HalStd::FindParam(opt, "p")) {
      SetMinMax(min, max, ProtonSigma());
    } else if (HalStd::FindParam(opt, "e")) {
      SetMinMax(min, max, ElectronSigma());
    } else {
      Cout::Text("Wrong SetSigma flag please use pi/K/p/e", "L", kOrange);
    }
  }

  void TrackTpcCut::SetCharge(Int_t i) { SetMinAndMax(i, Charge()); }

  void TrackTpcCut::SetNHits(Int_t min, Int_t max) { SetMinMax(min, max, TpcHits()); }

  Package* TrackTpcCut::Report() const {
    Package* pack = TrackCut::Report();
    TString sigma_name;
    switch (fParticleType) {
      case fgSigmaPionId: sigma_name = "PionSigma"; break;
      case fgSigmaKaonId: sigma_name = "KaonSigma"; break;
      case fgSigmaProtonId: sigma_name = "ProtonSigma"; break;
      case fgSigmaElectronId: sigma_name = "ElectronSigma"; break;
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

  void TrackTpcCut::SetDeDx(Double_t min, Double_t max) { SetMinMax(min, max, DeDx()); }

  TrackTpcCut::~TrackTpcCut() {}

  Bool_t TrackTpcCut::Pass(Track* track) {
    TpcTrack* tpc = (TpcTrack*) ((ExpTrack*) track)->GetDetTrack(DetectorID::kTPC);
    if (tpc == NULL) return ForcedUpdate(kFALSE);
    SetValue(track->GetCharge(), fgChargeId);
    SetValue(tpc->GetDeDx(), fgDeDxId);
    SetValue(tpc->GetNHits(), fgTpcHitsId);
    SetValue(tpc->GetSigmaPion(), fgSigmaPionId);
    SetValue(tpc->GetSigmaElectron(), fgSigmaElectronId);
    SetValue(tpc->GetSigmaKaon(), fgSigmaKaonId);
    SetValue(tpc->GetSigmaProton(), fgSigmaProtonId);
    return ForcedUpdate(Verify());
  }

  Bool_t TrackTpcCut::Verify() {
    switch (fMode) {
      case kGood: {
        for (int i = fgSigmaPionId; i < fgSigmaPionId + 4; i++) {
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
    if (GetValue(fgDeDxId) < GetMin(fgDeDxId)) return kFALSE;
    if (GetValue(fgDeDxId) > GetMax(fgDeDxId)) return kFALSE;
    // hits
    if (GetValue(fgTpcHitsId) < GetMin(fgTpcHitsId)) return kFALSE;
    if (GetValue(fgTpcHitsId) > GetMax(fgTpcHitsId)) return kFALSE;
    // charge
    if (GetValue(fgChargeId) < GetMin(fgChargeId)) return kFALSE;
    if (GetValue(fgChargeId) > GetMax(fgChargeId)) return kFALSE;
    return kTRUE;
  }

  TrackTpcCut::TrackTpcCut(const TrackTpcCut& other) :
    TrackExpCut(other), fParticleType(other.fParticleType), fMode(other.fMode) {}

  void TrackTpcCut::SetActiveSigma(TString flag) {
    if (HalStd::FindParam(flag, "pi")) {
      fParticleType = PionSigma();
    } else if (HalStd::FindParam(flag, "K")) {
      fParticleType = KaonSigma();
    } else if (HalStd::FindParam(flag, "p")) {
      fParticleType = ProtonSigma();
    } else if (HalStd::FindParam(flag, "e")) {
      fParticleType = ElectronSigma();
    } else {
      Cout::Text("Wrong SetActiveSigma flag please use pi/K/p/e", "L", kOrange);
    }
  }
}  // namespace Hal
