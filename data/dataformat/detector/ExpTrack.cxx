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
      case DataFieldID::Track::EExp::kChi2: return GetChi2(); break;
      case DataFieldID::Track::EExp::kNHits: return GetNHits(); break;
      case DataFieldID::Track::EExp::kTrackLenght: return GetTrackLenght(); break;
      case DataFieldID::Track::EExp::kDcaX: return GetDCA().X(); break;
      case DataFieldID::Track::EExp::kDcaY: return GetDCA().Y(); break;
      case DataFieldID::Track::EExp::kDcaZ: return GetDCA().Z(); break;
      case DataFieldID::Track::EExp::kDcaXY: return GetDCA().Pt(); break;
      case DataFieldID::Track::EExp::kDca: return GetDCA().Mag(); break;
      // only for detectors
      case DataFieldID::Track::EExp::kTofM2: return tof->GetMass2(); break;
      case DataFieldID::Track::EExp::kToFBeta: return tof->GetBeta(); break;
      case DataFieldID::Track::EExp::kToFFlag: return tof->GetFlag(); break;
      case DataFieldID::Track::EExp::kToFTime: return tof->GetTime(); break;
      case DataFieldID::Track::EExp::kToFEnergy: return tof->GetEnergy(); break;

      case DataFieldID::Track::EExp::kTpcDedx: return tpc->GetDeDx(); break;
      case DataFieldID::Track::EExp::kTpcNHits: return tpc->GetNHits(); break;
      case DataFieldID::Track::EExp::kTpcNsigmaPi: return tpc->GetSigmaPion(); break;
      case DataFieldID::Track::EExp::kTpcNsigmaKa: return tpc->GetSigmaKaon(); break;
      case DataFieldID::Track::EExp::kTpcNsigmaPr: return tpc->GetSigmaProton(); break;
      case DataFieldID::Track::EExp::kTpcNsigmaEl: return tpc->GetSigmaElectron(); break;
    }
    return Track::GetFieldVal(fieldID);
  }

  TString ExpTrack::GetFieldName(Int_t fieldID) const {
    switch (fieldID) {
      case DataFieldID::Track::EExp::kChi2: return "#chi^{2} [AU]"; break;
      case DataFieldID::Track::EExp::kNHits: return "Nhits [N]"; break;
      case DataFieldID::Track::EExp::kTrackLenght: return "L_{track} [cm]"; break;
      case DataFieldID::Track::EExp::kDcaX: return "DCA_{x} [cm]"; break;
      case DataFieldID::Track::EExp::kDcaY: return "DCA_{Y} [cm]"; break;
      case DataFieldID::Track::EExp::kDcaZ: return "DCA_{Z} [cm]"; break;
      case DataFieldID::Track::EExp::kDcaXY: return "DCA_{XY} [cm]"; break;
      case DataFieldID::Track::EExp::kDca: return "DCA [cm]"; break;
      // only for detectors
      case DataFieldID::Track::EExp::kTofM2: return "m^{2}_{ToF} [GeV^{2}/c^{4}]"; break;
      case DataFieldID::Track::EExp::kToFBeta: return "#beta_{ToF} [c]"; break;
      case DataFieldID::Track::EExp::kToFFlag: return "#Flag_{ToF} [AU]"; break;
      case DataFieldID::Track::EExp::kToFTime: return "#T_{ToF} [AU]"; break;
      case DataFieldID::Track::EExp::kToFEnergy: return "#E_{ToF} [GeV/c^{2}]"; break;

      case DataFieldID::Track::EExp::kTpcDedx: return "dEdX_{TPC} [AU]"; break;
      case DataFieldID::Track::EExp::kTpcNHits: return "#NHits_{TPC} [AU]"; break;
      case DataFieldID::Track::EExp::kTpcNsigmaPi: return "n#sigma#pi_{TPC} [AU]"; break;
      case DataFieldID::Track::EExp::kTpcNsigmaKa: return "n#sigma K_{TPC} [AU]"; break;
      case DataFieldID::Track::EExp::kTpcNsigmaPr: return "n#sigma p_{TPC} [AU]"; break;
      case DataFieldID::Track::EExp::kTpcNsigmaEl: return "n#sigma e_{TPC} [AU]"; break;
    }
    return Track::GetFieldName(fieldID);
  }
}  // namespace Hal
