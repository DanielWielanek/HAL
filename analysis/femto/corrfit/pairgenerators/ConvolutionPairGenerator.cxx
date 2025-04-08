/*
 * ConvolutionPairGenerator.cxx
 *
 *  Created on: 1 kwi 2025
 *      Author: daniel
 */

#include "ConvolutionPairGenerator.h"

#include <TAxis.h>
#include <TDatabasePDG.h>
#include <TMath.h>
#include <TParticlePDG.h>

#include "Cout.h"
#include "FemtoMiniPair.h"
#include "StdHist.h"

namespace Hal {

  void ConvolutionPairGenerator::GeneratePairLCMS(const Double_t ptTot, const Double_t pzTot, const Double_t phi) {
    const Double_t sumM = fM1 + fM2;
    fP1.SetXYZM(fX + ptTot * 0.5, fY, fX, fM1);
    fP2.SetXYZM(-fX + ptTot * 0.5, -fY, -fZ, fM2);
    fP1.RotateZ(phi);
    fP2.RotateZ(phi);
    Double_t boostZ = pzTot / TMath::Sqrt(pzTot * pzTot + ptTot * ptTot + sumM * sumM);
    fP1.Boost(0, 0, boostZ);
    fP2.Boost(0, 0, boostZ);
  }

  void ConvolutionPairGenerator::GeneratePairPRF(const Double_t ptTot, const Double_t pzTot, const Double_t phi) {
    const Double_t sumM = fM1 + fM2;
    fP1.SetXYZM(fX, fY, fZ, fM1);
    fP2.SetXYZM(-fX, -fY, -fZ, fM2);
    Double_t boostX = ptTot / TMath::Sqrt(ptTot * ptTot + sumM * sumM);
    fP1.Boost(boostX, 0, 0);
    fP2.Boost(boostX, 0, 0);
    Double_t boostZ = pzTot / TMath::Sqrt(pzTot * pzTot + ptTot * ptTot + sumM * sumM);
    fP1.Boost(0, 0, boostZ);
    fP2.Boost(0, 0, boostZ);
    fP1.RotateZ(phi);
    fP2.RotateZ(phi);
  }

  ConvolutionPairGenerator::ConvolutionPairGenerator(Hal::Femto::EKinematics frame) : fFrame(frame) {}

  void ConvolutionPairGenerator::Recalculate(Double_t kout, Double_t kside, Double_t klong) {
    fX = kout;
    fY = kside;
    fZ = klong;
    if (fFrame == Hal::Femto::EKinematics::kLCMS) {
      fX *= 0.5;
      fY *= 0.5;
      fZ *= 0.5;
    }
    fConvolution.Reset();
    FemtoMicroPair pair;
    Double_t sumM = fM1 + fM2;
    for (int iY = 1; iY <= fConvolution.GetNbinsX(); iY++) {
      Double_t y_sum = fConvolution.GetXaxis()->GetBinCenter(iY);
      Double_t sinH  = TMath::SinH(y_sum);
      for (int iPt = 1; iPt <= fConvolution.GetNbinsY(); iPt++) {
        Double_t ptTot = fConvolution.GetYaxis()->GetBinCenter(iPt);
        Double_t pzTot = TMath::Sqrt(ptTot * ptTot + sumM * sumM) * sinH;
        if (fFrame == Hal::Femto::EKinematics::kLCMS) {
          GeneratePairLCMS(ptTot, pzTot, 0.0);
        } else {
          GeneratePairPRF(ptTot, pzTot, 0.0);
        }
        for (int iPhi = 1; iPhi <= fConvolution.GetNbinsZ(); iPhi++) {
          Double_t phi      = fConvolution.GetZaxis()->GetBinCenter(iPhi);
          TLorentzVector P1 = fP1;
          TLorentzVector P2 = fP2;
          P1.RotateZ(phi);
          P2.RotateZ(phi);
          Int_t binX1  = fHist1.GetXaxis()->FindBin(P1.Rapidity());
          Int_t binX2  = fHist2.GetXaxis()->FindBin(P2.Rapidity());
          Int_t binY1  = fHist1.GetYaxis()->FindBin(P1.Pt());
          Int_t binY2  = fHist2.GetYaxis()->FindBin(P2.Pt());
          Double_t rho = fHist1.GetBinContent(binX1, binY1) * fHist2.GetBinContent(binX2, binY2) * ptTot;
          if (ptTot * 0.5 >= fKt[0] && ptTot * 0.5 <= fKt[1]) { fConvolution.Fill(y_sum, ptTot, phi, rho); }
        }
      }
    }
  }

