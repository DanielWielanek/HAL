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
#include <TLegendEntry.h>
#include <TMath.h>
#include <TNamed.h>
#include <TString.h>
#include <TSystem.h>

#include "Array.h"
#include "CorrFit1DCF.h"
#include "CorrFitDrawOptions.h"
#include "CorrFitHDFunc.h"
#include "Cout.h"
#include "Femto1DCF.h"
#include "Splines.h"
#include "Std.h"
#include "StdHist.h"
#include "StdString.h"

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

  void CorrFit1DCF::Paint(Bool_t repaint, Bool_t refresh) {
    if (repaint)
      if (gPad == nullptr) {
        TCanvas* c = new TCanvas();
        fTempPad   = c->cd();
      }
    if (fTempPad == nullptr) { fTempPad = gPad; }
    for (int i = 0; i < GetParametersNo(); i++) {
      fTempParamsEval[i] = GetParameter(i);
    }
    // TODO fFittedFunction->SetParameter(par,val);
    ParametersChanged();

    if (fDrawFunc.size() == 0) {
      fDrawFunc.resize(1);
      fDrawFunc[0].first  = GetFunctionForDrawing();
      fDrawFunc[0].second = fTempPad;
    }
    CopyParamsToTF1(GetTF1(0), kTRUE, kTRUE);
    if (fDrawOptions.DrawCf()) {
      MakeTHForDrawing();
      if (fDrawOptions.AutoNorm()) { fDrawHistograms[0]->Scale(1.0 / GetNorm()); }
      fDrawHistograms[0]->SetMarkerStyle(kFullSquare);
      if (fDrawOptions.DrawMinMax()) {
        fDrawHistograms[0]->SetMinimum(fDrawOptions.GetMin());
        fDrawHistograms[0]->SetMaximum(fDrawOptions.GetMax());
      }
      fDrawHistograms[0]->Draw("SAME");
    }
    if (fDrawOptions.AutoNorm()) {
      GetTF1(0)->SetParameter(Norm(), 1);
      GetTF1(0)->Draw("SAME");
    } else {
      GetTF1(0)->Draw("SAME");
    }
    UpdateLegend();
    if (fLegend) {
      fTempPad->cd();
      fLegend->Draw("SAME");
    }

    if (refresh) {
      fTempPad->Modified(kTRUE);
      fTempPad->Update();
#ifdef __APPLE__
      for (int ispe = 0; ispe < 2; ispe++) {
        if (gSystem->ProcessEvents()) break;
      }
#endif
    }
  }

  void CorrFit1DCF::SetErrors(TH1* num, const TH1* /*den*/) const {
    for (int i = 0; i <= num->GetNbinsX(); i++) {
      Double_t e_num = num->GetBinError(i);
      Double_t e_den = num->GetBinError(i);
      num->SetBinError(i, TMath::Sqrt(e_num * e_num + e_den * e_den));
    }
  }

  void CorrFit1DCF::PrepareRaw() {
    Prepare();
    if (fCorrelationFunctionHistogram) delete fCorrelationFunctionHistogram;
    fDenominatorHistogram         = ((DividedHisto1D*) fCF)->GetDen();
    fNumeratorHistogram           = ((DividedHisto1D*) fCF)->GetNum();
    fCorrelationFunctionHistogram = ((DividedHisto1D*) fCF)->GetHist(kFALSE);
    fCorrelationFunctionHistogram->SetDirectory(0);
    fKinematics = static_cast<Femto1DCF*>(fCF)->GetFrame();
    if (fHDMaps == nullptr) fHDMaps = new CorrFitHDFunc1D();
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
      //  x     = fDenominatorHistogram->GetBinCenter(fBinX);
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
      fBinX = cf->GetBin(i);
      A     = fNumeratorHistogram->GetBinContent(fBinX);
      B     = fDenominatorHistogram->GetBinContent(fBinX);
      //  x             = fDenominatorHistogram->GetBinCenter(fBinX);
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
      fBinX       = Cf->GetBin(i);
      Double_t cf = fCorrelationFunctionHistogram->GetBinContent(fBinX);
      // x              = fDenominatorHistogram->GetBinCenter(fBinX);
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
            Cout::PrintInfo("empty bin,  mask must be changed", EInfo::kWarning);
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

  void CorrFit1DCF::MakeTHForDrawing() {
    if (fDrawHistograms.size() == 0) {
      TH1* h = (TH1*) ((Femto1DCF*) fCF)->GetHist(kFALSE);
      h->SetName(Hal::Std::GetUniqueName(h->GetName()));
      h->SetMinimum(0);
      h->SetStats(kFALSE);
      fDrawHistograms.push_back(h);
    }
    TH1* cf = (TH1*) ((Femto1DCF*) fCF)->GetHist(kFALSE);  // TODO direct calculate without creation of TH1
    fDrawHistograms[0]->ResetStats();
    for (int i = 1; i <= fDrawHistograms[0]->GetNbinsX(); i++) {
      fDrawHistograms[0]->SetBinContent(i, cf->GetBinContent(i));
      fDrawHistograms[0]->SetBinError(i, cf->GetBinError(i));
    }
    delete cf;
  }

  Double_t CorrFit1DCF::EvalCF(const Double_t* x, const Double_t* params) const {
    if (fBinCalc == kExtrapolated) {
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
