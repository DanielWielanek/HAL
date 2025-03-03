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
    fBinCFZ    = fOneDimBin % fModuloZ;
    Int_t binY = ((fOneDimBin - fBinCFZ) / fModuloZ) % fModuloY;
    Int_t binX = ((fOneDimBin - fBinCFZ - binY * fModuloY) / fModuloZ) % fModuloY;
    fOut       = fCentersX[binX];
    fSide      = fCentersY[binY];
    fLong      = fCentersZ[fBinCFZ];
    // 1, 0.1, 0.11, 1, 0.07, 0.08, 1, 0.15, 0.16,
    fOut  = 0.305;
    fSide = 0.275;
    fLong = 0.155;
    //  fOut = fSide = fLong = 0.0;  // TODO
    fOneDimBin++;
    std::cout << "GEN" << fOut << " " << fSide << " " << fLong << std::endl;
    switch (fFrame) {
      case Hal::Femto::EKinematics::kPRF: {
        GenerateEventPRF();
      } break;
      case Hal::Femto::EKinematics::kLCMS: {
        GenerateEventLCMS();
      } break;
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

  void CorrFitPairGeneratorConvolutionYPt::GenerateEventPRF() {}

  void CorrFitPairGeneratorConvolutionYPt::GenerateEventLCMS() {
    fConvolution.Reset();
    Double_t sumM   = fM1 + fM2;
    Double_t q_out  = 0;  // fOut;
    Double_t q_side = 0;  // fSide;
    Double_t q_long = 0;  // fLong;
    for (int iY = 1; iY <= fConvolution.GetNbinsX(); iY++) {
      Double_t y_sum = fConvolution.GetXaxis()->GetBinCenter(iY);
      Double_t sinH  = TMath::SinH(y_sum);
      for (int iPt = 1; iPt <= fConvolution.GetNbinsY(); iPt++) {
        Double_t pt_sum = fConvolution.GetYaxis()->GetBinCenter(iPt);
        Double_t pz_sum = TMath::Sqrt(pt_sum * pt_sum + sumM * sumM) * sinH;
        Int_t se        = TMath::Floor(gRandom->Uniform(0, 8));
        switch (se) {
          case 0: {
            q_out  = 0.5 * fOut;
            q_side = 0.5 * fSide;
            q_long = 0.5 * fLong;
          } break;

          case 1: {
            q_out  = 0.5 * fOut;
            q_side = 0.5 * fSide;
            q_long = -0.5 * fLong;
          } break;
          case 2: {
            q_out  = 0.5 * fOut;
            q_side = -0.5 * fSide;
            q_long = 0.5 * fLong;
          } break;
          case 3: {
            q_out  = 0.5 * fOut;
            q_side = -0.5 * fSide;
            q_long = -0.5 * fLong;
          } break;
          case 4: {
            q_out  = -0.5 * fOut;
            q_side = 0.5 * fSide;
            q_long = 0.5 * fLong;
          } break;
          case 5: {
            q_out  = -0.5 * fOut;
            q_side = 0.5 * fSide;
            q_long = -0.5 * fLong;
          } break;
          case 6: {
            q_out  = -0.5 * fOut;
            q_side = -0.5 * fSide;
            q_long = 0.5 * fLong;
          } break;
          case 7: {
            q_out  = -0.5 * fOut;
            q_side = -0.5 * fSide;
            q_long = -0.5 * fLong;
          } break;
        }
        q_out += gRandom->Uniform(-0.01, 0.01);
        q_side += gRandom->Uniform(-0.01, 0.01);
        q_long += gRandom->Uniform(-0.01, 0.01);
        TLorentzVector p1, p2;
        p1.SetXYZM(q_out + pt_sum * 0.5, q_side, q_long, fM1);
        p2.SetXYZM(-q_out + pt_sum * 0.5, -q_side, -q_long, fM2);
        Double_t boostZ = pz_sum / TMath::Sqrt(pz_sum * pz_sum + pt_sum * pt_sum + sumM * sumM);
        p1.Boost(0, 0, boostZ);
        p2.Boost(0, 0, boostZ);  // do it later in PRF
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
          Double_t rho = fHist1.GetBinContent(binX1, binY1) * fHist2.GetBinContent(binX2, binY2) * pt_sum;
          if (pt_sum * 0.5 >= fKt[0] && pt_sum * 0.5 <= fKt[1]) { fConvolution.Fill(y_sum, pt_sum, phi, rho); }
        }
      }
    }
    auto array = fPairFile->GetSignal(fBinCFZ);

    for (int i = 0; i < fPairsPerBin; i++) {
      Double_t y_sum, pt_sum, phi_sum;
      fConvolution.GetRandom3(y_sum, pt_sum, phi_sum);
      if (pt_sum * 0.5 < fKt[0] && pt_sum * 0.5 > fKt[1]) {
        i--;
        std::cout << "OOPS" << pt_sum * 0.5 << std::endl;
        continue;
      }
      TLorentzVector p1, p2;
      Int_t se = TMath::Floor(gRandom->Uniform(0, 8));
      switch (se) {
        case 0: {
          q_out  = 0.5 * fOut;
          q_side = 0.5 * fSide;
          q_long = 0.5 * fLong;
        } break;

        case 1: {
          q_out  = 0.5 * fOut;
          q_side = 0.5 * fSide;
          q_long = -0.5 * fLong;
        } break;
        case 2: {
          q_out  = 0.5 * fOut;
          q_side = -0.5 * fSide;
          q_long = 0.5 * fLong;
        } break;
        case 3: {
          q_out  = 0.5 * fOut;
          q_side = -0.5 * fSide;
          q_long = -0.5 * fLong;
        } break;
        case 4: {
          q_out  = -0.5 * fOut;
          q_side = 0.5 * fSide;
          q_long = 0.5 * fLong;
        } break;
        case 5: {
          q_out  = -0.5 * fOut;
          q_side = 0.5 * fSide;
          q_long = -0.5 * fLong;
        } break;
        case 6: {
          q_out  = -0.5 * fOut;
          q_side = -0.5 * fSide;
          q_long = 0.5 * fLong;
        } break;
        case 7: {
          q_out  = -0.5 * fOut;
          q_side = -0.5 * fSide;
          q_long = -0.5 * fLong;
        } break;
      }
      // q_out += gRandom->Uniform(-0.00, 0.01);
      // q_side += gRandom->Uniform(-0.01, 0.01);
      // q_long += gRandom->Uniform(-0.01, 0.01);
      p1.SetXYZM(q_out + pt_sum * 0.5, q_side, q_long, fM1);
      p2.SetXYZM(-q_out + pt_sum * 0.5, -q_side, -q_long, fM2);
      p1.RotateZ(phi_sum);
      p2.RotateZ(phi_sum);
      Double_t sinH   = TMath::SinH(y_sum);
      Double_t pz_sum = TMath::Sqrt(pt_sum * pt_sum + sumM * sumM) * sinH;
      Double_t boostZ = pz_sum / TMath::Sqrt(pz_sum * pz_sum + pt_sum * pt_sum + sumM * sumM);
      p1.Boost(0, 0, boostZ);
      p2.Boost(0, 0, boostZ);

      auto pair = (FemtoMicroPair*) array->ConstructedAt(array->GetEntriesFast());
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

} /* namespace Hal */
