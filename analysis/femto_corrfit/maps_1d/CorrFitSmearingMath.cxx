/*
 * CorrFitSmearingMatrixCalculator.cxx
 *
 *  Created on: 23 sty 2026
 *      Author: daniel
 */

#include "CorrFitSmearingMath.h"

#include <TLorentzVector.h>
#include <TMath.h>
#include <TMathBase.h>
#include <memory>

#include "Cout.h"
#include "FemtoConst.h"
#include "FemtoPair.h"
#include "LorentzSmearing.h"
#include "StdPhys.h"
#include "StdTypes.h"

namespace Hal {

  CorrFitSmearingMath::CorrFitSmearingMath(Int_t tracks) : fNtracks(tracks) {}

  void CorrFitSmearingMath::SetAxis(Int_t bins, Double_t lo, Double_t hi) {
    fBins = bins;
    fLow  = lo;
    fHigh = hi;
  }

  void CorrFitSmearingMath::SetYield(TH2D& yield, Int_t type) {
    if (type == 0) {
      fYield1 = (TH2D*) yield.Clone();
    } else {
      fYield2 = (TH2D*) yield.Clone();
    }
  }

  void CorrFitSmearingMath::Calculate(Int_t nEvents) {
    if (!fYield1) {
      Hal::Cout::PrintInfo("CorrFitSmearingMath no yield for first particle !", EInfo::kError);
      return;
    }
    if (!fSmearing1) {
      Hal::Cout::PrintInfo("CorrFitSmearingMath no smearing algo for first particle !", EInfo::kError);
      return;
    }
    if (!fSmearing1->Init()) return;
    fPdg1 = fSmearing1->GetPid();
    if (fYield2 == nullptr && fSmearing2 == nullptr) {  // identical
      fPdg2      = fSmearing1->GetPid();
      fSmearing2 = fSmearing1;
    } else {
      if (!fYield2) {
        Hal::Cout::PrintInfo("CorrFitSmearingMath no yield for second particle !", EInfo::kError);
        return;
      }
      if (!fSmearing2) {
        Hal::Cout::PrintInfo("CorrFitSmearingMath no smearing algo for second particle !", EInfo::kError);
        return;
      }
      if (!fSmearing2->Init()) return;
      fPdg2 = fSmearing2->GetPid();
    }


    fOutput =
      new TH2D("smearing_matrix", "smearing_matrix;k*_{sim} [GeV/c];k*_{reco} [GeV/c]", fBins, fLow, fHigh, fBins, fLow, fHigh);
    auto pair_re = std::unique_ptr<Hal::FemtoPair>(Hal::Femto::MakePair(Hal::Femto::EKinematics::kPRF, false));
    auto pair_mc = std::unique_ptr<Hal::FemtoPair>(Hal::Femto::MakePair(Hal::Femto::EKinematics::kPRF, true));
    pair_re->SetPdg1(fPdg1);
    pair_re->SetPdg2(fPdg2);
    pair_mc->SetPdg1(fPdg1);
    pair_mc->SetPdg2(fPdg2);
    pair_re->Init(-1);
    pair_mc->Init(-1);
    auto PSim1 = new TLorentzVector[fNtracks];
    auto PSim2 = new TLorentzVector[fNtracks];
    auto PRec1 = new TLorentzVector[fNtracks];
    auto PRec2 = new TLorentzVector[fNtracks];

    TLorentzVector *tracks_raw1, *tracks_raw2;
    TLorentzVector *tracks_sme1, *tracks_sme2;
    if (fExpYields) {
      tracks_raw1 = PRec1;
      tracks_raw2 = PRec2;
      tracks_sme1 = PSim1;
      tracks_sme2 = PSim2;
    } else {
      tracks_raw1 = PSim1;
      tracks_raw2 = PSim2;
      tracks_sme1 = PRec1;
      tracks_sme2 = PRec2;
    }


    if (fPdg1 == fPdg2) {
      for (int nEv = 0; nEv < nEvents; nEv++) {
        GenerateTracks(tracks_raw1, 0);
        for (int i = 0; i < fNtracks; i++) {
          fSmearing1->Smear(tracks_sme1[i], tracks_raw1[i]);
        }
        for (int i = 0; i < fNtracks; i++) {
          for (int j = i + 1; j < fNtracks; j++) {
            pair_re->SetMomenta(PRec1[i], PRec1[j]);
            if (!CheckPair(pair_re.get())) continue;
            pair_re->Compute();
            pair_mc->SetMomenta(PSim1[i], PSim1[j]);
            pair_mc->Compute();
            fOutput->Fill(TMath::Abs(pair_mc->GetT()), TMath::Abs(pair_re->GetT()));
          }
        }
      }
    } else {
      for (int nEv = 0; nEv < nEvents; nEv++) {
        GenerateTracks(tracks_raw1, 0);
        GenerateTracks(tracks_raw2, 1);
        for (int i = 0; i < fNtracks; i++) {
          fSmearing1->Smear(tracks_sme1[i], tracks_raw1[i]);
          fSmearing2->Smear(tracks_sme2[i], tracks_raw2[i]);
        }
        for (int i = 0; i < fNtracks; i++) {
          for (int j = 0; j < fNtracks; j++) {
            pair_re->SetMomenta(PRec1[i], PRec2[j]);
            if (!CheckPair(pair_re.get())) continue;
            pair_re->Compute();
            pair_mc->SetMomenta(PSim1[i], PSim2[j]);
            pair_mc->Compute();
            fOutput->Fill(TMath::Abs(pair_mc->GetT()), TMath::Abs(pair_re->GetT()));
          }
        }
      }
    }
    delete[] PSim1;
    delete[] PSim2;
    delete[] PRec1;
    delete[] PRec2;
  }

