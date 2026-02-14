/*
 * OTFMisidentification.cxx
 *
 *  Created on: 13 lut 2026
 *      Author: daniel
 */

#include <RtypesCore.h>
#include <TAxis.h>
#include <TH1.h>
#include <TRandom.h>
#include <TString.h>
#include <stddef.h>

#include "ComplexTrack.h"
#include "Const.h"
#include "Cout.h"
#include "McTrack.h"
#include "OTFMisidentification.h"
#include "OTFRecoEvent.h"
#include "StdHist.h"
#include "StdTypes.h"

namespace HalOTF {

  void MisidentificationKin::AddPidHypo(const TH2D& h, Int_t pid) {
    if (fHypos.size() > 0) {
      auto first = fHypos[0];
      if (!Hal::Std::AreSimilar(first, &h, true)) {
        Hal::Cout::PrintInfo(Form("%s/AddPidHypo incompatible histograms", ClassName()), Hal::EInfo::kLowWarning);
        return;
      }
    }
    fHypos.push_back((TH2D*) h.Clone());
    fPids.push_back(pid);
  }

  void MisidentificationKin::Normalize() {
    if (fHypos.size() == 0) return;
    auto sum = (TH2D*) fHypos[0]->Clone();
    for (size_t i = 1; i < fHypos.size(); i++) {
      sum->Add(fHypos[i]);
    }
    for (int i = 0; i <= sum->GetNbinsX() + 1; i++) {
      for (int j = 0; j <= sum->GetNbinsY() + 1; j++) {
        if (sum->GetBinContent(i, j) == 0) sum->SetBinContent(i, j, 1);  // to avoid division by zero
      }
    }
    for (size_t i = 0; i < fHypos.size(); i++) {
      fHypos[i]->Divide(sum);
    }
    for (size_t i = 1; i < fHypos.size(); i++) {
      fHypos[i]->Add(fHypos[i - 1]);
    }
    int last = fHypos.size() - 1;
    for (int i = 0; i <= fHypos[last]->GetNbinsX() + 1; i++) {
      for (int j = 0; j <= fHypos[last]->GetNbinsY() + 1; j++) {
        fHypos[last]->SetBinContent(i, j, 2);
      }
    }
    delete sum;
  }

  void MisidentificationKin::AddMisidenfiedAuto() {
    if (fHypos.size() == 0) return;
    auto sum = (TH2D*) fHypos[0]->Clone();
    for (size_t i = 1; i < fHypos.size(); i++) {
      sum->Add(fHypos[i]);
    }
    auto contamination = (TH2D*) fHypos[0]->Clone();
    contamination->Reset();
    for (int i = 0; i <= sum->GetNbinsX() + 1; i++) {
      for (int j = 0; j <= sum->GetNbinsY() + 1; j++) {
        contamination->SetBinContent(i, j, 1.0 - sum->GetBinContent(i, j));
      }
    }
    AddPidHypo(*contamination, Hal::Const::DummyPID());
    delete contamination;
    delete sum;
  }

  Int_t MisidentificationKin::GetNewPid(Double_t x, Double_t y) const {
    Int_t binX       = fHypos[0]->GetXaxis()->FindBin(x);
    Int_t binY       = fHypos[0]->GetYaxis()->FindBin(y);
    Double_t randval = gRandom->Uniform();
    Int_t count      = 0;
    while (randval > fHypos[count]->GetBinContent(binX, binY)) {
      count++;
    }
    return fPids[count];
  }

  MisidentificationKin::~MisidentificationKin() {
    for (auto h : fHypos)
      delete h;
  }

  Bool_t MisidentificationKin::Init() {
    if (fHypos.size() == 0) return false;
    Normalize();
    return true;
  }

  void MisidentificationYPt::ProcessTrack(Hal::ComplexTrack* track) {
    Hal::Track* tr          = nullptr;
    Hal::McTrack* sim       = (Hal::McTrack*) track->GetImgTrack();
    HalOTF::RecoTrack* reco = (HalOTF::RecoTrack*) track->GetRealTrack();
    if (fMode == eMode::kReco) {
      tr = track->GetRealTrack();
    } else {
      tr = track->GetImgTrack();
    }
    Int_t newPid = GetNewPid(tr->GetMomentum().Rapidity(), tr->GetMomentum().Pt());
    reco->SetPidHypo(newPid);
  }

  void MisidentificationEtaPt::ProcessTrack(Hal::ComplexTrack* track) {
    Hal::Track* tr          = nullptr;
    Hal::McTrack* sim       = (Hal::McTrack*) track->GetImgTrack();
    HalOTF::RecoTrack* reco = (HalOTF::RecoTrack*) track->GetRealTrack();
    if (fMode == eMode::kReco) {
      tr = track->GetRealTrack();
    } else {
      tr = track->GetImgTrack();
    }
    Int_t newPid = GetNewPid(tr->GetMomentum().Eta(), tr->GetMomentum().Pt());
    reco->SetPidHypo(newPid);
  }

} /* namespace HalOTF */
