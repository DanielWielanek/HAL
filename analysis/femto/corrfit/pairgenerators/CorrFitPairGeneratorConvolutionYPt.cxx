/*
 * CorrFitPairGeneratorConvolutionYPt.cxx
 *
 *  Created on: 20 sty 2025
 *      Author: daniel
 */

#include "CorrFitPairGeneratorConvolutionYPt.h"

#include "CorrFitPairFile.h"
#include "Cout.h"
#include "FemtoCorrFuncKt.h"
#include "FemtoMiniPair.h"
#include "StdHist.h"

#include <RtypesCore.h>
#include <TAxis.h>
#include <TClonesArray.h>
#include <TDatabasePDG.h>
#include <TFile.h>
#include <TH3D.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TObjArray.h>
#include <TParticlePDG.h>
#include <TRandom.h>
#include <TTree.h>


namespace Hal {

  void CorrFitPairGeneratorConvolutionYPt::GenerateEvent() {
    fBinCFZ                  = fOneDimBin % fModuloZ;
    Int_t binY               = ((fOneDimBin - fBinCFZ) / fModuloZ) % fModuloY;
    Int_t binX               = ((fOneDimBin - fBinCFZ - binY * fModuloY) / fModuloZ) % fModuloY;
    Double_t q_out           = fCentersX[binX];
    Double_t q_side          = fCentersY[binY];
    Double_t q_long          = fCentersZ[fBinCFZ];
    Double_t q_outLimits[2]  = {q_out - fHalfWidthX, q_out + fHalfWidthX};
    Double_t q_sideLimits[2] = {q_side - fHalfWidthY, q_side + fHalfWidthY};
    Double_t q_longLimits[2] = {q_long - fHalfWidthZ, q_long + fHalfWidthZ};

    // fOut  = 0.11;
    // fSide = 0.07;
    // fLong = 0.15;
    std::cout << "COMPUING " << q_out << " " << q_side << " " << q_long << std::endl;
    fOneDimBin++;
    if (fGenerateSwap && fBinSmearFactor > 0) {  // smear and swap
      for (int i = 0; i < fBinSmearFactor; i++) {
        for (int j = 0; j < 8; j++) {
          q_out            = gRandom->Uniform(q_outLimits[0], q_outLimits[1]);
          q_side           = gRandom->Uniform(q_sideLimits[0], q_sideLimits[1]);
          q_long           = gRandom->Uniform(q_longLimits[0], q_longLimits[1]);
          fOut[i * 8 + j]  = (j & 1) ? -q_out : q_out;
          fSide[i * 8 + j] = (j & 2) ? -q_side : q_side;
          fLong[i * 8 + j] = (j & 4) ? -q_long : q_long;
        }
      }
    } else if (fGenerateSwap) {  // only swap
      for (int i = 0; i < 8; i++) {
        fOut[i]  = (i & 1) ? -q_out : q_out;
        fSide[i] = (i & 2) ? -q_side : q_side;
        fLong[i] = (i & 4) ? -q_long : q_long;
      }
    } else if (fBinSmearFactor > 0) {  // only smear
      for (int i = 0; i < fBinSmearFactor; i++) {
        fOut[i]  = gRandom->Uniform(q_outLimits[0], q_outLimits[1]);
        fSide[i] = gRandom->Uniform(q_sideLimits[0], q_sideLimits[1]);
        fLong[i] = gRandom->Uniform(q_longLimits[0], q_longLimits[1]);
      }
    } else {  // simples case
      fOut[0]  = q_out;
      fSide[0] = q_side;
      fLong[0] = q_long;
    }
    for (int i = 0; i < fOut.size(); i++) {
      std::cout << fOut[i] << " <==" << std::endl;
      fX = fOut[i];
      fY = fSide[i];
      fZ = fLong[i];
      GeneratePairEvent();
    }
  }