  void CorrFitSmearingMath::GenerateTracks(TLorentzVector* tracks, Int_t pid) {
    Double_t m = 0;
    TH2D* h    = nullptr;
    switch (pid) {
      case 0: {
        m = fSmearing1->GetMass();
        h = fYield1;
      } break;
      case 1: {
        m = fSmearing2->GetMass();
        h = fYield2;
      } break;
    }
    switch (fMode) {
      case EMode::kEtaPt: {
        for (int i = 0; i < fNtracks; i++) {
          Double_t pt, eta;
          h->GetRandom2(eta, pt);
          tracks[i] = Hal::Std::LorentzGenerator::PtEtaM(pt, eta, m);
        }
      } break;
      case EMode::kYPt: {
        for (int i = 0; i < fNtracks; i++) {
          Double_t pt, eta;
          h->GetRandom2(eta, pt);
          tracks[i] = Hal::Std::LorentzGenerator::PtYM(pt, eta, m);
        }
      } break;
    }
  }

  Bool_t CorrFitSmearingMath::CheckPair(Hal::FemtoPair* pair) const {
    Double_t pxT = pair->GetPx1() + pair->GetPx2();
    Double_t pyT = pair->GetPy1() + pair->GetPy2();
    Double_t kt  = TMath::Sqrt(pxT * pxT + pyT * pyT) * 0.5;
    if (kt < fKtCut[0]) return false;
    if (kt > fKtCut[1]) return false;
    return true;
  }

  void CorrFitSmearingMath::SetSmearingAlgo(const Hal::LorentzSmearing& smear, Int_t type) {
    if (type == 0)
      fSmearing1 = smear.MakeCopy();
    else if (type == 1)
      fSmearing2 = smear.MakeCopy();
  }

  CorrFitSmearingMath::~CorrFitSmearingMath() {
    if (fYield1) delete fYield1;
    if (fYield2) delete fYield2;
    if (fSmearing1 == fSmearing2) {
      if (fSmearing1) delete fSmearing1;
      return;
    }
    if (fSmearing1) delete fSmearing1;
    if (fSmearing2) delete fSmearing2;
  }


} /* namespace Hal */
