/*
 * TrackCombined.cxx
 *
 *  Created on: 4 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "ComplexTrack.h"
#include "ComplexEvent.h"
#include "DataFormat.h"

#include <TVector2.h>

namespace Hal {
  ComplexTrack::ComplexTrack() : Track(), fMatchID(-1), fRealTrack(NULL), fImgTrack(NULL) {}

  ComplexTrack::~ComplexTrack() {}

  void ComplexTrack::CopyData(Track* other) {
    Track::CopyData(other);
    fMatchID = ((ComplexTrack*) other)->GetMatchID();
  }

  ComplexTrack::ComplexTrack(const ComplexTrack& other) :
    Track(other), fMatchID(other.fMatchID), fRealTrack(other.fRealTrack), fImgTrack(other.fImgTrack) {}

  ComplexTrack& ComplexTrack::operator=(const ComplexTrack& other) {
    if (this != &other) {
      Track::operator=(other);
      fMatchID   = other.fMatchID;
      fRealTrack = other.fRealTrack;
      fImgTrack  = other.fImgTrack;
    }
    return *this;
  }

  Float_t ComplexTrack::GetFieldVal(Int_t fieldID) const {
    if (fieldID >= DataFieldID::Internal::EventStart) {
      if (GetEvent())
        return GetEvent()->GetFieldVal(fieldID);
      else
        return FLT_MIN;
    } else if (fieldID >= DataFieldID::ImStep) {
      if (GetImgTrack() == nullptr) return FLT_MIN;
      return GetImgTrack()->GetFieldVal(fieldID - DataFieldID::ImStep);
    } else if (fieldID >= DataFieldID::ReStep) {
      return GetRealTrack()->GetFieldVal(fieldID - DataFieldID::ReStep);
    } else {
      Track* mc   = GetImgTrack();
      Track* reco = GetRealTrack();
      if (mc == nullptr) return FLT_MIN;
      switch (fieldID) {
        case DataFieldID::Track::EComplex::kDeltaPt: return mc->GetMomentum().Pt() - reco->GetMomentum().Pt(); break;
        case DataFieldID::Track::EComplex::kDeltaPz: return mc->GetMomentum().Pz() - reco->GetMomentum().Pz(); break;
        case DataFieldID::Track::EComplex::kDeltaP: return mc->GetMomentum().P() - reco->GetMomentum().P(); break;
        case DataFieldID::Track::EComplex::kDeltaPx: return mc->GetMomentum().Px() - reco->GetMomentum().Px(); break;
        case DataFieldID::Track::EComplex::kDeltaPy: return mc->GetMomentum().Py() - reco->GetMomentum().Py(); break;
        case DataFieldID::Track::EComplex::kDeltaPhi:
          return TVector2::Phi_mpi_pi(mc->GetMomentum().Phi() - reco->GetMomentum().Phi());
          break;
        case DataFieldID::Track::EComplex::kDeltaTheta:
          return TVector2::Phi_mpi_pi(mc->GetMomentum().Theta() - reco->GetMomentum().Theta());
          break;
        case DataFieldID::Track::EComplex::kDeltaEta: return mc->GetMomentum().Eta() - reco->GetMomentum().Eta(); break;
        default: return Track::GetFieldVal(fieldID); break;
      }
    }
    return Track::GetFieldVal(fieldID);
  }

  TString ComplexTrack::GetFieldName(Int_t fieldID) const {
    if (fieldID >= DataFieldID::Internal::EventStart) {
      if (GetEvent())
        return GetEvent()->GetFieldVal(fieldID);
      else
        return "xxx";
    } else if (fieldID >= DataFieldID::ImStep) {
      ComplexEvent* ev = static_cast<ComplexEvent*>(GetEvent());
      Track* tr        = ev->GetImgEvent()->GetNewTrack();
      tr->SetEvent(ev->GetImgEvent());
      TString name = tr->GetFieldName(fieldID - DataFieldID::ImStep) + " (im)";

      delete tr;
      return name;
    } else if (fieldID >= DataFieldID::ReStep) {
      ComplexEvent* ev = static_cast<ComplexEvent*>(GetEvent());
      Track* tr        = ev->GetRealEvent()->GetNewTrack();
      tr->SetEvent(ev->GetRealEvent());
      TString name = tr->GetFieldName(fieldID - DataFieldID::ReStep) + " (re)";
      delete tr;
      return name;
    } else {
      switch (fieldID) {
        case DataFieldID::Track::EComplex::kDeltaPt: return "#Delta p_{T} [GeV/c]"; break;
        case DataFieldID::Track::EComplex::kDeltaPz: return "#Delta p_{z} [GeV/c]"; break;
        case DataFieldID::Track::EComplex::kDeltaP: return "#Delta p [GeV/c]"; break;
        case DataFieldID::Track::EComplex::kDeltaPx: return "#Delta p_{x} [GeV/c]"; break;
        case DataFieldID::Track::EComplex::kDeltaPy: return "#Delta p_{y} [GeV/c]"; break;
        case DataFieldID::Track::EComplex::kDeltaPhi: return "#Delta #phi [rad]"; break;
        case DataFieldID::Track::EComplex::kDeltaTheta: return "#Delta #theta [rad]"; break;
        case DataFieldID::Track::EComplex::kDeltaEta: return "#Delta #eta []"; break;
        default: return Track::GetFieldName(fieldID); break;
      }
    }
    return Track::GetFieldName(fieldID);
  }

  void ComplexTrack::Clear(Option_t* /*opt*/) {
    Track::Clear();
    if (fRealTrack) fRealTrack->Clear();
    if (fImgTrack) fImgTrack->Clear();
  }

  void ComplexTrack::ResetTrack(Int_t thisID, Event* event) {
    Track::ResetTrack(thisID, event);
    fRealTrack = nullptr;
    fImgTrack  = nullptr;
    fMatchID   = -1;
  }

  void ComplexTrack::CopyAllData(Track* other) {
    Track::CopyAllData(other);
    fMatchID   = ((ComplexTrack*) other)->GetMatchID();
    fRealTrack = ((ComplexTrack*) other)->GetRealTrack();
    fImgTrack  = ((ComplexTrack*) other)->GetImgTrack();
  }
}  // namespace Hal