  CorrFitPairGeneratorConvolutionYPt::CorrFitPairGeneratorConvolutionYPt() {}

  void CorrFitPairGeneratorConvolutionYPt::SetHist(const TH2D& hist1, const TH2D& hist2) {
    hist1.Copy(fHist1);
    hist2.Copy(fHist2);
    fHist1.SetName("FirstParticle");
    fHist2.SetName("SecondParticle");
    fHist1.SetDirectory(nullptr);
    fHist2.SetDirectory(nullptr);
  }

  void CorrFitPairGeneratorConvolutionYPt::GeneratePairEvent() {
    fConvolution.Reset();
    Double_t sumM = fM1 + fM2;
    if (fFrame == Hal::Femto::EKinematics::kLCMS) {
      fX *= 0.5;
      fY *= 0.5;
      fZ *= 0.5;
    }
    CalculateConvolution();

    auto array = fPairFile->GetSignal(fBinCFZ);
    TLorentzVector p1, p2;
    for (int i = 0; i < fPairsPerBin; i++) {
      Double_t y_sum, pt_sum, phi_sum;
      fConvolution.GetRandom3(y_sum, pt_sum, phi_sum);
      if (pt_sum * 0.5 < fKt[0] && pt_sum * 0.5 > fKt[1]) {
        i--;
        std::cout << "OOPS" << pt_sum * 0.5 << std::endl;
        continue;
      }
      Double_t sinH   = TMath::SinH(y_sum);
      Double_t pz_sum = TMath::Sqrt(pt_sum * pt_sum + sumM * sumM) * sinH;
      auto pair       = (FemtoMicroPair*) array->ConstructedAt(array->GetEntriesFast());
      if (fFrame == Hal::Femto::EKinematics::kLCMS) {
        GeneratePairLCMS(pt_sum, pz_sum, phi_sum, p1, p2);
      } else {
        GeneratePairPRF(pt_sum, pz_sum, phi_sum, p1, p2);
      }
      pair->SetTrueMomenta1(p1.X(), p1.Y(), p1.Z(), p1.E());
      pair->SetTrueMomenta2(p2.X(), p2.Y(), p2.Z(), p2.E());
      pair->SetMomenta1(p1.X(), p1.Y(), p1.Z(), p1.E());
      pair->SetMomenta2(p2.X(), p2.Y(), p2.Z(), p2.E());
    }
  }

