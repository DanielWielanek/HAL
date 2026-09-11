/*
 * CorrFitSmearingMath1DCF.cxx
 *
 *  Created on: 11 wrz 2026
 *      Author: daniel
 */

#include "CorrFitSmearingMath1DCF.h"

#include <TH2.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TMathBase.h>

#include "Cout.h"
#include "FemtoConst.h"
#include "FemtoPair.h"
#include "LorentzSmearing.h"
#include "StdOpenMP.h"
#include "StdTypes.h"

namespace Hal {


  CorrFitSmearingMath1DCF::CorrFitSmearingMath1DCF(Int_t tracks) : fNtracks(tracks) {}

  Bool_t CorrFitSmearingMath1DCF::CheckPair(Hal::FemtoPair* pair) const {
    Double_t pxT = pair->GetPx1() + pair->GetPx2();
    Double_t pyT = pair->GetPy1() + pair->GetPy2();
    Double_t kt  = TMath::Sqrt(pxT * pxT + pyT * pyT) * 0.5;
    if (kt < fKtCut[0]) return false;
    if (kt > fKtCut[1]) return false;
    return true;
  }

  void CorrFitSmearingMath1DCF::DoMathSingleCore(Int_t nEvents, LorentzVectors data) {
    Int_t threads           = Hal::Std::OpenMP::GetMaxThreads();
    Hal::FemtoPair* pair_re = Hal::Femto::MakePair(Hal::Femto::EKinematics::kPRF, false);
    Hal::FemtoPair* pair_mc = Hal::Femto::MakePair(Hal::Femto::EKinematics::kPRF, true);
    pair_re->SetPdgs(fPdg1, fPdg2);
    pair_mc->SetPdgs(fPdg1, fPdg2);
    pair_re->Init(-1);
    pair_mc->Init(-1);

    if (fId) {
      for (int nEv = 0; nEv < nEvents; nEv++) {
        GenerateTracks(data.tracks_raw1, 0);
        for (int i = 0; i < fNtracks; i++) {
          fSmearing1->Smear(data.tracks_sme1[i], data.tracks_raw1[i]);
        }
        for (int i = 0; i < fNtracks; i++) {
          for (int j = i + 1; j < fNtracks; j++) {
            pair_re->SetMomenta(data.PRec1[i], data.PRec1[j]);
            if (!CheckPair(pair_re)) continue;
            pair_re->Compute();
            pair_mc->SetMomenta(data.PSim1[i], data.PSim1[j]);
            pair_mc->Compute();
            fOutput->Fill(TMath::Abs(pair_mc->GetT()), TMath::Abs(pair_re->GetT()));
          }
        }
      }
    } else {
      for (int nEv = 0; nEv < nEvents; nEv++) {
        GenerateTracks(data.tracks_raw1, 0);
        GenerateTracks(data.tracks_raw2, 1);
        for (int i = 0; i < fNtracks; i++) {
          fSmearing1->Smear(data.tracks_sme1[i], data.tracks_raw1[i]);
          fSmearing2->Smear(data.tracks_sme2[i], data.tracks_raw2[i]);
        }
        for (int i = 0; i < fNtracks; i++) {
          for (int j = 0; j < fNtracks; j++) {
            pair_re->SetMomenta(data.PRec1[i], data.PRec2[j]);
            if (!CheckPair(pair_re)) continue;
            pair_re->Compute();
            pair_mc->SetMomenta(data.PSim1[i], data.PSim2[j]);
            pair_mc->Compute();
            fOutput->Fill(TMath::Abs(pair_mc->GetT()), TMath::Abs(pair_re->GetT()));
          }
        }
      }
    }
    delete pair_re;
    delete pair_mc;
  }

