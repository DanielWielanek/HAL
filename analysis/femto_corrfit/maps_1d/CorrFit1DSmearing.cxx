/*
 * CorrFit1DMomemtumSmearingCorrection.cxx
 *
 *  Created on: 23 sty 2026
 *      Author: daniel
 */

#include "CorrFit1DSmearing.h"

#include <RtypesCore.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>

#include "CorrFitMapKstarRstar.h"
#include "Cout.h"
#include "DividedHisto.h"
#include "Femto1DCF.h"
#include "SmearAlgo.h"
#include "StdHist.h"
#include "StdTypes.h"

#include <iostream>

namespace Hal {
  namespace HMath = Hal::Std::Math;

  void CorrFit1DSmearing::SetCorrelationFunction(const Hal::Femto1DCF& cf) { fCF = (Hal::Femto1DCF*) cf.Clone(); }

  void CorrFit1DSmearing::SetCorrFitMap(const Hal::CorrFitMapKstarRstar& map) {
    fInputMap = (Hal::CorrFitMapKstarRstar*) map.Clone();
  }

  void CorrFit1DSmearing::Calculate() {
    if (!InitSmearing()) {
      Hal::Cout::PrintInfo(Form("Cannot calculate %s", ClassName()), EInfo::kWarning);
      return;
    }
    DoMapSmearing();
    FinishSmearing();
  }

  void CorrFit1DSmearing::SetSmearingMap(TH2D& smearing, ESmearInput opt) {
    if (opt == ESmearInput::kXsimYreco) {
      fSmearingMap = (TH2D*) smearing.Clone();
    } else if (opt == ESmearInput::kXrecoYsim) {
      fSmearingMap = DoInversion(&smearing);
    }
  }

  TH2D* CorrFit1DSmearing::DoInversion(TH2D* h) const {
    auto hx = (TH2D*) h->Clone();
    for (int i = 0; i <= h->GetNbinsX(); i++) {
      for (int j = 0; j <= h->GetNbinsY(); j++) {
        hx->SetBinContent(i, j, h->GetBinContent(j, i));
      }
    }
    return hx;
  }

  CorrFit1DSmearing::~CorrFit1DSmearing() {
    if (fCF) delete fCF;
    if (fSmearingMap) delete fSmearingMap;
    if (fRatio) delete fRatio;
    if (fAcceptanceMap) delete fAcceptanceMap;
  }

  void CorrFit1DSmearing::DoMapSmearing() {
    auto CF_theo2d      = fInputMap->GetHisto();
    auto CF_smeared2d   = fSmearedMap->GetHisto();
    const int kStarBins = CF_theo2d->GetNbinsX();
    const int rStarBins = CF_theo2d->GetNbinsY();
    TMatrixD theoVector(kStarBins + 2, 1);
    TMatrixD theoVectorNum(kStarBins + 2, 1);
    auto smearedD = fAlgo->GetSmeared(fSimDenominator);
    for (int iRadius = 1; iRadius <= rStarBins; iRadius++) {
      for (int jKstar = 1; jKstar <= kStarBins; jKstar++) {
        theoVector[jKstar][0]    = CF_theo2d->GetBinContent(jKstar, iRadius);
        theoVectorNum[jKstar][0] = theoVector[jKstar][0] * fSimDenominator[jKstar][0];
      }
      auto smearedN = fAlgo->GetSmeared(theoVectorNum);
      /*     if (iRadius == 5) {
             TMatrixD comp(smearedN.GetNrows(), 4);
             for (int i = 0; i < smearedN.GetNrows(); i++) {
               comp[i][0] = smearedN[i][0];
               comp[i][1] = smearedD[i][0];
               comp[i][2] = comp[i][0] / comp[i][1];
               comp[i][3] = theoVector[i][0];
             }
             comp.Print();
           }*/
      for (int jKstar = 1; jKstar <= kStarBins; jKstar++) {
        double DenVal  = smearedD[jKstar][0];
        double NumVal  = smearedN[jKstar][0];
        double TheoVal = theoVector[jKstar][0];
        if (DenVal == 0) {
          CF_smeared2d->SetBinContent(jKstar, iRadius, TheoVal);
          fRatio->SetBinContent(jKstar, iRadius, 1);
        } else {
          double smearedVal = NumVal / DenVal;
          double ratio      = TheoVal / smearedVal;
          CF_smeared2d->SetBinContent(jKstar, iRadius, smearedVal);
          fRatio->SetBinContent(jKstar, iRadius, ratio);
        }
      }
    }
  }

  void CorrFit1DSmearing::FinishSmearing() {}

