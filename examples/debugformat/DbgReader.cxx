/*
 * DbgReader.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "DbgReader.h"
#include "DbgMcEvent.h"
#include "DbgRecoEvent.h"

#include "Cout.h"
#include "DataManager.h"
#include "DbgData.h"
#include "Event.h"
#include "McTrack.h"
#include "Std.h"

#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TParticlePDG.h>
#include <TRandom.h>
#include <TString.h>

namespace HalDbg {
  Reader::Reader() {}

  Hal::Task::EInitFlag Reader::Init() {
    fMcEvent   = new Dbg::McEvent();
    fRecoEvent = new Dbg::RecoEvent();
    if (!fSpectras) {
      fSpectras = new TH2D("spec", "spec", 1, 0, 1, 1, -1, 1);
      fSpectras->SetBinContent(1, 1, 1);
    }
    Hal::DataManager* mng = Hal::DataManager::Instance();
    if (mng->GetObject("Dbg::McEvent.")) {  // branch exists
      fMcEvent   = (Dbg::McEvent*) mng->GetObject("Dbg::McEvent.");
      fRecoEvent = (Dbg::RecoEvent*) mng->GetObject("Dbg::RecoEvent.");
    } else {
      fOwner = kTRUE;
      mng->Register("Dbg::McEvent.", "HalEvents", fMcEvent, fRegister);
      mng->Register("Dbg::RecoEvent.", "HalEvents", fRecoEvent, fRegister);
    }
    return Hal::Task::EInitFlag::kSUCCESS;
  }

  void Reader::SetSpiecies(const TH2D& h, Int_t pid, Double_t w) {
    TH2D* copy         = (TH2D*) h.Clone();
    TDatabasePDG* pdg  = TDatabasePDG::Instance();
    TParticlePDG* part = pdg->GetParticle(pid);
    if (part == nullptr) {
      Hal::Cout::PrintInfo(Form("Cannot add particle with PID = %i", pid), Hal::EInfo::kWarning);
      return;
    }
    copy->SetDirectory(nullptr);
    fSpectras     = copy;
    fPids         = pid;
    fMass         = part->Mass();
    fMultiplicity = w;
    fCharge       = part->Charge() * 3;
  }

  void Reader::Exec(Option_t* /*opt*/) {
    fCount++;
    if (fOwner) {
      fRecoEvent->Clear();
      fMcEvent->Clear();
      Int_t runId = fCount % fModuloCount;
      fRecoEvent->SetEventId(runId);
      fMcEvent->SetEventId(runId);
    }

    Int_t shift = fMcEvent->GetNTracks();
    for (int i = 0; i < fMultiplicity; i++) {
      Double_t pt, y;
      fSpectras->GetRandom2(y, pt);
      Double_t mt  = TMath::Sqrt(pt * pt + fMass * fMass);
      Double_t phi = gRandom->Uniform(-TMath::Pi(), TMath::Pi());
      Double_t px  = pt * TMath::Cos(phi);
      Double_t py  = pt * TMath::Sin(phi);
      Double_t pz  = mt * TMath::SinH(y);

      Dbg::McTrack tr;
      TLorentzVector p;
      p.SetXYZM(px, py, pz, fMass);
      tr.SetMomentum(p);
      tr.SetPdgCode(fPids);
      TLorentzVector xr(gRandom->Gaus(0, 1), gRandom->Gaus(0, 1), gRandom->Gaus(0), 0);
      tr.SetFreezeout(xr);
      fMcEvent->AddTrack(tr);

      Dbg::RecoTrack rtr;
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

  Reader::~Reader() {
    if (fSpectras) delete fSpectras;
    if (fOwner && !fRegister) {
      if (fMcEvent) delete fMcEvent;
      if (fRecoEvent) delete fRecoEvent;
    }
  }
}  // namespace HalDbg
