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

#include "CorrFit1DCFPainter.h"
#include "CorrFitHDFunc1D.h"
#include "CorrFitMask.h"
#include "CorrFitMask1D.h"
#include "Cout.h"
#include "Femto1DCF.h"
#include "Splines.h"
#include "Std.h"
#include "StdHist.h"
#include "StdString.h"

namespace Hal {

  CorrFit1DCF::CorrFit1DCF(Int_t parameters) : CorrFitFunc(parameters, 1) {
    fNormParIndex = 0;
    if (GetParametersNo() < 2) { HalCoutDebug("1D corrfit func should contain at least 2 params"); }
    if (GetParametersNo() > 2) {  // not only radii fit also lambda
      SetParameterName(LambdaID(), "#lambda");
      fParameters[LambdaID()].SetRange(0, 1);
    }
    fBinX = 0;
    SetParameterName(NormID(), "N");
    SetParameterName(RadiusID(), "R");
  }

  Double_t CorrFit1DCF::EvalDenominator(Double_t x) const {
    Int_t bin = fDenominatorHistogram->FindBin(x);
    return fDenominatorHistogram->GetBinContent(bin);
  }

  void CorrFit1DCF::MakePainter(TString opt) {
    fPainter = new CorrFit1DCFPainter(this, (Femto1DCF*) fCF);
    fPainter->SetOption(opt);
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
    Double_t A, B;
    Double_t e3, chi;
    CorrFitHDFunc1D* cf = static_cast<CorrFitHDFunc1D*>(fHDMaps);
    Bool_t useHD        = kFALSE;
    if (fBinCalc == kExtrapolated) useHD = kTRUE;
    for (int i = 0; i < cf->GetNbins(); i++) {
      fBinX = cf->GetBin(i);
      A     = fNumeratorHistogram->GetBinContent(fBinX);
      B     = fDenominatorHistogram->GetBinContent(fBinX);
      //  x     = fDenominatorHistogram->GetBinCenter(fBinX);

      Double_t Cf, ecf;
      Double_t Cf_theo;
      //  CalcError(A, fNumeratorHistogram->GetBinError(fBinX), B, fDenominatorHistogram->GetBinError(fBinX), Cf, ecf);
      Cf_theo = cf->GetBinCFVal(fBinX, useHD);
      e3      = GetNumericalError(fBinX) * Cf_theo;
      Cf_theo = Cf_theo * B;                              // cf_theo * denominator
      Cf      = A;                                        // just numerator
      ecf     = fNumeratorHistogram->GetBinError(fBinX);  // just sqrt of numerator
      chi     = Cf_theo - Cf;
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
    Double_t A, B, C;
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
    Double_t C;
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
    if (params[GetParametersNo()]) { return EvalCF(x, params) / params[fNormParIndex]; }
    return EvalCF(x, params);
  }

  Double_t CorrFit1DCF::Eval(Double_t q) const {
    fBinX = fDenominatorHistogram->GetXaxis()->FindBin(q);
    return CalculateCF(&q, fTempParamsEval);
  }

  void CorrFit1DCF::EstimateActiveBins() {
    fActiveBins = 0;
    if (fMask == nullptr) {
      fMask = new CorrFitMask1D(fDenominatorHistogram->GetNbinsX(),
                                fDenominatorHistogram->GetXaxis()->GetXmin(),
                                fDenominatorHistogram->GetXaxis()->GetXmax());
    }
    if (fOwnRangeMap == kTRUE) {
      fMask->ApplyThreshold(*fDenominatorHistogram, 0);
    } else {
      GetMask()->Reset(false);
      GetMask()->ApplyRange(fRange[0], fRange[1], true);
      GetMask()->ApplyThreshold(*fNumeratorHistogram, fThreshold);
      GetMask()->ApplyThreshold(*fDenominatorHistogram, fThreshold);
    }
    fMask->Init();
    fActiveBins              = GetMask()->GetActiveBins();
    Double_t free_parameters = 0;
    for (int i = 0; i < GetParametersNo(); i++) {
      if (!fParameters[i].IsFixed()) free_parameters++;
    }

    Bool_t useHD = kFALSE;
    if (fBinCalc == kExtrapolated) useHD = kTRUE;

    fHDMaps->SetMask(*fMask, fDenominatorHistogram, useHD);

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
                             GetParametersNo() + 1,
                             this->ClassName(),
                             "GetFunDrawable");  // last parameter enable normalization if set to 1
    for (int i = 0; i < GetParametersNo(); i++) {
      draw_func->FixParameter(i, GetParameter(i));
      draw_func->SetParName(i, GetParameterName(i));
    }
    draw_func->FixParameter(GetParametersNo(), 0);
    draw_func->SetLineColor(GetLineColor());
    draw_func->SetLineStyle(GetLineStyle());
    draw_func->SetLineWidth(GetLineWidth());
    return draw_func;
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

  Double_t CorrFit1DCF::EvalCFNormalized(const Double_t* x, const Double_t* params) const {
    Double_t val = EvalCF(x, params);
    return val / params[NormID()];
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

  void CorrFit1DCF::SetFittingMask(const CorrFitMask& map) {
    const CorrFitMask1D* mask = dynamic_cast<const CorrFitMask1D*>(&map);
    if (mask == nullptr) return;
    if (!mask->AreCompatible(fCF)) return;
    if (fMask) delete fMask;
    fMask        = (CorrFitMask*) mask->Clone();
    fOwnRangeMap = kTRUE;
  }
}  // namespace Hal
