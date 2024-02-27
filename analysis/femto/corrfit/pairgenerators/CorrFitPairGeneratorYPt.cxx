/*
 * CorrFitPairGeneratorYPt.cpp
 *
 *  Created on: 10 kwi 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitPairGeneratorYPt.h"

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
  CorrFitPairGeneratorYPt::CorrFitPairGeneratorYPt() : fKt {0, 1E+9} {}


  void CorrFitPairGeneratorYPt::SetHist(const TH2D& hist1, const TH2D& hist2) {
    hist1.Copy(fHist1);
    hist2.Copy(fHist2);
    fHist1.SetName("FirstParticle");
    fHist2.SetName("SecondParticle");
    fHist1.SetDirectory(nullptr);
    fHist2.SetDirectory(nullptr);
  }

  Bool_t CorrFitPairGeneratorYPt::Init() {
    bool val = CorrFitPairGenerator::Init();
    if (!val) return val;
    fHist1.ResetStats();
    fHist2.ResetStats();
    auto kt = dynamic_cast<Hal::FemtoCorrFuncKt*>(fCF);
    if (kt) {
      auto ktArr = kt->GetRange();
      fKt[0]     = ktArr[0];
      fKt[1]     = ktArr[ktArr.GetSize() - 1];
      fKt[0]     = fKt[0] * fKt[0] * 4;
      fKt[1]     = fKt[1] * fKt[1] * 4;
    }
    if (fHist1.GetEntries() <= 0) return kFALSE;
    if (fHist2.GetEntries() <= 0) { fHist2 = fHist1; }
    return kTRUE;
  }

  Int_t CorrFitPairGeneratorYPt::GeneratePair() {

    Double_t pt1, pt2, y1, y2;
    fHist1.GetRandom2(y1, pt1);
    fHist2.GetRandom2(y2, pt2);
    Float_t phi1 = gRandom->Uniform(-TMath::Pi(), TMath::Pi());
    Float_t phi2 = gRandom->Uniform(-TMath::Pi(), TMath::Pi());
    Float_t px1  = pt1 * TMath::Cos(phi1);
    Float_t px2  = pt2 * TMath::Cos(phi2);
    Float_t py1  = pt1 * TMath::Sin(phi1);
    Float_t py2  = pt2 * TMath::Sin(phi2);
    Float_t et1  = TMath::Sqrt(pt1 * pt2 + fM1 * fM1);
    Float_t et2  = TMath::Sqrt(pt2 * pt2 + fM2 * fM2);
    Float_t pz1  = TMath::Sign(et1 * 0.5, y1) * TMath::Exp(-y1) * (TMath::Exp(2.0 * y1) - 1.0);
    Float_t pz2  = TMath::Sign(et2 * 0.5, y2) * TMath::Exp(-y2) * (TMath::Exp(2.0 * y2) - 1.0);
    Float_t e1   = TMath::Sqrt(et1 * et1 + pz1 * pz1);
    Float_t e2   = TMath::Sqrt(et2 * et2 + pz2 * pz2);
    Float_t Px   = px1 + px2;
    Float_t Py   = py1 + py2;
    Float_t Pz   = pz1 + pz2;
    Float_t tE   = e1 + e2;
    Float_t tPt  = Px * Px + Py * Py;
    if (tPt < fKt[0]) return kFALSE;
    if (tPt > fKt[1]) return kFALSE;
    Float_t tMt    = tE * tE - Pz * Pz;  // mCVK;
    Float_t tM     = TMath::Sqrt(tMt - tPt);
    tMt            = TMath::Sqrt(tMt);
    tPt            = TMath::Sqrt(tPt);
    Float_t tBeta  = Pz / tE;
    Float_t tGamma = tE / tMt;
    Float_t tZ     = tGamma * (pz1 - tBeta * e1);
    Float_t tE1L   = tGamma * (e1 - tBeta * pz1);

    // Rotate in transverse plane
    Float_t tX = (px1 * Px + py1 * Py) / tPt;
    Float_t tY = (-px1 * Py + py1 * Px) / tPt;

    tX = tMt / tM * (tX - tPt / tMt * tE1L);

    Float_t tT = tX > 0. ? 1. : -1.;
    tT *= TMath::Sqrt(tY * tY + tX * tX + tZ * tZ);


    fPair.SetPdg1(fPid1);
    fPair.SetPdg2(fPid2);
    fPair.SetTrueMomenta1(px1, py1, pz1, e1);
    fPair.SetMomenta1(px1, py1, pz1, e1);
    fPair.SetTrueMomenta2(px2, py2, pz2, e2);
    fPair.SetMomenta2(px2, py2, pz2, e2);
    Double_t scale = 1.0;
    if (fFrame == Femto::EKinematics::kLCMS) scale = 2.0;  // from k*  to LCMS
    if (fGroupByKstar) { return GetBin(tT * scale); }
    return GetBin(tZ * scale);
  }

  CorrFitPairGeneratorYPt::~CorrFitPairGeneratorYPt() {}

  void CorrFitPairGeneratorYPt::GenerateEvent() {
    for (int i = 0; i < 100; i++) {
      int bin = GeneratePair();
      if (bin < 0) continue;
      if (fLimitsN[bin] > fBinLimit) continue;
      auto array = fSignalPairs[bin];
      auto pair  = (FemtoMicroPair*) array->ConstructedAt(array->GetEntriesFast());
      fLimitsN.IncrementAfter(bin);
      *pair = fPair;
    }
  }

}  // namespace Hal
