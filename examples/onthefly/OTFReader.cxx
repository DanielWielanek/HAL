/*
 * OTFReader.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "OTFReader.h"
#include "OTFComplexEvent.h"
#include "OTFMcEvent.h"
#include "OTFRecoEvent.h"

#include "Cout.h"
#include "DataManager.h"
#include "Event.h"
#include "McTrack.h"
#include "OTFData.h"
#include "Std.h"

#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TParticlePDG.h>
#include <TRandom.h>
#include <TString.h>

namespace HalOTF {
  Reader::Reader() :
    fSpectras(nullptr),
    fOwner(kFALSE),
    fRegister(kFALSE),
    fMultiplicity(nullptr),
    fPids(211),
    fCharge(1),
    fMass(0),
    fSmear(0.01),
    fMcEvent(nullptr),
    fRecoEvent(nullptr) {}

  Hal::Task::EInitFlag Reader::Init() {
    fMcEvent              = new OTF::McEvent();
    fRecoEvent            = new OTF::RecoEvent();
    Hal::DataManager* mng = Hal::DataManager::Instance();
    if (mng->GetObject("OTF::McEvent.")) {  // branch exists
      fMcEvent   = (OTF::McEvent*) mng->GetObject("OTF::McEvent.");
      fRecoEvent = (OTF::RecoEvent*) mng->GetObject("OTF::RecoEvent.");
    } else {
      fOwner = kTRUE;
      mng->Register("OTF::McEvent.", "HalEvents", fMcEvent, fRegister);
      mng->Register("OTF::RecoEvent.", "HalEvents", fRecoEvent, fRegister);
    }

    switch (fTranslate) {
      case ETranslate::kMc: {
        if (mng->GetObject("HalOTF::McEvent.")) {
          fTranslate = ETranslate::kNone;
        } else {
          fHalMcEvent = new HalOTF::McEvent();
          mng->Register("HalOTF::McEvent.", "HalEvents", fHalMcEvent, kFALSE);
          fTranslateInterface = fHalMcEvent->CreateInterface();
        }
      } break;
      case ETranslate::kReco: {
        if (mng->GetObject("HalOTF::RecoEvent.")) {
          fTranslate = ETranslate::kNone;
        } else {
          fHalRecoEvent = new HalOTF::RecoEvent();
          mng->Register("HalOTF::RecoEvent.", "HalEvents", fHalRecoEvent, kFALSE);
          fTranslateInterface = fHalRecoEvent->CreateInterface();
        }
      } break;
      case ETranslate::kComplex: {
        if (mng->GetObject("HalOTF::ComplexEvent.")) {
          fTranslate = ETranslate::kNone;
        } else {
          fHalComplexEvent = new HalOTF::ComplexEvent();
          mng->Register("HalOTF::ComplexEvent.", "HalEvents", fHalComplexEvent, kFALSE);
          fTranslateInterface = fHalComplexEvent->CreateInterface();
        }
      } break;
      default: break;
    }
    if (fTranslateInterface) fTranslateInterface->ConnectToTree(Hal::EventInterface::eMode::kRead);
    return Hal::Task::EInitFlag::kSUCCESS;
  }

  void Reader::SetSpiecies(const TH2D& h, Int_t pid) {
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

  void Reader::GenerateEvent() {
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
  void Reader::Exec(Option_t* /*opt*/) {
    PrepareTables();
    GenerateEvent();
    if (fTranslate != ETranslate::kNone) TranslateEvent();
  }

  Reader::~Reader() {
    if (fSpectras) delete fSpectras;
    if (fOwner && !fRegister) {
      if (fMcEvent) delete fMcEvent;
      if (fRecoEvent) delete fRecoEvent;
    }
    if (fMultiplicity) delete fMultiplicity;
  }

  void Reader::PrepareTables() {
    if (fOwner) {
      fRecoEvent->Clear();
      fMcEvent->Clear();
    }
  }

  void Reader::SetMultHisto(TH1D& h) {
    if (fMultiplicity) delete fMultiplicity;
    fMultiplicity = (TH1D*) h.Clone();
    fMultiplicity->SetDirectory(nullptr);
  }

  void Reader::SetFixMult(Int_t mult) {
    fFixedMultiplicity = mult;
    if (fMultiplicity) delete fMultiplicity;
    fMultiplicity = nullptr;
  }

  void Reader::Translate(TString opt) {
    Int_t flag = 0;
    if (Hal::Std::FindParam(opt, "sim")) { flag = 1; }
    if (Hal::Std::FindParam(opt, "reco")) { flag += 2; }
    switch (flag) {
      case 1: fTranslate = ETranslate::kMc; break;
      case 2: fTranslate = ETranslate::kReco; break;
      case 3: fTranslate = ETranslate::kComplex; break;
    }
  }

  void Reader::TranslateEvent() {
    switch (fTranslate) {
      case ETranslate::kMc: {
        fHalMcEvent->Update(fTranslateInterface);
      } break;
      case ETranslate::kReco: {
        fHalRecoEvent->Update(fTranslateInterface);
      } break;
      case ETranslate::kComplex: {
        fHalComplexEvent->Update(fTranslateInterface);
      } break;
      default: break;
    }
  }

}  // namespace HalOTF
