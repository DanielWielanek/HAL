/*
 * OTFGenerator.cxx
 *
 *  Created on: 3 lip 2024
 *      Author: daniel
 */

#include "OTFEventGenerator.h"

#include <TDatabasePDG.h>
#include <TH1.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TParticlePDG.h>
#include <TRandom.h>

#include "Cout.h"
#include "DataManager.h"
#include "OTFData.h"
#include "Std.h"
#include "Task.h"


namespace HalOTF { /* namespace Hal */

  void EventGenerator::GenerateEvent() {
    Int_t shift   = fMcEvent->GetNTracks();
    fCurrrentMult = fFixedMultiplicity;
    if (fMultiplicity) fCurrrentMult = fMultiplicity->GetRandom();
    for (int i = 0; i < fCurrrentMult; i++) {
      Double_t pt, y;
      fSpectras->GetRandom2(y, pt);
      Double_t mt  = TMath::Sqrt(pt * pt + fMass * fMass);
      Double_t phi = gRandom->Uniform(-TMath::Pi(), TMath::Pi());
      Double_t px  = pt * TMath::Cos(phi);
      Double_t py  = pt * TMath::Sin(phi);
      Double_t pz  = mt * TMath::SinH(y);

      OTF::McTrack tr;
      TLorentzVector p;
      p.SetXYZM(px, py, pz, fMass);
      tr.SetMomentum(p);
      tr.SetPdgCode(fPids);
      TLorentzVector xr(gRandom->Gaus(0, 1), gRandom->Gaus(0, 1), gRandom->Gaus(0), 0);
      tr.SetFreezout(xr);
      fMcEvent->AddTrack(tr);

      OTF::RecoTrack rtr;
      px         = px + gRandom->Gaus(0, fSmear) * px;
      py         = py + gRandom->Gaus(0, fSmear) * py;
      pz         = pz + gRandom->Gaus(0, fSmear) * pz;
      Double_t e = TMath::Sqrt(px * px + py * py + pz * pz + fMass * fMass);
      rtr.SetMom(px, py, pz, e);
      rtr.SetNHits(5);
      rtr.SetCharge(fCharge);
      rtr.SetMcIndex(i + shift);
      fRecoEvent->AddTrack(rtr);
    }
  }

  EventGenerator::EventGenerator() {}

  void EventGenerator::SetSpiecies(const TH2D& h, Int_t pid) {
    TH2D* copy         = (TH2D*) h.Clone();
    TDatabasePDG* pdg  = TDatabasePDG::Instance();
    TParticlePDG* part = pdg->GetParticle(pid);
    if (part == nullptr) {
      Hal::Cout::PrintInfo(Form("Cannot add particle with PID = %i", pid), Hal::EInfo::kWarning);
      return;
    }
    copy->SetDirectory(nullptr);
    fSpectras = copy;
    fPids     = pid;
    fMass     = part->Mass();
    fCharge   = part->Charge() * 3;
  }

  void EventGenerator::SetMultHisto(TH1D& h) {
    if (fMultiplicity) delete fMultiplicity;
    fMultiplicity = (TH1D*) h.Clone();
    fMultiplicity->SetDirectory(nullptr);
  }

  void EventGenerator::SetFixMult(Int_t mult) {
    fFixedMultiplicity = mult;
    if (fMultiplicity) delete fMultiplicity;
    fMultiplicity = nullptr;
  }

  Bool_t EventGenerator::Init() {
    if (fSpectras == nullptr) return kFALSE;
    return kTRUE;
  }

  EventGenerator::~EventGenerator() {
    if (fSpectras) delete fSpectras;
    if (fMultiplicity) delete fMultiplicity;
  }
}  // namespace HalOTF