  Bool_t CorrFitPairGeneratorConvolutionYPt::Init() {
    bool val = CorrFitPairGenerator::Init();
    if (fGroupingFlag == EGrouping::kOneDim) { return false; }
    if (!val) return val;
    fHist1.ResetStats();
    fHist2.ResetStats();
    auto kt = dynamic_cast<Hal::FemtoCorrFuncKt*>(fCF);
    if (kt) {
      auto ktArr = kt->GetRange();
      fKt[0]     = ktArr[0];
      fKt[1]     = ktArr[ktArr.GetSize() - 1];
    }
    if (fHist1.GetEntries() <= 0) return kFALSE;
    if (fHist2.GetEntries() <= 0) { fHist2 = fHist1; }
    if (!Hal::Std::AreSimilar(&fHist1, &fHist2, kTRUE)) {
      Hal::Cout::PrintInfo("Incompatible yield histograms, different axes?", Hal::EInfo::kError);
      return kFALSE;
    }
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
    fOut.resize(1);
    fSide.resize(1);
    fLong.resize(1);
    if (fGenerateSwap) {
      fOut.resize(8);
      fSide.resize(8);
      fLong.resize(8);
    }
    if (fBinSmearFactor >= 1) {
      fOut.resize(fOut.size() * fBinSmearFactor);
      fSide.resize(fSide.size() * fBinSmearFactor);
      fLong.resize(fLong.size() * fBinSmearFactor);
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
    fModuloX = fXaxis.GetNBins();
    fModuloY = fYaxis.GetNBins();
    fModuloZ = fZaxis.GetNBins();
    return kTRUE;
  }

  void CorrFitPairGeneratorConvolutionYPt::SwapSignRandom(Double_t& x, Double_t& y, Double_t& z) const {
    Int_t se = TMath::Floor(gRandom->Uniform(0, 8));
    switch (se) {
      case 0: {
      } break;
      case 1: {
        x = -x;
      } break;
      case 2: {
        y = -y;
      } break;
      case 3: {
        x = -x;
        y = -y;
      } break;
      case 4: {
        z = -z;
      } break;
      case 5: {
        x = -x;
        z = -z;
      } break;
      case 6: {
        y = -y;
        z = -z;
      } break;
      case 7: {
        x = -x;
        y = -y;
        z = -z;
      } break;
    }
  }

  void CorrFitPairGeneratorConvolutionYPt::CalculateConvolution() {
    fConvolution.Reset();
    FemtoMicroPair pair;
    Double_t sumM = fM1 + fM2;
    TLorentzVector p1, p2;
    for (int iY = 1; iY <= fConvolution.GetNbinsX(); iY++) {
      Double_t y_sum = fConvolution.GetXaxis()->GetBinCenter(iY);
      Double_t sinH  = TMath::SinH(y_sum);
      for (int iPt = 1; iPt <= fConvolution.GetNbinsY(); iPt++) {
        Double_t ptTot = fConvolution.GetYaxis()->GetBinCenter(iPt);
        Double_t pzTot = TMath::Sqrt(ptTot * ptTot + sumM * sumM) * sinH;
        if (fFrame == Hal::Femto::EKinematics::kLCMS) {
          GeneratePairLCMS(ptTot, pzTot, 0.0, p1, p2);
        } else {
          GeneratePairPRF(ptTot, pzTot, 0.0, p1, p2);
        }
        for (int iPhi = 1; iPhi <= fConvolution.GetNbinsZ(); iPhi++) {
          Double_t phi      = fConvolution.GetZaxis()->GetBinCenter(iPhi);
          TLorentzVector P1 = p1;
          TLorentzVector P2 = p2;
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

  void CorrFitPairGeneratorConvolutionYPt::GeneratePairLCMS(const Double_t ptTot,
                                                            const Double_t pzTot,
                                                            const Double_t phi,
                                                            TLorentzVector& p1,
                                                            TLorentzVector& p2) const {
    const Double_t sumM = fM1 + fM2;
    p1.SetXYZM(fX + ptTot * 0.5, fY, fX, fM1);
    p2.SetXYZM(-fX + ptTot * 0.5, -fY, -fZ, fM2);
    p1.RotateZ(phi);
    p2.RotateZ(phi);
    Double_t boostZ = pzTot / TMath::Sqrt(pzTot * pzTot + ptTot * ptTot + sumM * sumM);
    p1.Boost(0, 0, boostZ);
    p2.Boost(0, 0, boostZ);
  }

  void CorrFitPairGeneratorConvolutionYPt::GeneratePairPRF(const Double_t ptTot,
                                                           const Double_t pzTot,
                                                           const Double_t phi,
                                                           TLorentzVector& p1,
                                                           TLorentzVector& p2) const {
    const Double_t sumM = fM1 + fM2;
    p1.SetXYZM(fX, fY, fZ, fM1);
    p2.SetXYZM(-fX, -fY, -fZ, fM2);
    Double_t boostX = ptTot / TMath::Sqrt(ptTot * ptTot + sumM * sumM);
    p1.Boost(boostX, 0, 0);
    p2.Boost(boostX, 0, 0);
    Double_t boostZ = pzTot / TMath::Sqrt(pzTot * pzTot + ptTot * ptTot + sumM * sumM);
    p1.Boost(0, 0, boostZ);
    p2.Boost(0, 0, boostZ);
    p1.RotateZ(phi);
    p2.RotateZ(phi);
  }

} /* namespace Hal */
