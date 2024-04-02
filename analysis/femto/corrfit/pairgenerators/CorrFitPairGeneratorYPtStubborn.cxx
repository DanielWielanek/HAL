/*
 * CorrFitPairGeneratorYPtStubbornSubborn.cxx
 *
 *  Created on: 29 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitPairGeneratorYPtStubborn.h"

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
  CorrFitPairGeneratorYPtStubborn::CorrFitPairGeneratorYPtStubborn() : f2Kt2 {0, 1E+9} {}

  void CorrFitPairGeneratorYPtStubborn::SetHist(const TH2D& hist1, const TH2D& hist2) {
    hist1.Copy(fHist1);
    hist2.Copy(fHist2);
    fHist1.SetName("FirstParticle");
    fHist2.SetName("SecondParticle");
    fHist1.SetDirectory(nullptr);
    fHist2.SetDirectory(nullptr);
  }

  Bool_t CorrFitPairGeneratorYPtStubborn::Init() {
    bool val = CorrFitPairGenerator::Init();
    if (fFrame != Femto::EKinematics::kLCMS) return kFALSE;
    if (!fGroupByKstar) return kFALSE;
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

  Bool_t CorrFitPairGeneratorYPtStubborn::GeneratePair(Int_t kstar) {
    Double_t pt1, pt2, y1, y2;
    fHist1.GetRandom2(y1, pt1);
    fHist2.GetRandom2(y2, pt2);
    Double_t phi1 = gRandom->Uniform(-TMath::Pi(), TMath::Pi());
    Double_t phi2 = gRandom->Uniform(-TMath::Pi(), TMath::Pi());
    Double_t dCos = TMath::Cos(phi1 - phi2);
    Double_t kT2  = pt1 * pt1 + pt2 * pt2 + 2.0 * pt1 * pt2 * dCos;
    if (kT2 < f2Kt2[0]) return kFALSE;
    if (kT2 > f2Kt2[1]) return kFALSE;

    double pt12 = pt1 * pt1;
    double pt22 = pt2 * pt2;
    // double alpha1 = TMath::Sqrt(pt12 + fM1 * fM1);
    // double alpha2 = TMath::Sqrt(pt22 + fM2 * fM2);
    double m2 = (pt12 + fM1 * fM1) + (pt22 + fM2 * fM2)
                - 2. * TMath::Sqrt((pt12 + fM1 * fM1) * (pt22 + fM2 * fM2)) * TMath::CosH(y1 - y2) - pt12 - pt22
                + 2. * pt1 * pt2 * dCos;  // is faster?
    double qinv2 = TMath::Sqrt(TMath::Abs(m2));
    Double_t px1 = pt1 * TMath::Cos(phi1);
    Double_t px2 = pt2 * TMath::Cos(phi2);
    Double_t py1 = pt1 * TMath::Sin(phi1);
    Double_t py2 = pt2 * TMath::Sin(phi2);
    Double_t Px  = px1 + px2;
    Double_t Py  = py1 + py2;
    Double_t tPt = Px * Px + Py * Py;
    if (TMath::Abs(tPt - kT2) > 1.E-6) { std::cout << "DIFF " << (tPt - kT2) * 1.E+6 << std::endl; }
    Double_t et1 = TMath::Sqrt(pt1 * pt1 + fM1 * fM1);
    Double_t et2 = TMath::Sqrt(pt2 * pt2 + fM2 * fM2);
    Double_t pz1 = TMath::Sign(et1 * 0.5, y1) * TMath::Exp(-y1) * (TMath::Exp(2.0 * y1) - 1.0);
    Double_t pz2 = TMath::Sign(et2 * 0.5, y2) * TMath::Exp(-y2) * (TMath::Exp(2.0 * y2) - 1.0);
    Double_t e1  = TMath::Sqrt(et1 * et1 + pz1 * pz1);
    Double_t e2  = TMath::Sqrt(et2 * et2 + pz2 * pz2);
    Double_t dpx = px1 - px2;
    Double_t dpy = py1 - py2;
    Double_t dpz = pz1 - pz2;
    Double_t dpe = e1 - e2;

    Double_t qinv = TMath::Sqrt(TMath::Abs(dpe * dpe - dpx * dpx - dpy * dpy - dpz * dpz));
    /*
        Double_t qinv2 = 2.0 * TMath::CosH(y1 - y2) * TMath::Sqrt((pt1 * pt1 + fM1 * fM1) * (pt2 * pt2 + fM2 * fM2))
                         - 2.0 * TMath::Cos(phi1 - (phi2 - TMath::Pi())) * pt1 * pt2 + fM1 * fM1 + fM2 * fM2;
        qinv2 = TMath::Sqrt(TMath::Abs(qinv2));*/
    if (GetBin(qinv) != kstar) return kFALSE;


    fPair.SetPdg1(fPid1);
    fPair.SetPdg2(fPid2);
    fPair.SetTrueMomenta1(px1, py1, pz1, e1);
    fPair.SetMomenta1(px1, py1, pz1, e1);
    fPair.SetTrueMomenta2(px2, py2, pz2, e2);
    fPair.SetMomenta2(px2, py2, pz2, e2);
    return kTRUE;
  }

  CorrFitPairGeneratorYPtStubborn::~CorrFitPairGeneratorYPtStubborn() {}

  void CorrFitPairGeneratorYPtStubborn::GenerateEvent() {
    Double_t step = fCentersX[1] - fCentersX[0];
    step *= 0.5;
    for (int i = 0; i < fCentersX.GetSize(); i++) {
      int dummy = 0;
      while (!GeneratePair(i)) {
        dummy++;
      };
      // std::cout << i << " " << dummy << std::endl;
      if (i < 0) continue;
      if (fLimitsN[i] > fBinLimit) continue;
      auto array = fSignalPairs[i];
      auto pair  = (FemtoMicroPair*) array->ConstructedAt(array->GetEntriesFast());
      fLimitsN.IncrementAfter(i);
      *pair = fPair;
    }
  }

}  // namespace Hal
