/*
 * CorrFitSmearingMatrixCalculator.cxx
 *
 *  Created on: 23 sty 2026
 *      Author: daniel
 */

#include "CorrFitSmearingMath.h"

#include <TAxis.h>
#include <TDatabasePDG.h>
#include <TF1.h>
#include <TH1.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TParticlePDG.h>
#include <TRandom.h>
#include <memory>
#include <vector>

#include "FemtoConst.h"
#include "FemtoPair.h"
#include "StdHist.h"
#include "StdPhys.h"

namespace Hal {

  CorrFitSmearingMath::CorrFitSmearingMath(Int_t tracks) : fNtracks(tracks) {
    fResoP2d        = new TH2D*[2];
    fResoPhi2d      = new TH2D*[2];
    fResoTheta2d    = new TH2D*[2];
    fResoP2d[0]     = nullptr;
    fResoP2d[1]     = nullptr;
    fResoPhi2d[0]   = nullptr;
    fResoPhi2d[1]   = nullptr;
    fResoTheta2d[0] = nullptr;
    fResoTheta2d[1] = nullptr;
  }

  void CorrFitSmearingMath::SetAxis(Int_t bins, Double_t lo, Double_t hi) {
    fBins = bins;
    fLow  = lo;
    fHigh = hi;
  }

  void CorrFitSmearingMath::SetYield(TH2D& yield, Int_t pid, Int_t type) {
    if (type == 0) {
      fYield1 = (TH2D*) yield.Clone();
      fPdg1   = pid;
      fM1     = TDatabasePDG::Instance()->GetParticle(pid)->Mass();
    } else {
      fYield2 = (TH2D*) yield.Clone();
      fPdg2   = pid;
      fM2     = TDatabasePDG::Instance()->GetParticle(pid)->Mass();
    }
  }

  void CorrFitSmearingMath::Calculate(Int_t nEvents) {
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
    fPSim1 = new TLorentzVector[fNtracks];
    fPSim2 = new TLorentzVector[fNtracks];
    fPRec1 = new TLorentzVector[fNtracks];
    fPRec2 = new TLorentzVector[fNtracks];

    TLorentzVector *tracks_raw1, *tracks_raw2;
    TLorentzVector *tracks_sme1, *tracks_sme2;
    if (fExpYields) {
      tracks_raw1 = fPRec1;
      tracks_raw2 = fPRec2;
      tracks_sme1 = fPSim1;
      tracks_sme2 = fPSim2;
    } else {
      tracks_raw1 = fPSim1;
      tracks_raw2 = fPSim2;
      tracks_sme1 = fPRec1;
      tracks_sme2 = fPRec2;
    }


    if (fPdg1 == fPdg2 || fYield2 == nullptr) {
      for (int nEv = 0; nEv < nEvents; nEv++) {
        GenerateTracks(tracks_raw1, 0);
        if (fUseFunc) {
          MakeSmearTracksTF(tracks_sme1, tracks_raw1, 0);
        } else {
          MakeSmearTracksTH(tracks_sme1, tracks_raw1, 0);
        }
        for (int i = 0; i < fNtracks; i++) {
          for (int j = i + 1; j < fNtracks; j++) {
            pair_re->SetMomenta(fPRec1[i], fPRec1[j]);
            if (!CheckPair(pair_re.get())) continue;
            pair_re->Compute();
            pair_mc->SetMomenta(fPSim1[i], fPSim1[j]);
            pair_mc->Compute();
            fOutput->Fill(TMath::Abs(pair_mc->GetT()), TMath::Abs(pair_re->GetT()));
          }
        }
      }
    } else {
      for (int nEv = 0; nEv < nEvents; nEv++) {
        GenerateTracks(tracks_raw1, 0);
        GenerateTracks(tracks_raw2, 1);
        if (fUseFunc) {
          MakeSmearTracksTF(tracks_sme1, tracks_raw1, 0);
          MakeSmearTracksTF(tracks_sme2, tracks_raw2, 1);
        } else {
          MakeSmearTracksTH(tracks_sme1, tracks_raw1, 0);
          MakeSmearTracksTH(tracks_sme2, tracks_raw2, 1);
        }
        for (int i = 0; i < fNtracks; i++) {
          for (int j = 0; j < fNtracks; j++) {
            pair_re->SetMomenta(fPRec1[i], fPRec2[j]);
            if (!CheckPair(pair_re.get())) continue;
            pair_re->Compute();
            pair_mc->SetMomenta(fPSim1[i], fPSim2[j]);
            pair_mc->Compute();

            fOutput->Fill(TMath::Abs(pair_mc->GetT()), TMath::Abs(pair_re->GetT()));
          }
        }
      }
    }
    delete[] fPSim1;
    delete[] fPSim2;
    delete[] fPRec1;
    delete[] fPRec2;
    fPSim1 = nullptr;
    fPSim2 = nullptr;
    fPRec1 = nullptr;
    fPRec2 = nullptr;
  }

