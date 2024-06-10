/*
 * CorrFitPairGeneratorYPt2.cxx
 *
 *  Created on: 28 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitPairGeneratorYPt2.h"

#include "FemtoCorrFuncKt.h"
#include "FemtoMiniPair.h"
#include "StdHist.h"

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
  CorrFitPairGeneratorYPt2::CorrFitPairGeneratorYPt2() : f2Kt {0, 1E+9} {}

  void CorrFitPairGeneratorYPt2::SetHist(const TH2D& hist1, const TH2D& hist2) {
    hist1.Copy(fHist1);
    hist2.Copy(fHist2);
    fHist1.SetName("FirstParticle");
    fHist2.SetName("SecondParticle");
    fHist1.SetDirectory(nullptr);
    fHist2.SetDirectory(nullptr);
  }

  Bool_t CorrFitPairGeneratorYPt2::Init() {
    bool val = CorrFitPairGenerator::Init();
    if (!val) return val;
    fHist1.ResetStats();
    fHist2.ResetStats();
    if (!Hal::Std::AreSimilar(&fHist1, &fHist2)) {
      std::cout << __FILE__ << " " << __LINE__ << " histograms are not similar cannot safely make convolution" << std::endl;
      return kFALSE;
    }
    Double_t min1, min2, max1, max2;
    Int_t bin1, bin2;
    Hal::Std::GetAxisPar(fHist1, bin1, min1, max1, "x");
    Hal::Std::GetAxisPar(fHist1, bin2, min2, max2, "y");


    auto kt = dynamic_cast<Hal::FemtoCorrFuncKt*>(fCF);
    if (kt) {
      auto ktArr = kt->GetRange();
      f2Kt[0]    = ktArr[0] * 2.0;
      f2Kt[1]    = ktArr[ktArr.GetSize() - 1] * 2.0;
    }

    fConvolution  = TH2D("conv", "conv;y_{sum},pt_{sum}", bin1 * 2, min1 * 2.0, max1 * 2.0, bin2 * 2, min2 * 2.0, max2 * 2.0);
    auto xCenters = Hal::Std::GetAxisCenters(fConvolution, "x+o");
    auto yCenters = Hal::Std::GetAxisCenters(fConvolution, "y+o");

    std::vector<double> cosines;
    double dphi = 2.0 * TMath::Pi() / 90.0;

    TH2D* rounded = (TH2D*) fHist2.Clone();
    rounded->Reset();
    for (double phi = 0; phi < TMath::TwoPi(); phi += dphi) {
      cosines.push_back(TMath::Cos(phi));
    }

    for (int i = 1; i <= fHist2.GetNbinsX(); i++) {
      double y1 = xCenters[i];
      for (int j = 1; j <= fHist2.GetNbinsY(); j++) {
        Double_t pt1 = yCenters[j];
        for (int k = 1; k < yCenters.size() - 1; k++) {
          double pti = yCenters[k];
          if (pti > pt1) continue;
          double rhoi = fHist2.GetBinContent(i, j);
          double rho  = TMath::Sqrt(pt1 * pt1 - pti * pti) / pt1;
          rounded->Fill(y1, pti * rho * rhoi);
        }
      }
    }


    for (int i = 1; i <= fHist1.GetNbinsX(); i++) {
      double y1 = xCenters[i];
      for (int j = 1; j <= fHist1.GetNbinsY(); j++) {
        double pt1  = yCenters[j];
        double rho1 = fHist1.GetBinContent(i, j);
        if (rho1 == 0) continue;
        for (int k = 1; k <= fHist2.GetNbinsX(); k++) {
          double y2 = xCenters[k];
          for (int l = 1; l <= fHist2.GetNbinsY(); l++) {
            double pt2   = yCenters[k];
            double rho2  = rounded->GetBinContent(k, l);
            double psum1 = pt1 + pt2;
            double psum2 = pt1 - pt2;
            double ptsum = pt1 + pt2;
            fConvolution.Fill(y1 + y2, pt1 + pt2, rho1 * rho2);
          }
        }
      }
    }
    Double_t maximum = fConvolution.GetBinContent(fConvolution.GetMaximumBin());
    fConvolution.Scale(1.0 / maximum);


    if (fHist1.GetEntries() <= 0) return kFALSE;
    if (fHist2.GetEntries() <= 0) { fHist2 = fHist1; }
    return kTRUE;
  }

  Int_t CorrFitPairGeneratorYPt2::GeneratePair(Double_t kstar) {

    Double_t ysum = 0, ptsum = 0;
    while (ptsum < f2Kt[0] || ptsum > f2Kt[1])
      fConvolution.GetRandom2(ysum, ptsum);
    int binx     = fConvolution.GetXaxis()->FindBin(ysum);
    int biny     = fConvolution.GetXaxis()->FindBin(ptsum);
    Double_t rho = fConvolution.GetBinContent(binx, biny);
    fPair.SetPdg1(fPid1);
    fPair.SetPdg2(fPid2);
    TVector3 sum(ptsum,0,ysum);
    sum.RotateZ(gRandom->Uniform(-TMath::Pi(), TMath::Pi()));
    Double_t x,y,z;
    if(fGroupByKstar)
    gRandom->Sphere(x, y, z, kstar);
    TVector3 diff(x,y,z);
    Hal::Femto::FillRandomKinematics(*fHbtPair, sum, diff, fFrame);


    *fHbtPair = fPair;
    fHbtPair->Compute();
    if (fGroupByKstar) { return GetBin(fHbtPair->GetT()); }
    return GetBin(fHbtPair->GetZ());
  }

  CorrFitPairGeneratorYPt2::~CorrFitPairGeneratorYPt2() {}

  void CorrFitPairGeneratorYPt2::GenerateEvent() {
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

} /* namespace Hal */
