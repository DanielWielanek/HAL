/*
 * Decay.cxx
 *
 *  Created on: 21 gru 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Decay.h"

#include "Cout.h"
#include "McEvent.h"
#include "McTrack.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TParticlePDG.h>
#include <TRandom.h>
#include <TString.h>

namespace Hal {

  DecayChannel::DecayChannel(Int_t dau1, Int_t dau2, Double_t ratio) : fDaughters(2), fBranchRatio(ratio) {
    fPdgCodes.resize(2);
    fMass.resize(2);
    fPdgCodes[0] = dau1;
    fPdgCodes[1] = dau2;
    auto pids    = TDatabasePDG::Instance();
    for (int i = 0; i < 2; i++) {
      auto pidpart = pids->GetParticle(fPdgCodes[i]);
      if (pidpart) {
        fMass[i] = pidpart->Mass();
      } else {
        fMass[i] = 0.0;
        Hal::Cout::Text(Form("DecayChannel: Cannot find Particle with PID %i", fPdgCodes[i]), "L", kOrange);
      }
    }
  }

  DecayChannel::DecayChannel(Int_t dau1, Int_t dau2, Int_t dau3, Double_t ratio) : fDaughters(3), fBranchRatio(ratio) {
    fPdgCodes.resize(3);
    fMass.resize(3);
    fPdgCodes[0] = dau1;
    fPdgCodes[1] = dau2;
    fPdgCodes[2] = dau3;
    auto pids    = TDatabasePDG::Instance();
    for (int i = 0; i < 3; i++) {
      auto pidpart = pids->GetParticle(fPdgCodes[i]);
      if (pidpart) {
        fMass[i] = pidpart->Mass();
      } else {
        fMass[i] = 0.0;
        Hal::Cout::Text(Form("DecayChannel: Cannot find Particle with PID %i", fPdgCodes[i]), "L", kOrange);
      }
    }
  }

  Bool_t DecayChannel::Init() {
    for (auto m : fMass) {
      fMassThres += m;
    }
    return kTRUE;
  }

  Decay::Decay(Int_t motherPdg) {
    fMotherPdg   = motherPdg;
    auto pids    = TDatabasePDG::Instance();
    auto pidpart = pids->GetParticle(fMotherPdg);
    if (pidpart) { fMotherMass = pidpart->Mass(); }
  }

  Double_t Decay::GetDecayTime(McTrack& mother, Double_t mass) const {
    auto mP = mother.GetMomentum();
    if (fGamma == 0) { return 1E+10; }
    double tTau0 = mP.E() / (mass * fGamma);
    // When it decays
    return -tTau0 * TMath::Log(gRandom->Rndm());
  }

  void Decay::Decay2Body(McTrack& mother, std::vector<McTrack*>& daughters, const DecayChannel& channel) const {
    Double_t M = fMotherMass;
    if (fBreightWigher) { M = GetDecayTime(mother, fMotherMass); }
    if (M < channel.GetMassThreshold()) { M = channel.GetMassThreshold(); }
    Double_t tTime = GetDecayTime(mother, M);

    const TLorentzVector& mP = mother.GetMomentum();
    const TLorentzVector& mX = mother.GetFreezoutPosition();
    Int_t pdg1               = channel.GetDaughterPdg(0);
    Int_t pdg2               = channel.GetDaughterPdg(1);
    Double_t m1              = channel.GetDaughterMass(0);
    Double_t m2              = channel.GetDaughterMass(1);
    // Decay posistion
    TVector3 boost = mP.BoostVector();
    Double_t X     = mX.X() + boost.X() * tTime;
    Double_t Y     = mX.Y() + boost.Y() * tTime;
    Double_t Z     = mX.Z() + boost.Z() * tTime;
    Double_t T     = mX.T() + tTime;

    Double_t tMC1 = (M * M - (m1 + m2) * (m1 + m2));
    Double_t tMC2 = (M * M - (m1 - m2) * (m1 - m2));
    Double_t tMom = TMath::Sqrt(tMC1 * tMC2) / (2 * M);

    Double_t px1, py1, pz1;
    Double_t px2, py2, pz2;
    gRandom->Sphere(px1, py1, pz1, tMom);
    px2 = -px1;
    py2 = -py1;
    pz2 = -pz1;
    TLorentzVector dau1, dau2;
    dau1.SetPxPyPzE(px1, py1, pz1, TMath::Sqrt(tMom * tMom + m1 * m1));
    dau2.SetPxPyPzE(px2, py2, pz2, TMath::Sqrt(tMom * tMom + m2 * m2));
    dau1.Boost(boost);
    dau2.Boost(boost);
    auto& daughter1 = daughters[0];
    auto& daughter2 = daughters[1];
    daughter1->SetMotherIndex(mother.GetThisID());
    daughter2->SetMotherIndex(mother.GetThisID());
    daughter1->SetPdg(pdg1);
    daughter2->SetPdg(pdg2);
    daughter1->SetMomentum(dau1.X(), dau1.Y(), dau1.Z(), dau1.T());
    daughter2->SetMomentum(dau2.X(), dau2.Y(), dau2.Z(), dau2.T());
    daughter1->SetFreezoutPosition(X, Y, Z, T);
    daughter2->SetFreezoutPosition(X, Y, Z, T);
  }

  void Decay::Decay3Body(McTrack& mother, std::vector<McTrack*>& daughters, const DecayChannel& channel) const {
    Double_t tE  = mother.GetMomentum().E();
    Double_t tM1 = channel.GetDaughterMass(0);
    Double_t tM2 = channel.GetDaughterMass(1);
    Double_t tM3 = channel.GetDaughterMass(2);
    Double_t M   = fMotherMass;
    if (fBreightWigher) { M = GetDecayTime(mother, fMotherMass); }
    if (M < channel.GetMassThreshold()) { M = channel.GetMassThreshold(); }
    Double_t tTime = GetDecayTime(mother, M);
    // Father mass via BreitWigner
    Double_t tES1, tES2, tP1, tP2, tCos12, tZ;
    do {
      // Generate E1 and E2 with the Monte-Carlo method
      do {
        tES1 = gRandom->Rndm() * (M - tM2 - tM3 - tM1) + tM1;
        tES2 = gRandom->Rndm() * (M - tM1 - tM3 - tM2) + tM2;
      } while (tES1 + tES2 > M);  // The sum of both energies must be smaller than the resonance mass
      tP1 = TMath::Sqrt(tES1 * tES1 - tM1 * tM1);
      tP2 = TMath::Sqrt(tES2 * tES2 - tM2 * tM2);
      tZ  = M - tES1 - tES2;
      tZ *= tZ;
      tCos12 = (tZ - tP1 * tP1 - tP2 * tP2 - tM3 * tM3) / (2 * tP1 * tP2);
    } while ((tCos12 < -1.0) || (tCos12 > 1.0));  // Cos Theta must exist (be within -1.0 to 1.0 )


    auto& momP = mother.GetMomentum();
    auto& momX = mother.GetFreezoutPosition();
    // Decay coordinates
    Double_t X = momX.X() + (momP.Px() / tE) * tTime;
    Double_t Y = momX.Y() + (momP.Py() / tE) * tTime;
    Double_t Z = momX.Z() + (momP.Pz() / tE) * tTime;
    Double_t T = momX.T() + tTime;

    Double_t tPxr2 = tP2 * TMath::Sqrt(1 - tCos12 * tCos12);
    Double_t tPzr2 = tP2 * tCos12;
    Double_t tPxr3 = -tPxr2;
    Double_t tPzr3 = -(tP1 + tPzr2);
    Double_t tP3   = TMath::Hypot(tPxr3, tPzr3);
    Double_t tES3  = TMath::Hypot(tM3, tP3);

    // Generating Euler angles
    Double_t tPhi   = gRandom->Rndm() * 2 * TMath::Pi();
    Double_t tKsi   = gRandom->Rndm() * 2 * TMath::Pi();
    Double_t tCosTh = gRandom->Rndm() * 2.0 - 1.0;

    Double_t sp = TMath::Sin(tPhi);
    Double_t cp = TMath::Cos(tPhi);
    Double_t sk = TMath::Sin(tKsi);
    Double_t ck = TMath::Cos(tKsi);
    Double_t st = TMath::Sqrt(1.0 - tCosTh * tCosTh);
    Double_t ct = tCosTh;

    // Rotating the whole system
    Double_t tPxp1 = -st * ck * tP1;
    Double_t tPyp1 = st * sk * tP1;
    Double_t tPzp1 = ct * tP1;

    Double_t tPxp2 = (cp * ct * ck - sp * sk) * tPxr2 + (-st * ck) * tPzr2;
    Double_t tPyp2 = (-cp * ct * sk - sp * ck) * tPxr2 + (st * sk) * tPzr2;
    Double_t tPzp2 = cp * st * tPxr2 + ct * tPzr2;

    Double_t tPxp3 = (cp * ct * ck - sp * sk) * tPxr3 + (-st * ck) * tPzr3;
    Double_t tPyp3 = (-cp * ct * sk - sp * ck) * tPxr3 + (st * sk) * tPzr3;
    Double_t tPzp3 = cp * st * tPxr3 + ct * tPzr3;
    TVector3 boost = momP.BoostVector();

    tES1 = TMath::Sqrt(tM1 * tM1 + tPxp1 * tPxp1 + tPyp1 * tPyp1 + tPzp1 * tPzp1);
    tES2 = TMath::Sqrt(tM2 * tM2 + tPxp2 * tPxp2 + tPyp2 * tPyp2 + tPzp2 * tPzp2);
    tES3 = TMath::Sqrt(tM3 * tM3 + tPxp3 * tPxp3 + tPyp3 * tPyp3 + tPzp3 * tPzp3);

    TLorentzVector p1, p2, p3;
    p1.SetPxPyPzE(tPxp1, tPyp1, tPzp1, tES1);
    p2.SetPxPyPzE(tPxp2, tPyp2, tPzp2, tES2);
    p3.SetPxPyPzE(tPxp3, tPyp3, tPzp3, tES3);
    p1.Boost(boost);
    p2.Boost(boost);
    p3.Boost(boost);

    auto& daughter1 = daughters[0];
    auto& daughter2 = daughters[1];
    auto& daughter3 = daughters[2];
    daughter1->SetMotherIndex(mother.GetThisID());
    daughter2->SetMotherIndex(mother.GetThisID());
    daughter3->SetMotherIndex(mother.GetThisID());
    daughter1->SetPdg(channel.GetDaughterPdg(0));
    daughter2->SetPdg(channel.GetDaughterPdg(1));
    daughter3->SetPdg(channel.GetDaughterPdg(2));
    daughter1->SetMomentum(p1.X(), p1.Y(), p1.Z(), p1.T());
    daughter2->SetMomentum(p2.X(), p2.Y(), p2.Z(), p2.T());
    daughter3->SetMomentum(p3.X(), p3.Y(), p3.Z(), p3.T());
    daughter1->SetFreezoutPosition(X, Y, Z, T);
    daughter2->SetFreezoutPosition(X, Y, Z, T);
    daughter3->SetFreezoutPosition(X, Y, Z, T);
  }

  Int_t Decay::DecayParticle(McTrack& mother, std::vector<McTrack*>& daughters, Bool_t addToEvent) const {
    /**
     * get random channel
     */
    Double_t val = gRandom->Uniform();
    Int_t chan   = 0;
    while (val > fDecayChannels[chan].GetBranchingRatio()) {
      chan++;
    }
    auto decay        = fDecayChannels[chan];
    Int_t daughtersNo = decay.GetDaughtersNo();
    if (daughtersNo <= 0) {
      Hal::Cout::PrintInfo("Decay::DecayParitcle this number of channels not supported", EInfo::kError);
      return -1;
    }
    if (daughtersNo == 2) {
      Decay2Body(mother, daughters, decay);
    } else if (daughtersNo == 3) {
      Decay3Body(mother, daughters, decay);
    }
    if (addToEvent) {
      McEvent* event = (McEvent*) mother.GetEvent();
      for (int i = 0; i < daughtersNo; i++) {
        int tracks    = event->GetTotalTrackNo();
        McTrack* to   = (McTrack*) event->AddTrack();
        McTrack* from = daughters[i];
        from->SetThisID(tracks);
        from->SetEvent(event);
        to->CopyAllData(from);
      }
    }

    return daughtersNo;
  }

  Bool_t Decay::Init() {
    if (fDecayChannels.size() == 0) return kFALSE;
    Double_t branchSum = 0;
    for (auto& d : fDecayChannels) {
      if (!d.Init()) return kFALSE;
    }
    for (auto d : fDecayChannels) {
      branchSum += d.GetBranchingRatio();
    }
    for (auto& d : fDecayChannels) {
      d.SetBranchRatio(d.GetBranchingRatio() / branchSum);
    }
    branchSum = 0;
    for (auto& d : fDecayChannels) {
      branchSum += d.GetBranchingRatio();
      d.SetBranchRatio(branchSum);
    }
    fDecayChannels[fDecayChannels.size() - 1].SetBranchRatio(1.1);  // additional crosscheck
    auto pids    = TDatabasePDG::Instance();
    auto pidpart = pids->GetParticle(fMotherPdg);
    if (!pidpart) return kFALSE;
    fMotherMass = pidpart->Mass();
    return kTRUE;
  }
}  // namespace Hal
