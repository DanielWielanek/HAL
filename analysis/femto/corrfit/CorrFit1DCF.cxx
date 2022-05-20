/*
 * CorrFit1DCF.cxx
 *
 *  Created on: 02-04-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFit.h"


#include <TAttLine.h>
#include <TAttMarker.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TH1.h>
#include <TLegend.h>
#include <TMath.h>
#include <TNamed.h>
#include <TString.h>

#include "Array.h"
#include "CorrFit1DCF.h"
#include "CorrFitHDFunc.h"
#include "Cout.h"
#include "Femto1DCF.h"
#include "Std.h"
#include "StdHist.h"
#include "StdString.h"
#include "Splines.h"

namespace Hal {
  const Int_t CorrFit1DCF::fgRinv   = 0;
  const Int_t CorrFit1DCF::fgLambda = 1;
  const Int_t CorrFit1DCF::fgNorm   = 2;

  CorrFit1DCF::CorrFit1DCF(Int_t parameters) : CorrFitFunc(parameters, 1) {
    if (GetParametersNo() > 1) {  // not only radii fit also lambda
      SetParameterName(Lambda(), "#lambda");
      fParameters[Lambda()].SetRange(0, 1);
    }
    fBinX = 0;
    if (GetParametersNo() > 2) {  // not only radii & lambda - also norm
      SetParameterName(Norm(), "N");
    }
    SetParameterName(Radius(), "R");
  }

  Double_t CorrFit1DCF::EvalDenominator(Double_t x) const {
    Int_t bin = fDenominatorHistogram->FindBin(x);
    return fDenominatorHistogram->GetBinContent(bin);
  }

  void CorrFit1DCF::Draw(Option_t* draw_option) {
    TString option = draw_option;
    if (gPad == NULL) {
      TCanvas* c = new TCanvas();
      c->cd();
    }
    for (int i = 0; i < GetParametersNo(); i++) {
      fTempParamsEval[i] = GetParameter(i);
    }
    // TODO fFittedFunction->SetParameter(par,val);
    ParametersChanged();
    if (fDrawFunc.size() == 0) fDrawFunc.resize(1);
    Double_t draw_min, draw_max;
    Bool_t set_limits     = ExtrDraw(option, draw_min, draw_max);
    Bool_t drawNormalized = Hal::Std::FindParam(option, "norm", kTRUE);
    Bool_t drawFull       = Hal::Std::FindParam(option, "full", kTRUE);
    Bool_t drawSame       = Hal::Std::FindParam(option, "same", kTRUE);
    if (drawFull) {
      TH1* cf = GetTHForDrawing(drawNormalized);
      cf->SetMarkerStyle(kFullSquare);
      fDrawFunc[0].first = GetFunctionForDrawing();
      if (drawNormalized) fDrawFunc[0].first->SetParameter(Norm(), 1);
      if (set_limits) {
        cf->SetMaximum(draw_max);
        cf->SetMinimum(draw_min);
      }
      cf->Draw();
      fDrawFunc[0].first->Draw("SAME");
      fDrawFunc[0].second = gPad;
      TLegend* leg        = GetLegendForDrawing();
      leg->Draw("SAME");
    } else if (drawSame) {
      fDrawFunc[0].first = GetFunctionForDrawing();
      if (drawNormalized) GetTF1(0)->SetParameter(Norm(), 1);
      GetTF1(0)->Draw("SAME");
    } else {
      fDrawFunc[0].first = GetFunctionForDrawing();
      for (int i = 0; i < GetParametersNo(); i++) {
        GetTF1(0)->FixParameter(i, GetParameter(i));
      }
      Bool_t draw_leg = kFALSE;
      if (Hal::Std::FindParam(option, "leg", kTRUE)) draw_leg = kTRUE;

      if (drawNormalized) GetTF1(0)->SetParameter(Norm(), 1);
      fDrawFunc[0].second = gPad;
      GetTF1(0)->Draw(option);
      if (draw_leg) {
        TLegend* leg = GetLegendForDrawing();
        leg->Draw("SAME");
      }
    }
  }

  void CorrFit1DCF::SetErrors(TH1* num, const TH1* /*den*/) const {
    for (int i = 0; i <= num->GetNbinsX(); i++) {
      Double_t e_num = num->GetBinError(i);
      Double_t e_den = num->GetBinError(i);
      num->SetBinError(i, TMath::Sqrt(e_num * e_num + e_den * e_den));
    }
  }

  double CorrFit1DCF::GetChiTFD(const double* /*par*/) const {
    Double_t f = 0.0;
    Double_t A, B, C;
    Double_t e3, x, chi;
    CorrFitHDFunc1D* cf = static_cast<CorrFitHDFunc1D*>(fHDMaps);
    Bool_t useHD        = kFALSE;
    if (fBinCalc == kExtrapolated) useHD = kTRUE;
    for (int i = 0; i < cf->GetNbins(); i++) {
      fBinX = cf->GetBin(i);
      A     = fNumeratorHistogram->GetBinContent(fBinX);
      B     = fDenominatorHistogram->GetBinContent(fBinX);
      x     = fDenominatorHistogram->GetBinCenter(fBinX);
      Double_t Cf, ecf;
      CalcError(A, fNumeratorHistogram->GetBinError(fBinX), B, fDenominatorHistogram->GetBinError(fBinX), Cf, ecf);
      C   = cf->GetBinCFVal(fBinX, useHD);
      e3  = GetNumericalError(fBinX) * C;
      chi = C - Cf;
      chi *= chi;
      f += chi / (ecf * ecf + e3 * e3);
    }
#ifdef CF_FIT_TRACKING
    for (int i = 0; i < GetParametersNo(); i++) {
      std::cout << Form("%4.3f\t", par[i]);
    }
    std::cout << "->" << f << std::endl;
#endif
    return f;
  }

  double CorrFit1DCF::GetLogTFD(const double* /*par*/) const {
    Double_t f = 0.0;
    Double_t A, B, C, x;
    CorrFitHDFunc1D* cf = static_cast<CorrFitHDFunc1D*>(fHDMaps);
    Bool_t useHD        = kFALSE;
    if (fBinCalc == kExtrapolated) useHD = kTRUE;
    for (int i = 0; i < cf->GetNbins(); i++) {
      fBinX         = cf->GetBin(i);
      A             = fNumeratorHistogram->GetBinContent(fBinX);
      B             = fDenominatorHistogram->GetBinContent(fBinX);
      x             = fDenominatorHistogram->GetBinCenter(fBinX);
      C             = cf->GetBinCFVal(fBinX, useHD);
      Double_t logA = (C * (A + B)) / (A * (C + 1.0));
      Double_t logB = (A + B) / (B * (C + 1.0));
      f += -(A * TMath::Log(logA) + B * TMath::Log(logB));
      /// cf retur nan for some cfs
    }
#ifdef CF_FIT_TRACKING
    for (int i = 0; i < GetParametersNo(); i++) {
      std::cout << Form("%4.3f\t", par[i]);
    }
    std::cout << "->" << f << std::endl;
#endif
    return f;
  }

  double CorrFit1DCF::GetChiTF(const double* /*par*/) const {
    Double_t f = 0.0;
    Double_t C, x;
    CorrFitHDFunc1D* Cf = static_cast<CorrFitHDFunc1D*>(fHDMaps);
    Bool_t useHD        = kFALSE;
    if (fBinCalc == kExtrapolated) useHD = kTRUE;
    for (int i = 0; i < Cf->GetNbins(); i++) {
      fBinX          = Cf->GetBin(i);
      Double_t cf    = fCorrelationFunctionHistogram->GetBinContent(fBinX);
      x              = fDenominatorHistogram->GetBinCenter(fBinX);
      C              = Cf->GetBinCFVal(fBinX, useHD);
      Double_t ea    = fCorrelationFunctionHistogram->GetBinError(fBinX);
      Double_t eb    = GetNumericalError(fBinX);
      ea             = (ea + eb);
      Double_t delta = C - cf;
      f += delta * delta / (ea * ea);
    }
#ifdef CF_FIT_TRACKING
    for (int i = 0; i < GetParametersNo(); i++) {
      std::cout << Form("%4.3f\t", par[i]);
    }
    std::cout << "->" << f << std::endl;
#endif
    return f;
  }

  void CorrFit1DCF::ReadParametersName() {
    switch (fKinematics) {
      case Femto::EKinematics::kLCMS:
        if (GetParameterName(Radius()) == "R") SetParameterName(Radius(), "R_{inv}");
        break;
      case Femto::EKinematics::kPRF:
        if (GetParameterName(Radius()) == "R") SetParameterName(Radius(), "R*");
        break;
      default: break;
    }
  }

  Double_t CorrFit1DCF::GetFunDrawable(Double_t* x, Double_t* params) const {
    fBinX = fDenominatorHistogram->GetXaxis()->FindBin(x[0]);
    return EvalCF(x, params);
  }

  Double_t CorrFit1DCF::Eval(Double_t q) const {
    fBinX = fDenominatorHistogram->GetXaxis()->FindBin(q);
    return CalculateCF(&q, fTempParamsEval);
  };

  void CorrFit1DCF::EstimateActiveBins() {
    fActiveBins = 0;
    if (fOwnRangeMap) {
      if (!Hal::Std::AreSimilar(fDenominatorHistogram, fMask, kFALSE)) {
        delete fMask;
        fMask = NULL;
        Cout::Text("Non compatible mask in corrfit", "L", kOrange);
        fOwnRangeMap = kFALSE;
      }
    }
    if (fMask == NULL) {
      fMask = new TH1I("mask",
                       "mask",
                       fDenominatorHistogram->GetNbinsX(),
                       fDenominatorHistogram->GetXaxis()->GetXmin(),
                       fDenominatorHistogram->GetXaxis()->GetXmax());
    }
    if (fOwnRangeMap == kTRUE) {
      Bool_t warn = kTRUE;
      for (int i = 1; i <= fDenominatorHistogram->GetNbinsX(); i++) {
        Int_t mask = fMask->GetBinContent(i);
        if (fDenominatorHistogram->GetBinContent(i) == 0 || fNumeratorHistogram->GetBinContent(i) == 0) {
          mask = 0;
          fMask->SetBinContent(i, 0);
          if (warn) {
            Cout::PrintInfo("empty bin,  mask must be changed", EInfo::kImportantWarning);
            warn = kFALSE;
          }
        }

        if (mask > 0) fActiveBins++;
      }
    } else {
      Int_t bin_min = fDenominatorHistogram->FindBin(fRange[0]);
      Int_t bin_max = fDenominatorHistogram->FindBin(fRange[1]);
      fMask->Reset();
      for (int i = bin_min; i <= bin_max; i++) {
        Double_t A = fNumeratorHistogram->GetBinContent(i);
        Double_t B = fDenominatorHistogram->GetBinContent(i);
        if (A <= fThreshold || B < fThreshold) continue;
        fActiveBins++;
        fMask->SetBinContent(i, 1);
      }
    }
    Double_t free_parameters = 0;
    for (int i = 0; i < GetParametersNo(); i++) {
      if (!fParameters[i].IsFixed()) free_parameters++;
    }

    Bool_t useHD = kFALSE;
    if (fBinCalc == kExtrapolated) useHD = kTRUE;

    fHDMaps->SetMask(fMask, fDenominatorHistogram, useHD);

    fNDF = fActiveBins - free_parameters;
  }

  CorrFit1DCF::~CorrFit1DCF() {}

  void CorrFit1DCF::Fit(TObject* histo) {
    TH1* num = ((DividedHisto1D*) histo)->GetNum();
    CalculateNumErrors(num);
    CorrFitFunc::Fit(histo);
  }

  TF1* CorrFit1DCF::GetFunctionForDrawing() const {
    TF1* draw_func = new TF1(Hal::Std::GetUniqueName("func_draw"),
                             this,
                             &CorrFit1DCF::GetFunDrawable,
                             fRange.Get(0),
                             fRange.Get(1),
                             GetParametersNo(),
                             this->ClassName(),
                             "GetFunDrawable");
    for (int i = 0; i < GetParametersNo(); i++) {
      draw_func->FixParameter(i, GetParameter(i));
      draw_func->SetParName(i, GetParameterName(i));
    }
    draw_func->SetLineColor(GetLineColor());
    draw_func->SetLineStyle(GetLineStyle());
    draw_func->SetLineWidth(GetLineWidth());
    return draw_func;
  }

  TH1* CorrFit1DCF::GetTHForDrawing(Bool_t normalize) const {
    TH1* cf = (TH1*) ((Femto1DCF*) fCF)->GetHist(kFALSE);
    cf->SetName(Hal::Std::GetUniqueName(cf->GetName()));
    if (normalize) cf->Scale(1.0 / GetNorm());
    cf->SetStats(kFALSE);
    cf->SetMinimum(0);
    return cf;
  }

  TLegend* CorrFit1DCF::GetLegendForDrawing() const {
    TLegend* leg = new TLegend(0.7, 0.7, 0.95, 0.95);
    leg->SetHeader("1D Corrfit");
    for (int i = 0; i < GetParametersNo(); i++) {
      if (IsParFixed(i)) {
        leg->AddEntry(
          (TObject*) 0x0, Form("%s %4.3f (fixed)", fParameters[i].GetParName().Data(), fParameters[i].GetFittedValue()), "");
      } else {
        leg->AddEntry(
          (TObject*) 0x0,
          Form(
            "%s %4.3f#pm%4.3f", fParameters[i].GetParName().Data(), fParameters[i].GetFittedValue(), fParameters[i].GetError()),
          "");
      }
    }
    TString chi_s, chindf_s, ndf_s;
    Double_t chi    = GetChiSquare();
    Double_t chindf = GetChiNDF();
    Double_t ndf    = GetNDF();
    if (chi <= 1000)
      chi_s = Form("%4.3f", chi);
    else
      chi_s = Hal::Std::RoundToString(chi, 2, "prefix");
    if (chindf <= 1000) {
      chindf_s = Form("%4.3f", chindf);
    } else {
      chindf_s = Hal::Std::RoundToString(chindf, 2, "prefix");
    }
    if (ndf <= 1000) {
      ndf_s = Form("%i", (int) ndf);
    } else {
      ndf_s = Hal::Std::RoundToString(ndf, 2, "prefix");
    }
    leg->AddEntry((TObject*) 0x0, Form("#chi^{2}/NDF %s (%s/%s)", chindf_s.Data(), chi_s.Data(), ndf_s.Data()), "");
    return leg;
  }

  Double_t CorrFit1DCF::EvalCF(const Double_t* x, const Double_t* params) const {
    if (fBinCalc == kExtrapolated) {
      Double_t X[3];
      X[0]             = fDenominatorHistogram->GetXaxis()->GetBinLowEdge(fBinX);
      X[1]             = fDenominatorHistogram->GetXaxis()->GetBinCenter(fBinX);
      X[2]             = fDenominatorHistogram->GetXaxis()->GetBinUpEdge(fBinX);
      Double_t val     = 0;
      const Int_t bin0 = fBinX * 2 - 2;
      for (int i = 0; i < 3; i++) {
        Double_t n_temp = static_cast<CorrFitHDFunc1D*>(fHDMaps)->GetDenominatorHD()[bin0 + i];
        val += n_temp * static_cast<CorrFitHDFunc1D*>(fHDMaps)->GetCFMapHD()[bin0 + i];
      }
      return val * static_cast<CorrFitHDFunc1D*>(fHDMaps)->GetDenominatorSum()[fBinX];
    }
    return CalculateCF(x, params);
  }

  void CorrFit1DCF::RecalculateSmoothFunction() const {
    Double_t half       = fDenominatorHistogram->GetXaxis()->GetBinWidth(1);
    CorrFitHDFunc1D* cf = static_cast<CorrFitHDFunc1D*>(fHDMaps);
    Double_t X[1];
    for (int i = 0; i < cf->GetBinsHD().GetSize(); i++) {
      Int_t hdBin             = cf->GetBinsHD()[i];
      X[0]                    = cf->EvalHD(hdBin);
      fBinX                   = cf->HDBinToBin(hdBin);
      Double_t p              = CalculateCF(X, fTempParamsEval);
      cf->GetCFMapHD()[hdBin] = p;
    }
  }
}  // namespace Hal
