/*
 * CorrFitSHCF.cxx
 *
 *  Created on: 23 lis 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFitSHCF.h"
#include "CorrFit.h"
#include "Cout.h"

#include <TAttLine.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TClonesArray.h>
#include <TF1.h>
#include <TH1.h>
#include <TLegend.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TObjArray.h>
#include <TRandom.h>
#include <TString.h>
#include <iostream>

#include "FemtoSHCF.h"
#include "StdString.h"
namespace Hal {
  const Int_t CorrFitSHCF::fgRout   = 0;
  const Int_t CorrFitSHCF::fgRside  = 1;
  const Int_t CorrFitSHCF::fgRlong  = 2;
  const Int_t CorrFitSHCF::fgLambda = 3;
  const Int_t CorrFitSHCF::fgNorm   = 4;

  Double_t CorrFitSHCF::GetCFValRe(Double_t q, Int_t elm) const {
    Int_t bin = (q - fAxisMin) * fAxisStepOver;
    if (bin >= fBins) return 1;
    return fCalculatedRe[elm][bin][0] + fCalculatedRe[elm][bin][1] * q + fCalculatedRe[elm][bin][2] * q * q;
  }

  Double_t CorrFitSHCF::GetCFValIm(Double_t q, Int_t elm) const {
    Int_t bin = (q - fAxisMin) * fAxisStepOver;
    if (bin >= fBins) return 1;
    return fCalculatedIm[elm][bin][0] + fCalculatedIm[elm][bin][1] * q + fCalculatedIm[elm][bin][2] * q * q;
  }

  CorrFitSHCF::CorrFitSHCF(Int_t parNo) :
    CorrFitFunc(parNo, 1),
    fCalcMode(eCalcMode::kFullCalc),
    fMaxJM(0),
    fBins(1),
    fLmVals(FemtoYlmIndexes(0)),
    fAxisMin(0),
    fAxisStepOver(0),
    fCovCF(nullptr),
    fYlmBuffer(nullptr) {
    SetParameterName(0, "R_{out}");
    SetParameterName(1, "R_{side}");
    SetParameterName(2, "R_{long}");
    SetParameterName(3, "#lambda");
    SetParameterName(4, "N");
    for (int i = 0; i < GetParametersNo(); i++) {
      SetRange(0, 1);
    }
    fBinRange[0] = fBinRange[1] = 0;
  }

  void CorrFitSHCF::EstimateActiveBins() {
    fActiveBins = 0;
    if (fMask) delete fMask;
    fMask      = new TH1D("mask", "mask", fMaxJM * fNumeratorHistogram->GetNbinsX(), 0, 1);
    Int_t jump = fNumeratorHistogram->GetNbinsX();

    for (int i = fBinRange[0]; i < fBinRange[1]; i++) {
      TH1* num = fNumeratorHistogram;
      if (num->GetBinContent(i) < fThreshold) continue;
      for (int j = 0; j < fMaxJM; j++) {
        fMask->SetBinContent(jump * j + i, 1);
      }
      fActiveBins += fMaxJM * 2;  // x2 bo im + re
    }
    Double_t free_parameters = 0;
    for (int i = 0; i < GetParametersNo(); i++) {
      if (!fParameters[i].IsFixed()) free_parameters++;
    }
    fNDF = fActiveBins - free_parameters;
  }

  double CorrFitSHCF::GetChiTF(const double* par) const {
    Double_t f = 0.0;
    Double_t A, C;
    Double_t e, chi; /* FIXME */
    ParametersChanged();
    Int_t bins = fNumeratorHistogram->GetNbinsX();
    Double_t Q[1];
    chi = -1;
    switch (fCalcMode) {
      case eCalcMode::kFullCalc: {
        /* TODO implemenet full calc */
      } break;
      case eCalcMode::kPhysical1dId: {
        /*  take into account only 00*/
        chi = 0;
        for (int i = 1; i <= bins; i++) {
          if (fMask->GetBinContent(i) == 0) continue;
          Q[0]          = fNumeratorHistogram->GetXaxis()->GetBinCenter(i);
          Double_t cf00 = fCFHistogramsRe[0]->GetBinContent(i);
          Double_t er00 = fCFHistogramsRe[0]->GetBinError(i);
          Double_t th00 = GetCFValRe(Q[0], 0);
          chi += (cf00 - th00) * (cf00 - th00) / (er00 * er00);
        }
      } break;
      case eCalcMode::kPhysical1dNonId: {
        /* take into account only 00 and 11  */
        chi = 0;
        Int_t indexes[2];
        Double_t c_meas[2];
        Double_t c_theo[2];
        Double_t c_error[2];
        Double_t delta[2];
        indexes[0]       = fLmVals.GetIndex(0, 0);
        indexes[1]       = fLmVals.GetIndex(1, 1);
        Int_t covIndex00 = fLmVals.GetIndex(0, 0);
        Int_t covIndex11 = fLmVals.GetIndex(1, 1);
        for (int i = 1; i <= bins; i++) {
          if (fMask->GetBinContent(i) == 0) continue;
          Q[0] = fNumeratorHistogram->GetXaxis()->GetBinCenter(i);
          for (int j = 0; j < 2; j++) {
            c_meas[j]  = fCFHistogramsRe[indexes[j]]->GetBinContent(i);
            c_theo[j]  = GetCFValRe(Q[0], indexes[j]);
            c_error[j] = fCFHistogramsRe[indexes[j]]->GetBinError(i);
            delta[j]   = c_meas[j] - c_theo[j];
          }
          Double_t cov11 = fCovCF->GetBinContent(i, covIndex00 + 1, covIndex11 + 1);  //+1 because this is a histogram
          chi += delta[0] * delta[0] / (c_error[0] * c_error[0]);                     // c00 therm
          chi += delta[1] * delta[1] / (c_error[1] * c_error[1]);                     // c11 therm
          chi += -2. * cov11 * delta[0] * delta[1] / (c_error[0] * c_error[1] * (1 - cov11 * cov11));
        }
      } break;
      case eCalcMode::kPhysical3d: {
        chi = 0;
        /* take into account only 00, 20 and 22 */
        Int_t indexes[3];
        Double_t c_meas[3];
        Double_t c_theo[3];
        Double_t c_error[3];
        Double_t delta[3];

        indexes[0] = fLmVals.GetIndex(0, 0);
        indexes[1] = fLmVals.GetIndex(2, 0);
        indexes[2] = fLmVals.GetIndex(2, 2);

        Int_t covIndex00 = fLmVals.GetIndex(0, 0);
        Int_t covIndex20 = fLmVals.GetIndex(2, 0);
        Int_t covIndex22 = fLmVals.GetIndex(2, 2);

        for (int i = 1; i <= bins; i++) {
          if (fMask->GetBinContent(i) == 0) continue;
          Q[0] = fNumeratorHistogram->GetXaxis()->GetBinCenter(i);
          for (int j = 0; j < 3; j++) {
            c_meas[j]  = fCFHistogramsRe[indexes[j]]->GetBinContent(i);
            c_theo[j]  = GetCFValRe(Q[0], indexes[j]);
            c_error[j] = fCFHistogramsRe[indexes[j]]->GetBinError(i);
            delta[j]   = c_meas[j] - c_theo[j];
          }
          Double_t cov2000 = fCovCF->GetBinContent(i, 1 + covIndex00, 1 + covIndex20);
          Double_t cov2200 = fCovCF->GetBinContent(i, 1 + covIndex00, 1 + covIndex20);
          Double_t cov2220 = fCovCF->GetBinContent(i, 1 + covIndex20, 1 + covIndex22);

          chi += (delta[0] * delta[0]) / (c_error[0] * c_error[0]);  // c00 therm
          chi += (delta[1] * delta[1]) / (c_error[1] * c_error[1]);  // c20 therm
          chi += (delta[2] * delta[2]) / (c_error[2] * c_error[2]);  // c22 therm
          chi += -2. * cov2000 * (delta[0] * delta[1]) / (c_error[0] * c_error[1] * (1 - cov2000 * cov2000));
          chi += -2. * cov2200 * (delta[0] * delta[2]) / (c_error[0] * c_error[2] * (1 - cov2200 * cov2200));
          chi += -2. * cov2220 * (delta[2] * delta[1]) / (c_error[2] * c_error[1] * (1 - cov2220 * cov2220));
          // Double_t cov22 = fCovCF->GetBinContent(i, 1, 1);
        }
      } break;
    }
    return chi;
  }

  TF1* CorrFitSHCF::GetFittingFunction(Option_t* /*opt*/) const {
    return new TF1(
      "funcFitted", this, &CorrFitSHCF::GetFun, fRange.Get(0), fRange.Get(1), GetParametersNo() + 3, this->ClassName(), "GetFun");
  }

  Double_t CorrFitSHCF::GetFun(Double_t* x, Double_t* params) const {
    Int_t l  = params[GetParametersNo()];
    Int_t m  = params[GetParametersNo() + 1];
    Int_t re = params[GetParametersNo() + 2];
    Double_t R[1];
    R[0] = x[0];
    if (re == 1) {
      return GetCFValRe(x[0], fLmVals.GetIndex(l, m));
    } else {
      return GetCFValIm(x[0], fLmVals.GetIndex(l, m));
    }
  }

  void CorrFitSHCF::SetFuncRange(Double_t x_min, Double_t x_max, Int_t el, Int_t em) {
    Int_t l           = fLmVals.GetIndex(el, em);
    fRange[l * 2]     = x_min;
    fRange[l * 2 + 1] = x_max;
  }

  void CorrFitSHCF::Paint(Bool_t repaint, Bool_t refresh) {
    Int_t L = fLmVals.GetMaxL();
    if (repaint)
      if (gPad == nullptr) {
        new TCanvas();
        gPad->Clear();
        gPad->Divide(L + 1, L + 1);
      }
    for (int i = 0; i < GetParametersNo(); i++)
      fTempParamsEval[i] = GetParameter(i);
    RecalculateFunction();
    TVirtualPad* pad = gPad;
    if (fDrawOptions.DrawCf()) ((FemtoSHCF*) fCF)->Draw("short");

    TVirtualPad* temp_pad = gPad;
    // gPad->Divide(fL* 2 - 1, fL);
    if (!repaint) {
      for (int l = 0; l < L; l++) {
        for (int m = -l; m <= l; m++) {
          if (m < 0) continue;
          temp_pad->cd(fLmVals.GetPadId(l, m));
          TF1* f_re = GetFittingFunction(l, m, kTRUE);
          f_re->SetLineColor(kBlue);
          TF1* f_im = GetFittingFunction(l, m, kTRUE);
          f_im->SetLineColor(kRed);
          f_re->Draw("SAME");
          std::pair<TF1*, TVirtualPad*> pair_re, pair_im;
          pair_re.first  = f_re;
          pair_re.second = gPad;
          pair_im.first  = f_im;
          pair_im.second = gPad;
          fDrawFunc.push_back(pair_re);
          fDrawFunc.push_back(pair_im);
        }
      }
    }
    UpdateLegend();
    if (fTempPad == nullptr) {
      TCanvas* c = new TCanvas();
      c->cd();
      fTempPad = gPad;
    }
    if (fDrawOptions.DrawLegend()) {
      fTempPad->cd();
      fLegend->SetX1(0.05);
      fLegend->SetX2(0.95);
      fLegend->SetY1(0.05);
      fLegend->SetY2(0.95);
      fLegend->Draw();
      temp_pad->cd();
    }

    if (refresh) {
      for (auto i : fDrawFunc) {
        i.second->Modified(kTRUE);
        i.second->Update();
      }
    }
    gPad = temp_pad;
  }

  Double_t CorrFitSHCF::Eval(Double_t /*x*/, Double_t /*y*/, Double_t /*z*/) { return 0; }

  void CorrFitSHCF::PrepareRaw() {
    const Int_t L = GetSH()->GetL();
    fMaxJM        = (L + 1) * (L + 1);
    if (fCFHistogramsIm.size() > 0) {
      for (auto i : fCFHistogramsIm)
        delete i;
      for (auto i : fCFHistogramsRe)
        delete i;
    }
    /**
     * prepare some basic variables
     */
    fLmVals.Resize(L);
    fMaxJM = (GetSH()->GetL() + 1) * (GetSH()->GetL() + 1);
    if (fYlmBuffer) delete[] fYlmBuffer;
    fYlmBuffer = new std::complex<double>[fMaxJM];
    /**
     * prepare data
     */
    fCFHistogramsIm.clear();
    fCFHistogramsRe.clear();
    fCFHistogramsRe.resize(fMaxJM);
    fCFHistogramsIm.resize(fMaxJM);
    fCalculatedRe.MakeBigger(fMaxJM, fBins, 3);
    fCalculatedIm.MakeBigger(fMaxJM, fBins, 3);
    if (fCorrelationFunctionHistogram) delete fCorrelationFunctionHistogram;
    fDenominatorHistogram         = GetSH()->GetDen();
    fNumeratorHistogram           = GetSH()->GetNum();
    fCorrelationFunctionHistogram = GetSH()->GetHist(kFALSE);
    fCorrelationFunctionHistogram->SetDirectory(0);
    fKinematics = GetSH()->GetFrame();
    fCovCF      = GetSH()->GetCovCF();

    for (int i = 0; i < fMaxJM; i++) {
      fCFHistogramsRe[i] = (GetSH()->GetCFRe(fLmVals.GetEls(i), fLmVals.GetEms(i)));
      fCFHistogramsIm[i] = (GetSH()->GetCFIm(fLmVals.GetEls(i), fLmVals.GetEms(i)));
    }

    /**
     * prepare stuff for drawing
     */
    Double_t min, max;
    Int_t nbins;
    Hal::Std::GetAxisPar(*fNumeratorHistogram, nbins, min, max, "x");
    fBins         = nbins;
    fAxisMin      = min;
    fAxisStepOver = (max - min) / fBins;
    fDrawFunc.resize(fMaxJM);
  }

  Double_t CorrFitSHCF::CalculateCF(const Double_t* x, const Double_t* params) const { return 0; }

  TF1* CorrFitSHCF::GetFittingFunction(Int_t el, Int_t em, Bool_t re) const {
    TF1* r = GetFittingFunction("");
    r->FixParameter(GetParametersNo(), el);
    r->FixParameter(GetParametersNo() + 1, em);
    if (re) {
      r->FixParameter(GetParametersNo() + 2, 1);
    } else {
      r->FixParameter(GetParametersNo() + 2, 0);
    }
    return r;
  }

  void CorrFitSHCF::Check() {}

  void CorrFitSHCF::RecalculateFunction() const {
    Double_t half    = fNumeratorHistogram->GetXaxis()->GetBinWidth(1) * 0.5;
    const Int_t bins = fNumeratorHistogram->GetXaxis()->GetNbins();
    std::vector<std::complex<double>> fPrev, fNext, fCurrent;
    fPrev.resize(fMaxJM);
    fNext.resize(fMaxJM);
    fCurrent.resize(fMaxJM);
    Double_t a, b, c;
    std::vector<Int_t> indexes = GetIndexesForCalc(fCalcMode);
    for (int i = 1; i <= fNumeratorHistogram->GetNbinsX(); i++) {
      Double_t q      = fNumeratorHistogram->GetXaxis()->GetBinCenter(i);
      Double_t q_low  = q - half;
      Double_t q_mid  = q;
      Double_t q_high = q + half;
      if (i != 1) {  // use prev bin
        fPrev = fNext;
      } else {
        CalculateCF(&q_low, fTempParamsEval);
        fPrev = fYlmValBuffer;
      }
      CalculateCF(&q_mid, fTempParamsEval);
      fCurrent = fYlmValBuffer;
      CalculateCF(&q_high, fTempParamsEval);
      fNext = fYlmValBuffer;
      for (auto j : indexes) {
        Hal::Std::FitParabola(q_low, q_mid, q_high, fPrev[j].real(), fCurrent[j].real(), fNext[j].real(), a, b, c);
        fCalculatedRe[j][i][0] = a;
        fCalculatedRe[j][i][1] = b;
        fCalculatedRe[j][i][2] = c;
        Hal::Std::FitParabola(q_low, q_mid, q_high, fPrev[j].imag(), fCurrent[j].imag(), fNext[j].imag(), a, b, c);
        fCalculatedIm[j][i][0] = a;
        fCalculatedIm[j][i][1] = b;
        fCalculatedIm[j][i][2] = c;
      }
    }
  }

  CorrFitSHCF::~CorrFitSHCF() {
    if (fCFHistogramsIm.size() > 0) {
      for (auto i : fCFHistogramsIm)
        delete i;
      for (auto i : fCFHistogramsRe)
        delete i;
    }
    if (fYlmBuffer) delete[] fYlmBuffer;
    fCovCF = nullptr;
  }

  Double_t CorrFitSHCF::GetDrawableIm(Double_t* x, Double_t* params) const {
    Int_t bin  = (x[0] - fAxisMin) * fAxisStepOver;
    Double_t q = x[0];
    Int_t elm  = params[GetParametersNo()];
    if (bin >= fBins) return 1;
    return fCalculatedIm[elm][bin][0] + fCalculatedIm[elm][bin][1] * q + fCalculatedIm[elm][bin][2] * q * q;
  }

  Double_t CorrFitSHCF::GetDrawableRe(Double_t* x, Double_t* params) const {
    Int_t bin  = (x[0] - fAxisMin) * fAxisStepOver;
    Double_t q = x[0];
    if (bin >= fBins) return 1;
    Int_t elm = params[GetParametersNo()];
    return fCalculatedRe[elm][bin][0] + fCalculatedRe[elm][bin][1] * q + fCalculatedRe[elm][bin][2] * q * q;
  }

  std::vector<Int_t> CorrFitSHCF::GetIndexesForCalc(eCalcMode c) const {
    Int_t Lmax = fLmVals.GetMaxL();
    std::vector<Int_t> vals;
    switch (c) {
      case eCalcMode::kPhysical1dId: {
        vals.push_back(0);
      } break;
      case eCalcMode::kPhysical1dNonId: {
        vals.push_back(fLmVals.GetIndex(0, 0));
        vals.push_back(fLmVals.GetIndex(1, 1));
      } break;
      case eCalcMode::kPhysical3d: {
        vals.push_back(fLmVals.GetIndex(0, 0));
        vals.push_back(fLmVals.GetIndex(2, 0));
        vals.push_back(fLmVals.GetIndex(2, 2));
      } break;
      case eCalcMode::kFullCalc: {
        for (int i = 0; i < fMaxJM; i++) {
          vals.push_back(i);
        }
      } break;
    }
    return vals;
  }
}  // namespace Hal