  void CorrFitSmearingMath::GenerateTracks(TLorentzVector* tracks, Int_t pid) {
    Double_t m = 0;
    TH2D* h;
    switch (pid) {
      case 0: {
        m = fM1;
        h = fYield1;
      } break;
      case 1: {
        m = fM2;
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

  void CorrFitSmearingMath::MakeSmearTracksTH(TLorentzVector* smeared_tracks,
                                                          TLorentzVector* unsmeared_tracks,
                                                          Int_t type) {
    Double_t m = fM1;
    if (type == 1) m = fM2;
    for (int i = 0; i < fNtracks; i++) {
      auto& vec          = unsmeared_tracks[i];
      Double_t p         = vec.P();
      Double_t phi       = vec.Phi();
      Double_t theta     = vec.Theta();
      Int_t pBin         = fResoP2d[type]->GetXaxis()->FindBin(p);
      Double_t p_sim     = p + fResoP[type][pBin]->GetRandom();
      Double_t phi_sim   = phi + fResoPhi[type][pBin]->GetRandom();
      Double_t theta_sim = theta + fResoTheta[type][pBin]->GetRandom();
      double pt_sim      = p_sim * TMath::Sin(theta_sim);
      double px_sim      = pt_sim * TMath::Cos(phi_sim);
      double py_sim      = pt_sim * TMath::Sin(phi_sim);
      double pz_sim      = p_sim * TMath::Cos(theta_sim);
      smeared_tracks[i].SetXYZM(px_sim, py_sim, pz_sim, m);
    }
  }

  void CorrFitSmearingMath::MakeSmearTracksTF(TLorentzVector* smeared_tracks,
                                                          TLorentzVector* unsmeared_tracks,
                                                          Int_t type) {
    Double_t m = fM1;
    if (type == 1) m = fM2;
    for (int i = 0; i < fNtracks; i++) {
      auto& vec          = unsmeared_tracks[i];
      Double_t p         = vec.P();
      Double_t phi       = vec.Phi();
      Double_t theta     = vec.Theta();
      Double_t p_sim     = p + gRandom->Gaus(0, fFuncP[type]->Eval(p));
      Double_t phi_sim   = phi + gRandom->Gaus(0, fFuncPhi[type]->Eval(p));
      Double_t theta_sim = theta + gRandom->Gaus(0, fFuncTheta[type]->Eval(p));
      double pt_sim      = p_sim * TMath::Sin(theta_sim);
      double px_sim      = pt_sim * TMath::Cos(phi_sim);
      double py_sim      = pt_sim * TMath::Sin(phi_sim);
      double pz_sim      = p_sim * TMath::Cos(theta_sim);
      smeared_tracks[i].SetXYZM(px_sim, py_sim, pz_sim, m);
    }
  }


  void CorrFitSmearingMath::SetResolution(const TH2D& p, const TH2D& phi, const TH2D& theta, Int_t pid) {
    if (pid < 0 || pid > 1) return;
    if (fResoP2d[pid]) Clean(pid);
    fResoP2d[pid]     = (TH2D*) p.Clone();
    fResoPhi2d[pid]   = (TH2D*) phi.Clone();
    fResoTheta2d[pid] = (TH2D*) theta.Clone();
    for (int i = 0; i <= fResoP2d[pid]->GetNbinsX() + 1; i++) {
      auto pp     = Hal::Std::GetProjection1D(fResoP2d[pid], i, i, "bins+y");
      auto pphi   = Hal::Std::GetProjection1D(fResoPhi2d[pid], i, i, "bins+y");
      auto ptheta = Hal::Std::GetProjection1D(fResoTheta2d[pid], i, i, "bins+y");
      fResoP[pid].push_back(pp);
      fResoPhi[pid].push_back(pphi);
      fResoTheta[pid].push_back(ptheta);
    }
    fUseFunc = kFALSE;
  }

  void CorrFitSmearingMath::Clean(Int_t type) {
    if (fResoP2d[type]) delete fResoP2d[type];
    if (fResoPhi2d[type]) delete fResoPhi2d[type];
    if (fResoTheta2d[type]) delete fResoTheta2d[type];
    for (auto i : fResoP[type])
      delete i;
    for (auto i : fResoPhi[type])
      delete i;
    for (auto i : fResoTheta[type])
      delete i;
    fResoP[type].clear();
    fResoPhi[type].clear();
    fResoTheta[type].clear();
    fResoP2d[type]     = nullptr;
    fResoTheta2d[type] = nullptr;
    fResoPhi2d[type]   = nullptr;
  }

  void CorrFitSmearingMath::SetResolution(const TF1& p, const TF1& phi, const TF1& theta, Int_t type) {
    if (type < 0 || type > 1) return;
    if (fFuncP[type]) {
      delete fFuncP[type];
      delete fFuncPhi[type];
      delete fFuncTheta[type];
    }
    fFuncP[type]     = (TF1*) p.Clone();
    fFuncPhi[type]   = (TF1*) phi.Clone();
    fFuncTheta[type] = (TF1*) theta.Clone();
    fUseFunc         = kTRUE;
  }

  CorrFitSmearingMath::~CorrFitSmearingMath() {
    Clean(0);
    Clean(1);

    delete[] fResoP2d;
    delete[] fResoPhi2d;
    delete[] fResoTheta2d;
    for (int i = 0; i < 2; i++) {
      if (fFuncP[i]) delete fFuncP[i];
      if (fFuncPhi[i]) delete fFuncPhi[i];
      if (fFuncTheta[i]) delete fFuncTheta[i];
    }
  }


} /* namespace Hal */
