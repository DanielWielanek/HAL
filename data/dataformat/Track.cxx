/*
 * Track.cxx
 *
 *  Created on: 23-06-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Track.h"

#include "CompressionMap.h"
#include "Cout.h"
#include "DataFormat.h"
#include "Event.h"

#include <TMath.h>
#include <TString.h>


namespace Hal {

  Track::Track() :
    TObject(), fEvent(nullptr), fID(0), fStatus(0), fHiddenInfo(0), fMotherID(-1), fThisID(-1), fType(0), fCharge(0) {}

  void Track::Print(Option_t* /*option*/) const {
    Cout::Database(3, "Px", "Py", "Pz");
    Cout::Database(3, Form("%4.5f", GetPx()), Form("%4.5f", GetPy()), Form("%4.5f", GetPz()));
  }

  Track::~Track() {}

  void Track::Boost(Double_t vx, Double_t vy, Double_t vz) { fP.Boost(vx, vy, vz); }

  void Track::SetPrimary() {
    SETBIT(fType, kPrimary);
    CLRBIT(fType, kMother);
  }

  void Track::CopyData(Track* other) {
    fP          = other->fP;
    fID         = other->fID;
    fStatus     = other->fStatus;
    fMotherID   = other->fMotherID;
    fCharge     = other->fCharge;
    fType       = other->fType;
    fHiddenInfo = -1;
    if (IsV0()) {
      fHiddenInfo = GetEvent()->fTotalV0s++;
      V0Track* v0 = (V0Track*) GetEvent()->fV0sHiddenInfo->ConstructedAt(fHiddenInfo);
      v0->CopyData(other->GetV0Info());
    }
  }

  void Track::CopyAllData(Track* other) {
    fEvent  = other->fEvent;
    fThisID = other->fThisID;
    CopyData(other);
  }

  Track::Track(const Track& track) : TObject(track) {
    fEvent      = track.fEvent;
    fP          = track.fP;
    fID         = track.fID;
    fStatus     = track.fStatus;
    fMotherID   = track.fMotherID;
    fThisID     = track.fThisID;
    fType       = track.fType;
    fCharge     = track.fCharge;
    fHiddenInfo = track.fHiddenInfo;
  }

  std::vector<int> Track::GetLinks() const {
    std::vector<int> links;
    GetLinksFast(links, kFALSE);
    return links;
  }

  Int_t Track::GetLinksFast(std::vector<int>& vec, Bool_t fast) const {
    if (fast) {
      Int_t size  = 0;
      vec[size++] = GetThisID();
      if (IsGoodSecondary()) { vec[size++] = GetMotherIndex(); }
      if (IsGoodV0()) {
        V0Track* v0 = GetV0Info();
        vec[size++] = v0->GetPosId();
        vec[size++] = v0->GetNegId();
      }
      return size;
    } else {
      vec.push_back(GetThisID());
      if (IsGoodSecondary()) vec.push_back(GetMotherIndex());
      if (IsGoodV0()) {
        vec.push_back(GetV0Info()->GetPosId());
        vec.push_back(GetV0Info()->GetNegId());
      }
      return (int) vec.size();
    }
  }

  void Track::TranslateLinks(const CompressionMap& map) {
    if (IsGoodSecondary()) { SetMotherIndex(map.GetNewIndex(GetMotherIndex())); }
    if (IsGoodV0()) {
      auto v0Info = GetV0Info();
      v0Info->SetPosId(map.GetNewIndex(GetV0Info()->GetPosId()));
      v0Info->SetNegId(map.GetNewIndex(GetV0Info()->GetNegId()));
      v0Info->SetTrackId(GetThisID());
    }
  }

  void Track::SetLinks(std::vector<int>& vec) {
    this->SetThisID(vec[0]);
    if (IsGoodSecondary()) {
      SetMotherIndex(vec[1]);
      if (IsGoodV0()) {
        auto v0Info = GetV0Info();
        v0Info->SetTrackId(GetThisID());
        v0Info->SetPosId(vec[2]);
        v0Info->SetNegId(vec[3]);
      }
    } else {
      if (IsGoodV0()) {
        auto v0Info = GetV0Info();
        v0Info->SetTrackId(GetThisID());
        v0Info->SetPosId(vec[1]);
        v0Info->SetNegId(vec[2]);
      }
    }
  }

  void Track::SetMass(Double_t mass) {
    Double_t p = GetMomentum().P();
    Double_t e = TMath::Sqrt(p * p + mass * mass);
    fP.SetE(e);
  }

  void Track::SetSecondary(Bool_t parent) {
    CLRBIT(fType, kPrimary);
    if (parent) {
      SETBIT(fType, kMother);
    } else {
      CLRBIT(fType, kMother);
    }
  }

  void Track::EnableV0(Bool_t v0, Bool_t good) {
    if (v0) {
      SETBIT(fType, kV0);
      if (good)
        SETBIT(fType, kV0Daughters);
      else
        CLRBIT(fType, kV0Daughters);
      fHiddenInfo = fEvent->fTotalV0s;
      fEvent->fV0sHiddenInfo->ConstructedAt(fEvent->fTotalV0s++);
    } else {
      CLRBIT(fType, kV0);
    }
  }

  Float_t Track::GetFieldVal(Int_t fieldID) const {
    switch (fieldID) {
      case DataFieldID::Track::EBasic::kPx: return fP.Px(); break;
      case DataFieldID::Track::EBasic::kPy: return fP.Py(); break;
      case DataFieldID::Track::EBasic::kPz: return fP.Pz(); break;
      case DataFieldID::Track::EBasic::kE: return fP.E(); break;
      case DataFieldID::Track::EBasic::kPhi: return fP.Phi(); break;

      case DataFieldID::Track::EBasic::kPt: return fP.Pt(); break;
      case DataFieldID::Track::EBasic::kTheta: return fP.Theta(); break;
      case DataFieldID::Track::EBasic::kP: return fP.P(); break;
      case DataFieldID::Track::EBasic::kEta: return fP.Eta(); break;
      case DataFieldID::Track::EBasic::kRapidity: return fP.Rapidity(); break;

      case DataFieldID::Track::EBasic::kCharge: return GetCharge(); break;
      case DataFieldID::Track::EBasic::kStatus: return GetStatus(); break;
      case DataFieldID::Track::EBasic::kPq: return GetCharge() * fP.P(); break;
      case DataFieldID::Track::EBasic::kTrackZero: return 0; break;

      case DataFieldID::Track::EV0::kAssumedPdg: return GetV0IfPossible(fieldID); break;
      case DataFieldID::Track::EV0::kAssumedPdgPos: return GetV0IfPossible(fieldID); break;
      case DataFieldID::Track::EV0::kAssumedPdgNeg: return GetV0IfPossible(fieldID); break;
      case DataFieldID::Track::EV0::kS1: return GetV0IfPossible(fieldID); break;
      case DataFieldID::Track::EV0::kS2: return GetV0IfPossible(fieldID); break;
      case DataFieldID::Track::EV0::kAlphaArm: return GetV0IfPossible(fieldID); break;
      case DataFieldID::Track::EV0::kPtArm: return GetV0IfPossible(fieldID); break;
      case DataFieldID::Track::EV0::kDauDist: return GetV0IfPossible(fieldID); break;
      case DataFieldID::Track::EV0::kDecLenght: return GetV0IfPossible(fieldID); break;
      case DataFieldID::Track::EV0::kCosAngle: return GetV0IfPossible(fieldID); break;
      case DataFieldID::Track::EV0::kLambdaMass: return GetV0IfPossible(fieldID); break;
      case DataFieldID::Track::EV0::kAntiLambdaMass: return GetV0IfPossible(fieldID); break;
      case DataFieldID::Track::EV0::kK0Mass: return GetV0IfPossible(fieldID); break;
    }
    if (fieldID > DataFieldID::Internal::EventStart) { return GetEvent()->GetFieldVal(fieldID); }

    return -FLT_MAX;
  }

  TString Track::GetFieldName(Int_t fieldID) const {
    switch (fieldID) {
      case DataFieldID::Track::EBasic::kPx: return "P_{x} [GeV/c]"; break;
      case DataFieldID::Track::EBasic::kPy: return "P_{y} [GeV/c]"; break;
      case DataFieldID::Track::EBasic::kPz: return "P_{z} [GeV/c]"; break;
      case DataFieldID::Track::EBasic::kE: return "E [GeV/c^{2}]"; break;
      case DataFieldID::Track::EBasic::kPhi: return "#phi [rad]"; break;

      case DataFieldID::Track::EBasic::kPt: return "P_{T} [GeV/c]"; break;
      case DataFieldID::Track::EBasic::kTheta: return "#theta [rad]"; break;
      case DataFieldID::Track::EBasic::kP: return "P [GeV/c]"; break;
      case DataFieldID::Track::EBasic::kEta: return "#eta [AU]"; break;
      case DataFieldID::Track::EBasic::kRapidity: return "y [AU]"; break;

      case DataFieldID::Track::EBasic::kCharge: return "q [e]"; break;
      case DataFieldID::Track::EBasic::kStatus: return "stat [AU]"; break;
      case DataFieldID::Track::EBasic::kPq: return "Pq [GeV/c*e]"; break;

      case DataFieldID::Track::EV0::kAssumedPdg: return "V0_{pidHypo} [PDG]"; break;
      case DataFieldID::Track::EV0::kAssumedPdgPos: return "V0_{dau pos Hypo} [PDG]"; break;
      case DataFieldID::Track::EV0::kAssumedPdgNeg: return "V0_{dau neg Hypo} [PDG]"; break;
      case DataFieldID::Track::EV0::kS1: return "V0_{s1} [cm]"; break;
      case DataFieldID::Track::EV0::kS2: return "V0_{s2} [cm]"; break;
      case DataFieldID::Track::EV0::kAlphaArm: return "V0_{#alpha} [AU]"; break;
      case DataFieldID::Track::EV0::kPtArm: return "V0_{pT} [AU]"; break;
      case DataFieldID::Track::EV0::kDauDist: return "V0_{dau-dist} [cm]"; break;
      case DataFieldID::Track::EV0::kDecLenght: return "V0_{L} [cm]"; break;
      case DataFieldID::Track::EV0::kCosAngle: return "V0_{angle cos} [AU]"; break;
      case DataFieldID::Track::EV0::kLambdaMass: return "V0_{m#Lambda} [GeV/c^{2}]"; break;
      case DataFieldID::Track::EV0::kAntiLambdaMass: return "V0_{m#bar{#Lambda}} [GeV/c^{2}]"; break;
      case DataFieldID::Track::EV0::kK0Mass: return "V0_{K_{0}^{2}} [GeV/c^{2}]"; break;
    }
    if (fieldID > DataFieldID::Internal::EventStart) { return GetEvent()->GetFieldName(fieldID); }
    Cout::PrintInfo(Form("Track::GetFieldName cannot find field with fieldID  %i", fieldID), EInfo::kLowWarning);
    return "[]";
  }

  void Track::Clear(Option_t* /*opt*/) {
    fMotherID   = -1;
    fHiddenInfo = -1;
    fThisID     = -1;
    fType       = 0;
    fEvent      = nullptr;
    fID         = -1;
  }

  V0Track* Track::GetV0Info() const {
    if (fHiddenInfo == -1) return nullptr;
    return (V0Track*) fEvent->fV0sHiddenInfo->UncheckedAt(fHiddenInfo);
  }

  void Track::ResetTrack(Int_t thisID, Event* event) {
    fMotherID   = -1;
    fHiddenInfo = -1;
    fThisID     = thisID;
    fType       = 0;
    fEvent      = event;
    fID         = -1;
  }

  Float_t Track::GetV0IfPossible(Int_t fieldId) const {
    Hal::V0Track* tr = GetV0Info();
    if (tr == nullptr) return Hal::Const::DummyVal();
    switch (fieldId) {
      case DataFieldID::Track::EV0::kAssumedPdg: return tr->GetPdg(); break;
      case DataFieldID::Track::EV0::kAssumedPdgPos: return tr->GetPdgPosDau(); break;
      case DataFieldID::Track::EV0::kAssumedPdgNeg: return tr->GetPdgNegDau(); break;
      case DataFieldID::Track::EV0::kS1: return tr->GetS().first; break;
      case DataFieldID::Track::EV0::kS2: return tr->GetS().second; break;
      case DataFieldID::Track::EV0::kAlphaArm: return tr->GetAlphaArm(); break;
      case DataFieldID::Track::EV0::kPtArm: return tr->GetPtArm(); break;
      case DataFieldID::Track::EV0::kDauDist: return tr->GetDauDist(); break;
      case DataFieldID::Track::EV0::kDecLenght: return tr->GetDecLength(); break;
      case DataFieldID::Track::EV0::kCosAngle: return tr->GetCosAngle(); break;
      case DataFieldID::Track::EV0::kLambdaMass: return tr->GetLambdaMass(); break;
      case DataFieldID::Track::EV0::kAntiLambdaMass: return tr->GetAntiLambdaMass(); break;
      case DataFieldID::Track::EV0::kK0Mass: return tr->GetK0Mass(); break;
    }
    return -FLT_MIN;
  }

}  // namespace Hal
