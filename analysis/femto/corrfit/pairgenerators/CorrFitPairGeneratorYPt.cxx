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
  CorrFitPairGeneratorYPt::CorrFitPairGeneratorYPt() : f2Kt2 {0, 1E+9} {}

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
      f2Kt2[0]   = ktArr[0];
      f2Kt2[1]   = ktArr[ktArr.GetSize() - 1];
      f2Kt2[0]   = f2Kt2[0] * f2Kt2[0] * 4;
      f2Kt2[1]   = f2Kt2[1] * f2Kt2[1] * 4;
    }
    if (fHist1.GetEntries() <= 0) return kFALSE;
    if (fHist2.GetEntries() <= 0) { fHist2 = fHist1; }
    return kTRUE;
  }

  Int_t CorrFitPairGeneratorYPt::GeneratePair() {

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
    if (tPt < f2Kt2[0]) return -1;
    if (tPt > f2Kt2[1]) return -1;


    fPair.SetPdg1(fPid1);
    fPair.SetPdg2(fPid2);
    fPair.SetTrueMomenta1(px1, py1, pz1, e1);
    fPair.SetMomenta1(px1, py1, pz1, e1);
    fPair.SetTrueMomenta2(px2, py2, pz2, e2);
    fPair.SetMomenta2(px2, py2, pz2, e2);
    *fHbtPair = fPair;
    fHbtPair->Compute();
    if (fGroupByKstar) { return GetBin(fHbtPair->GetT()); }
    return GetBin(fHbtPair->GetZ());
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
