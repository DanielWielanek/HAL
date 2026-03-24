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

  void EfficientyKin::ChangeHits(HalOTF::RecoTrack* reco, Int_t flag) const {
    switch (fDetType) {
      case 'A': {
        reco->SetNHitsA(flag);
      } break;
      case 'B': {
        reco->SetNHitsB(flag);
      } break;
      case 'C': {
        reco->SetNHitsC(flag);
      } break;
      case ' ': {
        reco->SetNHits(flag);
      } break;
    }
  }

  void EfficientyKin::SetHistogram(const TH2D& histo) { fAcceptance = (TH2D*) histo.Clone(); }

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
      ChangeHits(reco, 1);
    } else {
      ChangeHits(reco, 0);
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
      ChangeHits(reco, 1);
    } else {
      ChangeHits(reco, 0);
    }
  }

} /* namespace HalOTF */
