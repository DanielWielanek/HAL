/*
 * CorrFitPairGeneratorYPtKt.cpp
 *
 *  Created on: 10 kwi 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitPairGeneratorYPtKt.h"

#include "FemtoCorrFuncKt.h"
#include "FemtoMiniPair.h"

#include <RtypesCore.h>
#include <TAxis.h>
#include <TClonesArray.h>
#include <TDatabasePDG.h>
#include <TFile.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TObjArray.h>
#include <TParticlePDG.h>
#include <TRandom.h>
#include <TTree.h>

namespace Hal {
  CorrFitPairGeneratorYPtKt::CorrFitPairGeneratorYPtKt() : f2Kt2 {0, 1E+9} {}

  void CorrFitPairGeneratorYPtKt::SetHist(const TH2D& hist1, const TH2D& hist2) {
    hist1.Copy(fHist1);
    hist2.Copy(fHist2);
    fHist1.SetName("FirstParticle");
    fHist2.SetName("SecondParticle");
    fHist1.SetDirectory(nullptr);
    fHist2.SetDirectory(nullptr);
  }

  Bool_t CorrFitPairGeneratorYPtKt::Init() {
    bool val = CorrFitPairGenerator::Init();
    if (!val) return val;
    fHist1.ResetStats();
    fHist2.ResetStats();
    auto kt = dynamic_cast<Hal::FemtoCorrFuncKt*>(fCF);
    if (kt) {
      auto ktArr = kt->GetRange();
      f2Kt2[0]   = ktArr[0];
      f2Kt2[1]   = ktArr[ktArr.GetSize() - 1];
      f2Kt2[0]   = f2Kt2[0] * f2Kt2[0] * 4;
      f2Kt2[1]   = f2Kt2[1] * f2Kt2[1] * 4;
    }
    if (fHist1.GetEntries() <= 0) return kFALSE;
    if (fHist2.GetEntries() <= 0) { fHist2 = fHist1; }
    return kTRUE;
  }

  CorrFitPairGeneratorYPtKt::~CorrFitPairGeneratorYPtKt() {}

  void CorrFitPairGeneratorYPtKt::GenerateEvent() {
    switch (fGroupingFlag) {
      case EGrouping::kOneDim: {
        int size = fLimitsN.GetSize() - 1;
        for (int i = 0; i < 10000; i++) {
          int bin = GeneratePairOneDim();
          if (bin < 1) continue;
          if (fLimitsN[bin] > fBinLimit) continue;
          auto array = fSignalPairs[bin - 1];
          auto pair  = (FemtoMicroPair*) array->ConstructedAt(array->GetEntriesFast());
          fLimitsN.IncrementAfter(bin);
          *pair = fPair;
        }
      } break;
      case EGrouping::kThreeDim: {
        for (int i = 0; i < 10000; i++) {
          auto bin = GeneratePairThreeDim();
          if (bin.x < 1) continue;
          if (bin.y < 1) continue;
          if (bin.z < 1) continue;
          if (fLimits3D[bin.x][bin.y][bin.z] > fBinLimit) continue;
          auto array = fSignalPairs[bin.z - 1];
          auto pair  = (FemtoMicroPair*) array->ConstructedAt(array->GetEntriesFast());
          fLimits3D.IncrementAfter(bin.x, bin.y, bin.z);
          *pair = fPair;
        }
      } break;
    }
  }

  Int_t CorrFitPairGeneratorYPtKt::GeneratePairOneDim() {
    GenerateFreeRandomPair();
    *fHbtPair = fPair;
    fHbtPair->Compute();
    if (fAbs) { return fXaxis.GetBin(TMath::Abs(fHbtPair->GetT())); }
    return fXaxis.GetBin(fHbtPair->GetT());
  }


  CorrFitPairGeneratorYPtKt::fourVect CorrFitPairGeneratorYPtKt::GeneratePairThreeDim() {
    GenerateFreeRandomPair();
    *fHbtPair = fPair;
    fHbtPair->Compute();
    fourVect res;
    if (fAbs) {
      res.t = fXaxis.GetBin(TMath::Abs(fHbtPair->GetT()));
      res.x = fXaxis.GetBin(TMath::Abs(fHbtPair->GetX()));
      res.y = fYaxis.GetBin(TMath::Abs(fHbtPair->GetY()));
      res.z = fZaxis.GetBin(TMath::Abs(fHbtPair->GetZ()));
    } else {
      res.t = fXaxis.GetBin(fHbtPair->GetT());
      res.x = fXaxis.GetBin(fHbtPair->GetX());
      res.y = fYaxis.GetBin(fHbtPair->GetY());
      res.z = fZaxis.GetBin(fHbtPair->GetZ());
    }
    return res;
  }

  void CorrFitPairGeneratorYPtKt::GenerateFreeRandomPair() {
    bool bad_pair = true;
    do {  // generate pairs until match kT bins
      Double_t pt1, pt2, y1, y2;
      fHist1.GetRandom2(y1, pt1);
      fHist2.GetRandom2(y2, pt2);
      Double_t phi1 = gRandom->Uniform(-TMath::Pi(), TMath::Pi());
      Double_t phi2 = gRandom->Uniform(-TMath::Pi(), TMath::Pi());
      Double_t px1  = pt1 * TMath::Cos(phi1);
      Double_t px2  = pt2 * TMath::Cos(phi2);
      Double_t py1  = pt1 * TMath::Sin(phi1);
      Double_t py2  = pt2 * TMath::Sin(phi2);
      Double_t et1  = TMath::Sqrt(pt1 * pt1 + fM1 * fM1);
      Double_t et2  = TMath::Sqrt(pt2 * pt2 + fM2 * fM2);
      Double_t pz1  = TMath::Sign(et1 * 0.5, y1) * TMath::Exp(-y1) * (TMath::Exp(2.0 * y1) - 1.0);
      Double_t pz2  = TMath::Sign(et2 * 0.5, y2) * TMath::Exp(-y2) * (TMath::Exp(2.0 * y2) - 1.0);
      Double_t e1   = TMath::Sqrt(et1 * et1 + pz1 * pz1);
      Double_t e2   = TMath::Sqrt(et2 * et2 + pz2 * pz2);
      Double_t Px   = px1 + px2;
      Double_t Py   = py1 + py2;
      Double_t Pz   = pz1 + pz2;
      Double_t tE   = e1 + e2;
      Double_t tPt  = Px * Px + Py * Py;
      if (tPt > f2Kt2[0] && tPt < f2Kt2[1]) {
        bad_pair = false;
        fPair.SetPdg1(fPid1);
        fPair.SetPdg2(fPid2);
        fPair.SetTrueMomenta1(px1, py1, pz1, e1);
        fPair.SetMomenta1(px1, py1, pz1, e1);
        fPair.SetTrueMomenta2(px2, py2, pz2, e2);
        fPair.SetMomenta2(px2, py2, pz2, e2);
      }
    } while (bad_pair);
  }

}  // namespace Hal