  void CorrFitSmearingMath1DCF::DoMathMT(Int_t nEvents, LorentzVectors data) {
    const int threads        = Hal::Std::OpenMP::GetMaxThreads();
    Hal::FemtoPair** pair_re = new Hal::FemtoPair*[threads];
    Hal::FemtoPair** pair_mc = new Hal::FemtoPair*[threads];
    TH2D** outputs           = new TH2D*[threads];
    for (int i = 0; i < threads; i++) {
      pair_re[i] = Hal::Femto::MakePair(Hal::Femto::EKinematics::kPRF, false);
      pair_mc[i] = Hal::Femto::MakePair(Hal::Femto::EKinematics::kPRF, true);
      pair_re[i]->SetPdgs(fPdg1, fPdg2);
      pair_mc[i]->SetPdgs(fPdg1, fPdg2);
      pair_re[i]->Init(-1);
      pair_mc[i]->Init(-1);
      outputs[i] = (TH2D*) fOutput->Clone();
    }

    if (fId) {
      for (int nEv = 0; nEv < nEvents; nEv++) {
        GenerateTracks(data.tracks_raw1, 0);
        for (int i = 0; i < fNtracks; i++) {
          fSmearing1->Smear(data.tracks_sme1[i], data.tracks_raw1[i]);
        }
#pragma omp parallel
        {
          int thread  = Hal::Std::OpenMP::GetThreadNum();
          auto pairRe = pair_re[thread];
          auto pairMc = pair_mc[thread];
          auto output = outputs[thread];
#pragma omp for schedule(static)
          for (int i = 0; i < fNtracks; i++) {
            for (int j = i + 1; j < fNtracks; j++) {
              pairRe->SetMomenta(data.PRec1[i], data.PRec1[j]);
              if (!CheckPair(pairRe)) continue;
              pairRe->Compute();
              pairMc->SetMomenta(data.PSim1[i], data.PSim1[j]);
              pairMc->Compute();
              output->Fill(TMath::Abs(pairMc->GetT()), TMath::Abs(pairRe->GetT()));
            }
          }
        }  // MT
      }
    } else {
      for (int nEv = 0; nEv < nEvents; nEv++) {
        GenerateTracks(data.tracks_raw1, 0);
        GenerateTracks(data.tracks_raw2, 1);
        for (int i = 0; i < fNtracks; i++) {
          fSmearing1->Smear(data.tracks_sme1[i], data.tracks_raw1[i]);
          fSmearing2->Smear(data.tracks_sme2[i], data.tracks_raw2[i]);
        }
#pragma omp parallel
        {
          int thread  = Hal::Std::OpenMP::GetThreadNum();
          auto pairRe = pair_re[thread];
          auto pairMc = pair_mc[thread];
          auto output = outputs[thread];
#pragma omp for schedule(static)
          for (int i = 0; i < fNtracks; i++) {
            for (int j = 0; j < fNtracks; j++) {
              pairRe->SetMomenta(data.PRec1[i], data.PRec2[j]);
              if (!CheckPair(pairRe)) continue;
              pairRe->Compute();
              pairMc->SetMomenta(data.PSim1[i], data.PSim2[j]);
              pairMc->Compute();
              output->Fill(TMath::Abs(pairMc->GetT()), TMath::Abs(pairRe->GetT()));
            }
          }
        }  // MT
      }
    }
    for (int i = 0; i < threads; i++) {
      delete pair_re[i];
      delete pair_mc[i];
      fOutput->Add(outputs[i]);
      delete outputs[i];
    }
    delete[] pair_re;
    delete[] pair_mc;
    delete[] outputs;
  }

  void CorrFitSmearingMath1DCF::SetAxis(Int_t bins, Double_t lo, Double_t hi) {
    fBins = bins;
    fLow  = lo;
    fHigh = hi;
  }

  Bool_t CorrFitSmearingMath1DCF::Init() {
    if (fId) {
      if (!fSmearing1) {
        Hal::Cout::PrintInfo("CorrFitSmearingMath no smearing algo for first particle !", EInfo::kError);
        return false;
      }
      if (!fSmearing1->Init()) return false;
      fPdg1 = fSmearing1->GetPid();
      fPdg1 = fPdg2;
    } else {
      if (!fSmearing1) {
        Hal::Cout::PrintInfo("CorrFitSmearingMath no smearing algo for first particle !", EInfo::kError);
        return false;
      }
      if (!fSmearing1->Init()) return false;
      fPdg1 = fSmearing1->GetPid();
      if (!fSmearing2) {
        Hal::Cout::PrintInfo("CorrFitSmearingMath no smearing algo for second particle !", EInfo::kError);
        return false;
      }
      if (!fSmearing2->Init()) return false;
      fPdg2 = fSmearing2->GetPid();
      if (fPdg1 == fPdg2) {
        Hal::Cout::PrintInfo("CorrFitSmearingMath two smearing algos detected but both for same PID !", EInfo::kError);
        return false;
      }
    }
    fOutput =
      new TH2D("smearing_matrix", "smearing_matrix;k*_{sim} [GeV/c];k*_{reco} [GeV/c]", fBins, fLow, fHigh, fBins, fLow, fHigh);
    return true;
  }

  void CorrFitSmearingMath1DCF::Calculate(Int_t nEvents) {
    if (!Init()) return;

    LorentzVectors data;
    data.Init(fExpYields, fNtracks);
    if (fOpenMP && Std::OpenMP::HasMP()) {
      DoMathMT(nEvents, data);
    } else {
      DoMathSingleCore(nEvents, data);
    }
    data.Destroy();
  }

  void CorrFitSmearingMath1DCF::SetSmearingAlgo(const Hal::LorentzSmearing& smear) { fSmearing1 = smear.MakeCopy(); }

  void CorrFitSmearingMath1DCF::SetSmearingAlgo(const Hal::LorentzSmearing& smear1, const Hal::LorentzSmearing& smear2) {
    fSmearing1 = smear1.MakeCopy();
    fSmearing2 = smear2.MakeCopy();
    fId        = false;
  }

  CorrFitSmearingMath1DCF::~CorrFitSmearingMath1DCF() {
    if (fSmearing1 == fSmearing2) {
      if (fSmearing1) delete fSmearing1;
      return;
    }
    if (fSmearing1) delete fSmearing1;
    if (fSmearing2) delete fSmearing2;
  }


} /* namespace Hal */

void Hal::CorrFitSmearingMath1DCF::LorentzVectors::Init(bool expFields, int tracksNo) {
  PSim1 = new TLorentzVector[tracksNo];
  PSim2 = new TLorentzVector[tracksNo];
  PRec1 = new TLorentzVector[tracksNo];
  PRec2 = new TLorentzVector[tracksNo];
  if (expFields) {
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
}

void Hal::CorrFitSmearingMath1DCF::LorentzVectors::Destroy() {
  delete[] PSim1;
  delete[] PSim2;
  delete[] PRec1;
  delete[] PRec2;
}