  Bool_t CorrFit1DSmearing::InitSmearing() {
    auto CF_unsmeared    = fInputMap->GetHisto();
    fSmearedMap          = (Hal::CorrFitMapKstarRstar*) fInputMap->Clone();
    fRatio               = (TH2D*) CF_unsmeared->Clone();
    TH1D* rawDenominator = (TH1D*) fCF->GetDen();
    Int_t bin[3];
    Double_t max[3], min[3];
    Hal::Std::GetAxisPar(*rawDenominator, bin[0], min[0], max[0], "x");
    Hal::Std::GetAxisPar(*fRatio, bin[1], min[1], max[1], "x");
    Hal::Std::GetAxisPar(*fSmearingMap, bin[2], min[2], max[2], "x");

    if (bin[0] != bin[1] || bin[0] != bin[2]) {
      Hal::Cout::PrintInfo(Form("Not equal number of bins in maps/smearing functions [%i %i %i]", bin[0], bin[1], bin[2]),
                           EInfo::kError);
      return false;
    }
    if (min[0] != min[1] || min[0] != min[2]) {
      Hal::Cout::PrintInfo(
        Form("Not equal number of mins in maps/smearing functions [%4.2f %4.2f %4.2f]", min[0], min[1], min[2]), EInfo::kError);
      return false;
    }
    if (max[0] != max[1] || max[0] != max[2]) {
      Hal::Cout::PrintInfo(
        Form("Not equal number of maxes in maps/smearing functions [%4.2f %4.2f %4.2f]", max[0], max[1], max[2]), EInfo::kError);
      return false;
    }
    auto RecoDenominator  = HMath::GetMatrix(*rawDenominator, HMath::EOrient::kVertical, true);
    RecoDenominator[0][0] = 0;
    fAcceptance.ResizeTo(RecoDenominator);
    fAcceptance                                 = RecoDenominator;
    fAcceptance[0][0]                           = 1;
    fAcceptance[rawDenominator->GetNbinsX()][0] = 1;
    if (fAcceptanceMap) {
      Hal::Std::GetAxisPar(*rawDenominator, bin[0], min[0], max[0], "x");
      Hal::Std::GetAxisPar(*fAcceptanceMap, bin[1], min[1], max[1], "x");
      if (bin[0] != bin[1] || bin[0] != bin[2] || max[0] != max[1]) {
        Hal::Cout::PrintInfo("Not equal number of bins in cf/acceptance map", EInfo::kError);
      }
      for (int i = 1; i <= rawDenominator->GetNbinsX(); i++) {
        double qinv       = rawDenominator->GetXaxis()->GetBinCenter(i);
        double accept     = fAcceptanceMap->Interpolate(qinv);
        fAcceptance[i][0] = accept;
      }
      fAcceptance[rawDenominator->GetNbinsX() + 1][0] = fAcceptance[rawDenominator->GetNbinsX()][0];
    }
    auto invAcc = fAcceptance;
    for (int i = 0; i < fAcceptance.GetNrows(); i++)
      invAcc[i][0] = 1.0 / fAcceptance[i][0];

    fAlgo = new Hal::SmearAlgoMatrix(true);
    fAlgo->SetInvertionParameters({fLambda});
    fAlgo->SetSmearMatrix(*fSmearingMap);
    fAlgo->SetInvertionMethod(fInversion);
    auto res = fAlgo->Init();
    auto M   = fAlgo->GetSmearingMatrix();
    fSimDenominator.ResizeTo(RecoDenominator);
    switch (fInversion) {
      case SmearAlgoMatrix::EMethod::kNone: {
        fSimDenominator = RecoDenominator;
        if (fAcceptanceMap) {
          for (int j = 0; j < fSimDenominator.GetNrows(); j++) {
            double sumA = 0;
            double sumB = 0;
            for (int i = 0; i < M.GetNcols(); i++) {
              sumA += M[j][i] * RecoDenominator[i][0] / fAcceptance[i][0];
              sumB += M[j][i];
            }
            fSimDenominator[j][0] = sumA;
          }
          const int n = RecoDenominator.GetNrows();
          TMatrixD corrected(RecoDenominator.GetNrows(), 1);

          for (int j = 0; j < RecoDenominator.GetNrows(); ++j)
            corrected[j][0] = RecoDenominator[j][0] / fAcceptance[j][0];

          TMatrixD sim(corrected);

          for (int iter = 0; iter < 10; ++iter) {
            TMatrixD smeared = TMatrixD(TMatrixD::kTransposed, M) * sim;

            for (int i = 0; i < sim.GetNrows(); ++i)
              sim[i][0] = corrected[i][0] + sim[i][0] - smeared[i][0];
          }

          fSimDenominator = sim;
        }
      } break;
      case SmearAlgoMatrix::EMethod::kMatrix: {
        if (fAcceptanceMap) RecoDenominator = VecMultiply(RecoDenominator, fAcceptance);
        fSimDenominator = fAlgo->GetUnsmeared(RecoDenominator);
      } break;
      case SmearAlgoMatrix::EMethod::kThikonov1: {
        if (fAcceptanceMap) RecoDenominator = VecMultiply(RecoDenominator, fAcceptance);
        fSimDenominator = fAlgo->GetUnsmeared(RecoDenominator);
      } break;
      case SmearAlgoMatrix::EMethod::kThikonov2: {

        if (fAcceptanceMap) RecoDenominator = VecMultiply(RecoDenominator, fAcceptance);
        fSimDenominator = fAlgo->GetUnsmeared(RecoDenominator);
      } break;
    }
    return res;
  }

  void CorrFit1DSmearing::SetAcceptanceMap(const TH1D& map) {
    if (fAcceptanceMap) delete fAcceptanceMap;
    fAcceptanceMap = (TH1D*) map.Clone();
  }

  TMatrixD CorrFit1DSmearing::VecMultiply(const TMatrixD& a, const TMatrixD& b) const {
    TMatrixD res(a.GetNrows(), 1);
    for (int i = 0; i < a.GetNrows(); i++) {
      res[i][0] = a[i][0] * b[i][0];
    }
    return res;
  }

} /* namespace Hal */
