/*
 * DetectedTrack.cxx
 *
 *  Created on: 7 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "ExpTrack.h"
#include "DataFormat.h"
#include "ExpEvent.h"
#include "ToFTrack.h"
#include "TpcTrack.h"
namespace Hal {
  ExpTrack::ExpTrack() : Track(), fNHits(0), fChi2(0), fTrackLenght(0) {}

  void ExpTrack::SetGlobal(Bool_t global) {
    if (global) {
      CLRBIT(fType, kGlobal);
    } else {
      SETBIT(fType, kGlobal);
    }
  }

  ExpTrack::~ExpTrack() {}

  void ExpTrack::CopyData(Track* other) {
    Track::CopyData(other);
    ExpTrack* track = (ExpTrack*) other;
    fNHits          = track->fNHits;
    fChi2           = track->fChi2;
    fTrackLenght    = track->fTrackLenght;
    fDCA            = track->fDCA;
  }

  void ExpTrackHelix::UpdateHelix() {
    TVector3 pos = GetDCA() + GetEvent()->GetVertex()->Vect();
    fHelix.SetParams(pos, GetMomentum().Vect(), GetCharge());
  }

  ExpTrackHelix::ExpTrackHelix() {}

  ExpTrackHelix::~ExpTrackHelix() {}

  void ExpTrackHelix::CopyData(Track* other) {
    ExpTrack::CopyData(other);
    ExpTrackHelix* track = (ExpTrackHelix*) other;
    fHelix               = track->fHelix;
  }

  Float_t ExpTrack::GetFieldVal(Int_t fieldID) const {
    TpcTrack* tpc = static_cast<TpcTrack*>(GetDetTrack(DetectorID::kTPC));
    ToFTrack* tof = static_cast<ToFTrack*>(GetDetTrack(DetectorID::kTOF));
    switch (fieldID) {
      case DataFieldID::EExpTrack::kChi2: return GetChi2(); break;
      case DataFieldID::EExpTrack::kNHits: return GetNHits(); break;
      case DataFieldID::EExpTrack::kTrackLenght: return GetTrackLenght(); break;
      case DataFieldID::EExpTrack::kDcaX: return GetDCA().X(); break;
      case DataFieldID::EExpTrack::kDcaY: return GetDCA().Y(); break;
      case DataFieldID::EExpTrack::kDcaZ: return GetDCA().Z(); break;
      case DataFieldID::EExpTrack::kDcaXY: return GetDCA().Pt(); break;
      case DataFieldID::EExpTrack::kDca: return GetDCA().Mag(); break;
      // only for detectors
      case DataFieldID::EExpTrack::kTofM2: return tof->GetMass2(); break;
      case DataFieldID::EExpTrack::kToFBeta: return tof->GetBeta(); break;
      case DataFieldID::EExpTrack::kToFFlag: return tof->GetFlag(); break;
      case DataFieldID::EExpTrack::kToFTime: return tof->GetTime(); break;
      case DataFieldID::EExpTrack::kToFEnergy: return tof->GetEnergy(); break;

      case DataFieldID::EExpTrack::kTpcDedx: return tpc->GetDeDx(); break;
      case DataFieldID::EExpTrack::kTpcNHits: return tpc->GetNHits(); break;
      case DataFieldID::EExpTrack::kTpcNsigmaPi: return tpc->GetSigmaPion(); break;
      case DataFieldID::EExpTrack::kTpcNsigmaKa: return tpc->GetSigmaKaon(); break;
      case DataFieldID::EExpTrack::kTpcNsigmaPr: return tpc->GetSigmaProton(); break;
      case DataFieldID::EExpTrack::kTpcNsigmaEl: return tpc->GetSigmaElectron(); break;
    }
    return Track::GetFieldVal(fieldID);
  }

  TString ExpTrack::GetFieldName(Int_t fieldID) const {
    switch (fieldID) {
      case DataFieldID::EExpTrack::kChi2: return "#chi^{2} [AU]"; break;
      case DataFieldID::EExpTrack::kNHits: return "Nhits [N]"; break;
      case DataFieldID::EExpTrack::kTrackLenght: return "L_{track} [cm]"; break;
      case DataFieldID::EExpTrack::kDcaX: return "DCA_{x} [cm]"; break;
      case DataFieldID::EExpTrack::kDcaY: return "DCA_{Y} [cm]"; break;
      case DataFieldID::EExpTrack::kDcaZ: return "DCA_{Z} [cm]"; break;
      case DataFieldID::EExpTrack::kDcaXY: return "DCA_{XY} [cm]"; break;
      case DataFieldID::EExpTrack::kDca: return "DCA [cm]"; break;
      // only for detectors
      case DataFieldID::EExpTrack::kTofM2: return "m^{2}_{ToF} [GeV^{2}/c^{4}]"; break;
      case DataFieldID::EExpTrack::kToFBeta: return "#beta_{ToF} [c]"; break;
      case DataFieldID::EExpTrack::kToFFlag: return "#Flag_{ToF} [AU]"; break;
      case DataFieldID::EExpTrack::kToFTime: return "#T_{ToF} [AU]"; break;
      case DataFieldID::EExpTrack::kToFEnergy: return "#E_{ToF} [GeV/c^{2}]"; break;

      case DataFieldID::EExpTrack::kTpcDedx: return "dEdX_{TPC} [AU]"; break;
      case DataFieldID::EExpTrack::kTpcNHits: return "#NHits_{TPC} [AU]"; break;
      case DataFieldID::EExpTrack::kTpcNsigmaPi: return "n#sigma#pi_{TPC} [AU]"; break;
      case DataFieldID::EExpTrack::kTpcNsigmaKa: return "n#sigma K_{TPC} [AU]"; break;
      case DataFieldID::EExpTrack::kTpcNsigmaPr: return "n#sigma p_{TPC} [AU]"; break;
      case DataFieldID::EExpTrack::kTpcNsigmaEl: return "n#sigma e_{TPC} [AU]"; break;
    }
    return Track::GetFieldName(fieldID);
  }
}  // namespace Hal
