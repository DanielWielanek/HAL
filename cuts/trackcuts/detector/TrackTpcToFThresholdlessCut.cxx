/*
 * TrackTpcToFCut2.cxx
 *
 *  Created on: 17 lip 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackTpcToFThresholdlessCut.h"

#include "DataFormat.h"
#include "ExpTrack.h"

namespace Hal {
  TrackTpcToFThresholdlessCut::TrackTpcToFThresholdlessCut(TrackTpcCut* tpc, TrackToFMass2Cut* tof) : TrackTpcToFCut(tpc, tof) {}

  Bool_t TrackTpcToFThresholdlessCut::Pass(Track* tr) {
    TpcTrack* track_tpc = (TpcTrack*) ((ExpTrack*) tr)->GetDetTrack(DetectorID::kTPC);
    ToFTrack* track_tof = (ToFTrack*) ((ExpTrack*) tr)->GetDetTrack(DetectorID::kTOF);
    SetValue(track_tof->GetMass2(), 7);
    SetValue(track_tpc->GetSigmaPion(), PionSigma());
    SetValue(track_tpc->GetSigmaKaon(), KaonSigma());
    SetValue(track_tpc->GetSigmaProton(), ProtonSigma());
    SetValue(track_tpc->GetSigmaElectron(), ElectronSigma());
    SetValue(track_tpc->GetDeDx(), DeDx());
    SetValue(tr->GetCharge(), Charge());
    SetValue(track_tpc->GetNHits(), TpcHits());
    Bool_t require_tof = kFALSE;
    for (int i = 0; i < 4; i++) {
      Int_t par      = i + PionSigma();
      Double_t sigma = GetValue(par);
      if (par == fTpc->GetActiveSigma()) {  // active sigma veryfication
        if (sigma < GetMin(par)) return ForcedUpdate(kFALSE);
        if (sigma > GetMax(par)) return ForcedUpdate(kFALSE);
      } else {
        if (sigma > GetMin(par) && sigma < GetMax(par))  // overlapping dedx require tof
          require_tof = kTRUE;
      }
    }
    Double_t m2 = fToF->GetValue(0);
    if (require_tof) {
      if (m2 == 0.0 || m2 < -1) return ForcedUpdate(kFALSE);  // no tof data
    }
    if (m2 == 0.0 || m2 < -1) return ForcedUpdate(kTRUE);  // no tof data accept
    if (m2 < fToF->GetMin()) return ForcedUpdate(kFALSE);
    if (m2 > fToF->GetMax()) return ForcedUpdate(kFALSE);
    return ForcedUpdate(kTRUE);
  }

  TrackTpcToFThresholdlessCut::~TrackTpcToFThresholdlessCut() {}
}  // namespace Hal
