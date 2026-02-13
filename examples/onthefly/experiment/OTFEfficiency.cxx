/*
 * OTFEfficiency.cxx
 *
 *  Created on: 11 lut 2026
 *      Author: daniel
 */

#include "OTFEfficiency.h"

#include <RtypesCore.h>
#include <TAxis.h>
#include <TH1.h>
#include <TLorentzVector.h>
#include <TRandom.h>

#include "ComplexTrack.h"
#include "OTFRecoEvent.h"
#include "Package.h"
#include "Track.h"

namespace HalOTF {

  EfficientyKin::~EfficientyKin() {
    if (fAcceptance) delete fAcceptance;
  }

  Bool_t EfficientyKin::Init() {
    if (!fAcceptance) return kFALSE;
    return kTRUE;
  }

  void EfficientyKin::SetHistogramSim(const TH2D& histo) {
    fMode       = eMode::kSim;
    fAcceptance = (TH2D*) histo.Clone();
  }

  void EfficientyKin::SetHistogramReco(const TH2D& histo) {
    fMode       = eMode::kReco;
    fAcceptance = (TH2D*) histo.Clone();
  }

  Hal::Package* EfficientyKin::Report() const {
    auto report = ExperimentSubTask::Report();
    report->AddObject(fAcceptance->Clone());
    return report;
  }


  void EfficiencyYPt::ProcessTrack(Hal::ComplexTrack* track) {
    Double_t pt, y;
    if (fMode == eMode::kSim) {
      pt = track->GetRealTrack()->GetMomentum().Pt();
      y  = track->GetRealTrack()->GetMomentum().Rapidity();
    } else {
      pt = track->GetImgTrack()->GetMomentum().Pt();
      y  = track->GetImgTrack()->GetMomentum().Rapidity();
    }
    auto reco     = (HalOTF::RecoTrack*) track->GetRealTrack();
    Int_t binx    = fAcceptance->GetXaxis()->FindBin(y);
    Int_t biny    = fAcceptance->GetYaxis()->FindBin(pt);
    Double_t prob = fAcceptance->GetBinContent(binx, biny);
    Double_t test = gRandom->Uniform();
    if (test <= prob) {
      switch (fDetType) {
        case 'A': {
          reco->SetNHitsA(1);
        } break;
        case 'B': {
          reco->SetNHitsB(1);
        } break;
        case 'C': {
          reco->SetNHitsC(1);
        } break;
        case ' ': {
          reco->SetNHits(1);
        } break;
      }
    } else {
      reco->SetNHits(0);
      switch (fDetType) {
        case 'A': {
          reco->SetNHitsA(0);
        } break;
        case 'B': {
          reco->SetNHitsB(0);
        } break;
        case 'C': {
          reco->SetNHitsC(0);
        } break;
        case ' ': {
          reco->SetNHits(0);
        } break;
      }
    }
  }

  void EfficiencyEtaPt::ProcessTrack(Hal::ComplexTrack* track) {
    Double_t pt, y;
    if (fMode == eMode::kSim) {
      pt = track->GetRealTrack()->GetMomentum().Pt();
      y  = track->GetRealTrack()->GetMomentum().Eta();
    } else {
      pt = track->GetImgTrack()->GetMomentum().Pt();
      y  = track->GetImgTrack()->GetMomentum().Eta();
    }
    auto reco     = (HalOTF::RecoTrack*) track->GetRealTrack();
    Int_t binx    = fAcceptance->GetXaxis()->FindBin(y);
    Int_t biny    = fAcceptance->GetYaxis()->FindBin(pt);
    Double_t prob = fAcceptance->GetBinContent(binx, biny);
    Double_t test = gRandom->Uniform();
    if (test <= prob) {
      switch (fDetType) {
        case 'A': {
          reco->SetNHitsA(1);
        } break;
        case 'B': {
          reco->SetNHitsB(1);
        } break;
        case 'C': {
          reco->SetNHitsC(1);
        } break;
        case ' ': {
          reco->SetNHits(1);
        } break;
      }
    } else {
      reco->SetNHits(0);
      switch (fDetType) {
        case 'A': {
          reco->SetNHitsA(0);
        } break;
        case 'B': {
          reco->SetNHitsB(0);
        } break;
        case 'C': {
          reco->SetNHitsC(0);
        } break;
        case ' ': {
          reco->SetNHits(0);
        } break;
      }
    }
  }

} /* namespace HalOTF */