  void ConvolutionPairGenerator::SetData(const TH2D& hist1, Int_t pdg1, const TH2D& hist2, Int_t pdg2) {
    hist1.Copy(fHist1);
    hist2.Copy(fHist2);
    fHist1.SetName("FirstParticle");
    fHist2.SetName("SecondParticle");
    fHist1.SetDirectory(nullptr);
    fHist2.SetDirectory(nullptr);
    fPid1 = pdg1;
    fPid2 = pdg2;
  }

  void ConvolutionPairGenerator::FillPair(Hal::FemtoMicroPair& pair) {
    Double_t y_sum, pt_sum, phi_sum;
    fConvolution.GetRandom3(y_sum, pt_sum, phi_sum);
    if (pt_sum * 0.5 < fKt[0] && pt_sum * 0.5 > fKt[1]) { return; }
    Double_t sinH   = TMath::SinH(y_sum);
    Double_t pz_sum = TMath::Sqrt(pt_sum * pt_sum + fSumM * fSumM) * sinH;
    if (fFrame == Hal::Femto::EKinematics::kLCMS) {
      GeneratePairLCMS(pt_sum, pz_sum, phi_sum);
    } else {
      GeneratePairPRF(pt_sum, pz_sum, phi_sum);
    }
    pair.SetTrueMomenta1(fP1.X(), fP1.Y(), fP1.Z(), fP1.E());
    pair.SetTrueMomenta2(fP2.X(), fP2.Y(), fP2.Z(), fP2.E());
    pair.SetMomenta1(fP1.X(), fP1.Y(), fP1.Z(), fP1.E());
    pair.SetMomenta2(fP2.X(), fP2.Y(), fP2.Z(), fP2.E());
  }

  void ConvolutionPairGenerator::FillPair(TLorentzVector& p1, TLorentzVector& p2) {
    Double_t y_sum, pt_sum, phi_sum;
    fConvolution.GetRandom3(y_sum, pt_sum, phi_sum);
    if (pt_sum * 0.5 < fKt[0] && pt_sum * 0.5 > fKt[1]) { return; }
    Double_t sinH   = TMath::SinH(y_sum);
    Double_t pz_sum = TMath::Sqrt(pt_sum * pt_sum + fSumM * fSumM) * sinH;
    if (fFrame == Hal::Femto::EKinematics::kLCMS) {
      GeneratePairLCMS(pt_sum, pz_sum, phi_sum);
    } else {
      GeneratePairPRF(pt_sum, pz_sum, phi_sum);
    }
    p1 = fP1;
    p2 = fP2;
  }

  ConvolutionPairGenerator::~ConvolutionPairGenerator() {}

  Bool_t ConvolutionPairGenerator::Init() {
    if (fHist1.GetEntries() <= 0) return kFALSE;
    if (fHist2.GetEntries() <= 0) { fHist2 = fHist1; }
    if (!Hal::Std::AreSimilar(&fHist1, &fHist2, kTRUE)) {
      Hal::Cout::PrintInfo("Incompatible yield histograms, different axes?", Hal::EInfo::kError);
      return kFALSE;
    }
    auto db   = TDatabasePDG::Instance();
    auto pid1 = db->GetParticle(fPid1);
    auto pid2 = db->GetParticle(fPid2);
    if (pid1) fM1 = pid1->Mass();
    if (pid2) fM2 = pid2->Mass();
    fSumM = fM1 + fM2;
    Double_t ptLow, ptHigh;
    Double_t yLow, yHigh;
    Int_t binsPt, binsY;
    Hal::Std::GetAxisPar(fHist1, binsY, yLow, yHigh, "x");
    Hal::Std::GetAxisPar(fHist1, binsPt, ptLow, ptHigh, "y");
    if (ptLow != 0) {
      Hal::Cout::PrintInfo("YPt histo - Pt should start from zero!", Hal::EInfo::kError);
      return kFALSE;
    }
    for (int iPt = 1; iPt <= fHist1.GetYaxis()->GetNbins(); iPt++) {
      Double_t low    = fHist1.GetYaxis()->GetBinLowEdge(iPt);
      Double_t high   = fHist1.GetYaxis()->GetBinUpEdge(iPt);
      Double_t weight = TMath::Sqrt(high * high - low * low);
      weight          = (high + low) * 0.5;
      // weight          = 1;
      for (int iY = 1; iY <= fHist1.GetNbinsX(); iY++) {
        fHist1.SetBinContent(iY, iPt, fHist1.GetBinContent(iY, iPt) / weight);
        fHist2.SetBinContent(iY, iPt, fHist2.GetBinContent(iY, iPt) / weight);
      }
    }
    fConvolution = TH3D("convolution",
                        "convolution",
                        binsY * 2,
                        yLow * 2,
                        yHigh * 2,
                        binsPt * 2,
                        ptLow * 2,
                        ptHigh * 2,
                        1,
                        -TMath::Pi(),
                        TMath::Pi());
    fConvolution.SetDirectory(nullptr);
    return kTRUE;
  }

} /* namespace Hal */
