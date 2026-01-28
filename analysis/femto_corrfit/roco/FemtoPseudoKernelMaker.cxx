/*
 * FemtoKernelMaker.cxx
 *
 *  Created on: 19 lip 2024
 *      Author: daniel
 */

#include "FemtoPseudoKernelMaker.h"

#include "FastHist.h"
#include "FemtoPair.h"
#include "FemtoWeightGenerator.h"

#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TParticlePDG.h>
#include <TRandom.h>
#include <TVector2.h>

namespace Hal {
  FemtoPseudoKernel::FemtoPseudoKernel(Int_t kstarbins,
                                       Double_t kstarLow,
                                       Double_t kStarHigh,
                                       Int_t rstarbins,
                                       Double_t rLow,
                                       Double_t rHigh,
                                       Int_t thetabins) {
    fHist3d =
      new FastHist3D("map", "map", kstarbins, kstarLow, kStarHigh, rstarbins, rLow, rHigh, thetabins, -TMath::Pi(), TMath::Pi());
  }

  Double_t FemtoPseudoKernel::GetWeight(Double_t kstar, Double_t rstar, Double_t theta) const {
    theta   = TVector2::Phi_mpi_pi(theta);
    int bin = fHist3d->FindBin(kstar, rstar, theta);
    return fHist3d->GetRawBinContent(bin);
  }

  void FemtoPseudoKernel::SetWeightInfo(TString className, Femto::EPairType type) {
    fPairType        = type;
    fWeightClassName = className;
  }

  void FemtoPseudoKernel::SetWeight(Double_t kstar, Double_t rstar, Double_t theta, Double_t w) {
    theta   = TVector2::Phi_mpi_pi(theta);
    int bin = fHist3d->FindBin(kstar, rstar, theta);
    fHist3d->SetRawBinContent(bin, w);
  }

  FemtoPseudoKernelMaker::FemtoPseudoKernelMaker(Int_t kstarbins,
                                                 Double_t kstarLow,
                                                 Double_t kStarHigh,
                                                 Int_t rstarbins,
                                                 Double_t rLow,
                                                 Double_t rHigh,
                                                 Int_t thetabins) {
    fKernel = new FemtoPseudoKernel(kstarbins, kstarLow, kStarHigh, rstarbins, rLow, rHigh, thetabins);
  }

  void FemtoPseudoKernelMaker::SetWeightGenerator(FemtoWeightGenerator& wCalc) {
    fWeight = wCalc.MakeCopy();
    fKernel->SetWeightInfo(fWeight->ClassName(), fWeight->GetPairType());
  }

  void FemtoPseudoKernelMaker::Run(Int_t nSamples) {
    fPair = Hal::Femto::MakePair(Hal::Femto::EKinematics::kPRF, false);
    fPair->SetPdg1(fPid1);
    fPair->SetPdg2(fPid2);
    auto pid  = TDatabasePDG::Instance();
    auto par1 = pid->GetParticle(fPid1);
    auto par2 = pid->GetParticle(fPid2);
    if (!par1 || !par2) return;
    fM1_2 = par1->Mass() * par1->Mass();
    fM2_2 = par2->Mass() * par2->Mass();

    if (nSamples <= 1) {
      const Int_t binsX = fKernel->GetHist()->GetNbinsX();
      const Int_t binsY = fKernel->GetHist()->GetNbinsY();
      const Int_t binsZ = fKernel->GetHist()->GetNbinsZ();
      for (int i = 1; i <= binsX; i++) {
        Double_t kstar = fKernel->GetHist()->GetBinCenterX(i);
        for (int j = 1; j <= binsY; j++) {
          Double_t rstar = fKernel->GetHist()->GetBinCenterY(j);
          for (int k = 1; k <= binsX; k++) {
            Double_t theta = fKernel->GetHist()->GetBinCenterZ(k);
            GeneratePair(kstar, rstar, theta);
            Double_t w = fWeight->GenerateWeight(fPair);
            fKernel->SetWeight(kstar, rstar, theta, w);
          }
        }
      }
    } else {
      const Int_t binsX     = fKernel->GetHist()->GetNbinsX();
      const Int_t binsY     = fKernel->GetHist()->GetNbinsY();
      const Int_t binsZ     = fKernel->GetHist()->GetNbinsZ();
      const Double_t widthX = fKernel->GetHist()->GetBinWidthX() * 0.5;
      const Double_t widthY = fKernel->GetHist()->GetBinWidthY() * 0.5;
      const Double_t widthZ = fKernel->GetHist()->GetBinWidthZ() * 0.5;
      Hal::Array_1<Double_t> kstarArr, rstarArr, thetaArr;
      kstarArr.MakeBigger(nSamples);
      rstarArr.MakeBigger(nSamples);
      thetaArr.MakeBigger(nSamples);
      for (int i = 1; i <= binsX; i++) {
        Double_t kstar = fKernel->GetHist()->GetBinCenterX(i) - widthX * 0.5;
        for (int a = 0; a < nSamples; a++)
          kstarArr[a] = kstar + widthX * gRandom->Rndm();

        for (int j = 1; j <= binsY; j++) {
          Double_t rstar = fKernel->GetHist()->GetBinCenterY(j) - widthY * 0.5;
          for (int a = 0; a < nSamples; a++)
            rstarArr[a] = rstar + widthY * gRandom->Rndm();
          for (int k = 1; k <= binsX; k++) {
            Double_t theta = fKernel->GetHist()->GetBinCenterZ(k) - widthZ * 0.5;
            Double_t sum   = 0;
            for (int a = 0; a < nSamples; a++) {
              thetaArr[a] = theta + widthZ * gRandom->Rndm();
              GeneratePair(kstarArr[a], rstarArr[a], thetaArr[a]);
              sum += fWeight->GenerateWeight(fPair);
            }
            Double_t w = sum / nSamples;
            fKernel->SetWeight(kstar, rstar, theta, w);
          }
        }
      }
    }
  }

  void FemtoPseudoKernelMaker::Save(TString file) {}

  void FemtoPseudoKernelMaker::GeneratePair(Double_t k, Double_t r, Double_t theta) {
    TLorentzVector p1, p2;
    TLorentzVector r1, r2;
    p1.SetPxPyPzE(k, 0, 0, TMath::Sqrt(k * k + fM1_2));
    p2.SetPxPyPzE(-k, 0, 0, TMath::Sqrt(k * k + fM2_2));
    r1.SetXYZT(r, 0, 0, 0);
    r2.SetXYZT(0, 0, 0, 0);
    r1.RotateZ(theta);
    r2.RotateZ(theta);
    Double_t miniboost = 0.01;
    p1.Boost(miniboost, 0, 0);
    p2.Boost(miniboost, 0, 0);
    r1.Boost(miniboost, 0, 0);
    r2.Boost(miniboost, 0, 0);
    fPair->SetMomenta(p1, p2);
    fPair->SetFreezeouts(r1, r2);
  }

  FemtoPseudoKernelMaker::~FemtoPseudoKernelMaker() {
    if (fPair) delete fPair;
    if (fWeight) delete fWeight;
    if (fKernel) delete fKernel;
  }

}  // namespace Hal
