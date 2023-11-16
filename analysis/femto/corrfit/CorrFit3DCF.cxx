/*
 * CorrFit3DCF.cxx
 *
 *  Created on: 08-04-2015
 *      Author: Daniel Wielanek
 *      E-mail: daniel.wielanek@gmail.com
 *      Warsaw University of Technology, Faculty of Physics
 */

#include "CorrFit.h"

#include <TAttLine.h>
#include <TAttMarker.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TF3.h>
#include <TH1.h>
#include <TH3.h>
#include <TLegend.h>
#include <TMath.h>
#include <TNamed.h>
#include <TString.h>
#include <iostream>

#include "Array.h"
#include "CorrFit3DCF.h"
#include "CorrFitHDFunc3D.h"
#include "CorrFitMask.h"
#include "CorrFitMask3D.h"
#include "Cout.h"
#include "DividedHisto.h"
#include "Femto3DCF.h"
#include "Splines.h"
#include "Std.h"
#include "StdHist.h"
#include "StdString.h"

namespace Hal {
  const Int_t CorrFit3DCF::fgRout   = 0;
  const Int_t CorrFit3DCF::fgRside  = 1;
  const Int_t CorrFit3DCF::fgRlong  = 2;
  const Int_t CorrFit3DCF::fgLambda = 3;
  const Int_t CorrFit3DCF::fgNorm   = 4;

  CorrFit3DCF::CorrFit3DCF(Int_t parameters) :
    CorrFitFunc(parameters, 3),
    fXbins(NULL),
    fYbins(NULL),
    fZbins(NULL),
    fXBinf(0),
    fYBinf(0),
    fZBinf(0),
    fXAxisf(0),
    fYAxisf(0),
    fZAxisf(0) {
    for (int i = 0; i < 3; i++) {
      fRange[2 * i]     = 0;
      fRange[2 * i + 1] = 1.0;
    }
    SetParameterName(Rout(), "R_{out}");
    SetParameterName(Rside(), "R_{side}");
    SetParameterName(Rlong(), "R_{long}");
    if (parameters < 3) {
      Cout::PrintInfo(Form("%s must have at least 3 parameters", this->ClassName()), EInfo::kWarning);
      return;
    }
    if (parameters > 3) fParameters[3].SetParName("#lambda");
    if (parameters > 4) fParameters[4].SetParName("N");
    fXbins = new Array_1<Double_t>(1);
    fYbins = new Array_1<Double_t>(1);
    fZbins = new Array_1<Double_t>(1);
  }

  Double_t CorrFit3DCF::EvalDenominator(Double_t x, Double_t y, Double_t z) const {
    Int_t binx = fDenominatorHistogram->GetXaxis()->FindBin(x);
    Int_t biny = fDenominatorHistogram->GetYaxis()->FindBin(y);
    Int_t binz = fDenominatorHistogram->GetZaxis()->FindBin(z);
    return fDenominatorHistogram->GetBinContent(binx, biny, binz);
  }

  Double_t CorrFit3DCF::GetFunX(Double_t* x, Double_t* params) const {
    // co z mapowanymcorrfitem?
    fBinX = fDenominatorHistogram->GetXaxis()->FindBin(x[0]);
    fBinY = fDenominatorHistogram->GetYaxis()->FindBin((*fYbins)[0]);
    fBinZ = fDenominatorHistogram->GetZaxis()->FindBin((*fZbins)[0]);
    if (fBinCalc == kExtrapolated) {  // calculate "step function"
      Double_t X[3] = {
        fDenominatorHistogram->GetXaxis()->GetBinCenter(fBinX),
        fDenominatorHistogram->GetYaxis()->GetBinCenter(fBinY),
        fDenominatorHistogram->GetZaxis()->GetBinCenter(fBinZ),
      };
      return EvalCF(X, params);
    }

    // simple make simple average operation
    Double_t sum = 0;
    Double_t ctn = fYbins->GetSize() + fZbins->GetSize();
    Int_t binY   = (fYbins->GetSize() - 1) / 2;
    Int_t binZ   = (fZbins->GetSize() - 1) / 2;
    for (int i = 0; i < fYbins->GetSize(); i++) {
      Double_t tempX[3] = {x[0], (*fYbins)[i], (*fZbins)[binZ]};
      sum += CalculateCF(tempX, params);
    }
    for (int j = 0; j < fZbins->GetSize(); j++) {
      Double_t tempX[3] = {x[0], (*fYbins)[binY], (*fZbins)[j]};
      sum += CalculateCF(tempX, params);
    }
    return sum / ctn;
  }

  Double_t CorrFit3DCF::GetFunY(Double_t* x, Double_t* params) const {
    fBinY = fDenominatorHistogram->GetYaxis()->FindBin(x[0]);
    fBinX = fDenominatorHistogram->GetXaxis()->FindBin((*fXbins)[0]);
    fBinZ = fDenominatorHistogram->GetZaxis()->FindBin((*fZbins)[0]);
    if (fBinCalc == kExtrapolated) {  // calculate "step function"
      Double_t X[3] = {
        fDenominatorHistogram->GetXaxis()->GetBinCenter(fBinX),
        fDenominatorHistogram->GetYaxis()->GetBinCenter(fBinY),
        fDenominatorHistogram->GetZaxis()->GetBinCenter(fBinZ),
      };
      return EvalCF(X, params);
    }
    Double_t sum = 0;
    Double_t ctn = fXbins->GetSize() + fZbins->GetSize();
    Int_t binZ   = (fZbins->GetSize() - 1) / 2;
    Int_t binX   = (fXbins->GetSize() - 1) / 2;
    for (int i = 0; i < fXbins->GetSize(); i++) {
      Double_t tempX[3] = {(*fXbins)[i], x[0], (*fZbins)[binZ]};
      sum += CalculateCF(tempX, params);
    }
    for (int j = 0; j < fZbins->GetSize(); j++) {
      Double_t tempX[3] = {(*fXbins)[binX], x[0], (*fZbins)[j]};
      sum += CalculateCF(tempX, params);
    }
    return sum / ctn;
  }

  Double_t CorrFit3DCF::GetFunZ(Double_t* x, Double_t* params) const {
    fBinZ = fDenominatorHistogram->GetZaxis()->FindBin(x[0]);
    fBinX = fDenominatorHistogram->GetXaxis()->FindBin((*fXbins)[0]);
    fBinY = fDenominatorHistogram->GetYaxis()->FindBin(((*fYbins)[0]));
    if (fBinCalc == kExtrapolated) {  // calculate "step function"
      Double_t X[3] = {
        fDenominatorHistogram->GetXaxis()->GetBinCenter(fBinX),
        fDenominatorHistogram->GetYaxis()->GetBinCenter(fBinY),
        fDenominatorHistogram->GetZaxis()->GetBinCenter(fBinZ),
      };
      return EvalCF(X, params);
    }
    Double_t sum = 0;
    Double_t ctn = fXbins->GetSize() + fYbins->GetSize();
    Int_t binX   = (fXbins->GetSize() - 1) / 2;
    Int_t binY   = (fYbins->GetSize() - 1) / 2;
    for (int i = 0; i < fXbins->GetSize(); i++) {
      Double_t tempX[3] = {(*fXbins)[i], (*fYbins)[binY], x[0]};
      sum += CalculateCF(tempX, params);
    }
    for (int j = 0; j < fYbins->GetSize(); j++) {
      Double_t tempX[3] = {(*fXbins)[binX], (*fYbins)[j], x[0]};
      sum += CalculateCF(tempX, params);
    }
    return sum / ctn;
  }

  void CorrFit3DCF::SetFuncRange(Double_t x_min, Double_t x_max, Double_t y_min, Double_t y_max, Double_t z_min, Double_t z_max) {
    fRange[0] = x_min;
    fRange[1] = x_max;
    fRange[2] = y_min;
    fRange[3] = y_max;
    fRange[4] = z_min;
    fRange[5] = z_max;
  }

  void CorrFit3DCF::Paint(Bool_t repaint, Bool_t refresh) {
    for (int i = 0; i < GetParametersNo(); i++) {
      fTempParamsEval[i] = GetParameter(i);
    }
    // TODO fFittedFunction->SetParameter(par,val);
    ParametersChanged();
    if (!repaint) Calculatef(0);
    if (gPad == nullptr) {
      new TCanvas();
      fTempPad = gPad;
    }
    if (fTempPad == nullptr) fTempPad = gPad;
    EDrawMode dm = EDrawMode::kNormal;
    Int_t padX = 0, padY = 0;
    Int_t legendPad = 0;
    if (fDrawOptions.Diag1()) {
      dm        = EDrawMode::kDiagonal1;
      padX      = 3;
      padY      = 3;
      legendPad = 9;
    } else if (fDrawOptions.Diag2()) {
      dm        = EDrawMode::kDiagonal2;
      padX      = 4;
      padY      = 4;
      legendPad = 15;
    } else {
      dm        = EDrawMode::kNormal;
      padX      = 2;
      padY      = 2;
      legendPad = 4;
    }
    GetTF1s(!repaint, dm);
    if (fDrawOptions.DrawCf()) { GetTH1s(dm); }


    Color_t colz[13] = {kRed, kBlue, kGreen, kViolet, kOrange, kCyan, kBlack, kGray, kMagenta, kYellow + 1, kPink, kAzure, kTeal};
    if (fDrawOptions.Rgb()) {
      int count = 0;
      for (auto f : fDrawFunc) {
        if (count < 13) { f.first->SetLineColor(colz[count++]); }
      }
    }

    if (!repaint) { fTempPad->Divide(padX, padY); }

    int pad = 0;
    for (auto f : fDrawFunc) {
      fTempPad->cd(++pad);
      if (fDrawOptions.DrawCf()) {
        if (fDrawOptions.AutoNorm() && fOldNorm != 0) { fDrawHistograms[pad - 1]->Scale(fOldNorm / GetNorm()); }
        if (fDrawOptions.DrawMinMax()) {
          fDrawHistograms[pad - 1]->SetMinimum(fDrawOptions.GetMin());
          fDrawHistograms[pad - 1]->SetMaximum(fDrawOptions.GetMax());
        }
        fDrawHistograms[pad - 1]->Draw("same");
      }
      if (fDrawOptions.AutoNorm()) { f.first->SetParameter(Norm(), 1); }
      f.first->Draw("SAME");

      f.second = gPad;
    }
    UpdateLegend();
    fTempPad->cd(legendPad);
    if (fDrawOptions.DrawLegend()) {
      fLegend->SetX1(0.05);
      fLegend->SetX2(0.95);
      fLegend->SetY1(0.05);
      fLegend->SetY2(0.95);
      fLegend->Draw();
    }


    if (refresh) {  //?
      for (unsigned int i = 0; i < fDrawFunc.size(); i++) {
        fTempPad->cd(i + 1);
        gPad->Modified();
        gPad->Update();
      }
    }
    gPad     = fTempPad;
    fOldNorm = GetNorm();
  }

  void CorrFit3DCF::SetParametersToTF1(TF1* f) const {
    for (int i = 0; i < GetParametersNo(); i++) {
      f->FixParameter(i, GetParameter(i));
    }
  }

  void CorrFit3DCF::SetErrors(TH1* num, const TH1* den) const {
    for (int i = 0; i <= num->GetXaxis()->GetNbins(); i++) {
      for (int j = 0; j <= num->GetYaxis()->GetNbins(); j++) {
        for (int k = 0; k <= num->GetZaxis()->GetNbins(); k++) {
          Double_t ea = num->GetBinError(i, j, k);
          Double_t eb = den->GetBinError(i, j, k);
          num->SetBinError(i, j, k, TMath::Sqrt(ea * ea + eb * eb));
        }
      }
    }
  }

  void CorrFit3DCF::PrepareRaw() {
    Prepare();
    if (fCorrelationFunctionHistogram) delete fCorrelationFunctionHistogram;
    fDenominatorHistogram         = ((DividedHisto1D*) fCF)->GetDen();
    fNumeratorHistogram           = ((DividedHisto1D*) fCF)->GetNum();
    fCorrelationFunctionHistogram = ((DividedHisto1D*) fCF)->GetHist(kFALSE);
    fCorrelationFunctionHistogram->SetDirectory(0);
    fKinematics = static_cast<Femto3DCF*>(fCF)->GetFrame();
    if (fHDMaps == nullptr) fHDMaps = new CorrFitHDFunc3D();
  }

  double CorrFit3DCF::GetChiTFD(const double* /*par*/) const {
    Double_t f = 0.0;
    Double_t A, B, C;
    Double_t e, e1, e2, chi; /* FIXME */
    Bool_t useHD = kFALSE;
    if (fBinCalc == kExtrapolated) useHD = kTRUE;
    CorrFitHDFunc3D* cf = static_cast<CorrFitHDFunc3D*>(fHDMaps);
    for (int i = 0; i < cf->GetNBins(); i++) {
      fBinX = cf->GetXBin(i);
      fBinY = cf->GetYBin(i);
      fBinZ = cf->GetZBin(i);

      A   = fNumeratorHistogram->GetBinContent(fBinX, fBinY, fBinZ);
      B   = fDenominatorHistogram->GetBinContent(fBinX, fBinY, fBinZ);
      e1  = fNumeratorHistogram->GetBinError(fBinX, fBinY, fBinZ);
      e   = e1 * e1;
      C   = cf->GetBinCFVal(fBinX, fBinY, fBinZ, useHD);
      chi = A - C * B;
      e2  = B * GetNumericalError(fBinX, fBinY, fBinZ);
      e += e2 * e2;
      if (e == 0) continue;
      f += chi * chi / e;
    }
#ifdef CF_FIT_TRACKING
    for (int i = 0; i < GetParametersNo(); i++) {
      std::cout << Form("%4.3f\t", par[i]);
    }
    std::cout << "->" << f / z << std::endl;
#endif
    return f;
  }

  double CorrFit3DCF::GetChiTF(const double* /*par*/) const {
    Double_t f = 0.0;
    Double_t A, B, C;
    Double_t e, e2, chi;
    Bool_t useHD = kFALSE;
    if (fBinCalc == kExtrapolated) useHD = kTRUE;
    CorrFitHDFunc3D* cf = static_cast<CorrFitHDFunc3D*>(fHDMaps);
    for (int i = 0; i < cf->GetNBins(); i++) {
      fBinX = cf->GetXBin(i);
      fBinY = cf->GetYBin(i);
      fBinZ = cf->GetZBin(i);
      A     = fNumeratorHistogram->GetBinContent(fBinX, fBinY, fBinZ);
      B     = fDenominatorHistogram->GetBinContent(fBinX, fBinY, fBinZ);
      e     = fCorrelationFunctionHistogram->GetBinError(fBinX, fBinY, fBinZ);
      e *= e;
      C   = cf->GetBinCFVal(fBinX, fBinY, fBinZ, useHD);
      chi = A / B - C;
      e2  = GetNumericalError(fBinX, fBinY, fBinZ);
      e += e2 * e2;
      if (e == 0) continue;
      f += chi * chi / e;
    }
#ifdef CF_FIT_TRACKING
    for (int i = 0; i < GetParametersNo(); i++) {
      std::cout << Form("%4.3f\t", par[i]);
    }
    std::cout << "->" << f << std::endl;
#endif
    return f;
  }

  double CorrFit3DCF::GetLogTFD(const double* /*par*/) const {
    Double_t f = 0.0;
    Double_t A, B, C;
    Bool_t useHD = kFALSE;
    if (fBinCalc == kExtrapolated) useHD = kTRUE;
    CorrFitHDFunc3D* cf = static_cast<CorrFitHDFunc3D*>(fHDMaps);
    for (int i = 0; i < cf->GetNBins(); i++) {
      fBinX         = cf->GetXBin(i);
      fBinY         = cf->GetYBin(i);
      fBinZ         = cf->GetZBin(i);
      A             = fNumeratorHistogram->GetBinContent(fBinX, fBinY, fBinZ);
      B             = fDenominatorHistogram->GetBinContent(fBinX, fBinY, fBinZ);
      C             = cf->GetBinCFVal(fBinX, fBinY, fBinZ, useHD);
      Double_t logA = (C * (A + B)) / (A * (C + 1.0));
      Double_t logB = (A + B) / (B * (C + 1.0));
      Double_t step = -(A * TMath::Log(logA) + B * TMath::Log(logB));
      // Double_t step = -A*(TMath::Log(logA*TMath::Power(logB,B/A)));
      if (TMath::IsNaN(step)) continue;
      f += step;
    }
#ifdef CF_FIT_TRACKING
    for (int i = 0; i < GetParametersNo(); i++) {
      std::cout << Form("%4.3f\t", par[i]);
    }
    std::cout << "->" << f << std::endl;
#endif
    return f;
  }

  void CorrFit3DCF::EstimateActiveBins() {
    fActiveBins = 0;
    if (fMask == nullptr) {
      fMask        = new CorrFitMask3D(fDenominatorHistogram->GetNbinsX(),
                                fDenominatorHistogram->GetXaxis()->GetXmin(),
                                fDenominatorHistogram->GetXaxis()->GetXmax(),
                                fDenominatorHistogram->GetNbinsY(),
                                fDenominatorHistogram->GetYaxis()->GetXmin(),
                                fDenominatorHistogram->GetYaxis()->GetXmax(),
                                fDenominatorHistogram->GetNbinsZ(),
                                fDenominatorHistogram->GetZaxis()->GetXmin(),
                                fDenominatorHistogram->GetZaxis()->GetXmax());
      fOwnRangeMap = kFALSE;
    }
    if (fOwnRangeMap) {  // use own map
    } else {             // get own mask
      GetMask()->Reset(true);
      GetMask()->ApplyThreshold(*fNumeratorHistogram, fThreshold);
      GetMask()->ApplyThreshold(*fDenominatorHistogram, fThreshold);
      GetMask()->ApplyRange(fRange[0], fRange[1], fRange[2], fRange[3], fRange[4], fRange[5], kFALSE);
    }
    GetMask()->ApplyThreshold(*fNumeratorHistogram, 0);
    GetMask()->ApplyThreshold(*fDenominatorHistogram, 0);
    fMask->Init();
    fActiveBins  = fMask->GetActiveBins();
    Bool_t useHD = kFALSE;
    if (fBinCalc == kExtrapolated) useHD = kTRUE;

    fHDMaps->SetMask(*fMask, fDenominatorHistogram, useHD);
    Double_t free_parameters = 0;
    for (int i = 0; i < GetParametersNo(); i++) {
      if (!fParameters[i].IsFixed()) free_parameters++;
    }
    fNDF = fActiveBins - free_parameters;
  }

  void CorrFit3DCF::SetRadiusLimits(Double_t min, Double_t max) {
    SetRoutLimits(min, max);
    SetRsideLimits(min, max);
    SetRlongLimits(min, max);
  }

  CorrFit3DCF::~CorrFit3DCF() {
    delete fXbins;
    delete fYbins;
    delete fZbins;
  }

  Double_t CorrFit3DCF::Eval(Double_t x, Double_t y, Double_t z) {
    Double_t q[3] = {x, y, z};
    return CalculateCF(q, fTempParamsEval);
  }

  Double_t CorrFit3DCF::EvalCF(const Double_t* xx, const Double_t* params) const {
    if (fBinCalc == kExtrapolated) {
      Double_t x[3];
      Double_t y[3];
      Double_t z[3];
      Double_t val = 0;
      x[1]         = fDenominatorHistogram->GetXaxis()->GetBinCenter(fBinX);
      x[0]         = x[1] - fXBinf;
      x[2]         = x[1] + fXBinf;

      y[1] = fDenominatorHistogram->GetYaxis()->GetBinCenter(fBinY);
      y[0] = y[1] - fYBinf;
      y[2] = y[1] + fYBinf;

      z[1]        = fDenominatorHistogram->GetZaxis()->GetBinCenter(fBinZ);
      z[0]        = z[1] - fZBinf;
      z[2]        = z[1] + fZBinf;
      Int_t xbin0 = fBinX * 2 - 2;
      Int_t ybin0 = fBinY * 2 - 2;
      Int_t zbin0 = fBinZ * 2 - 2;

      CorrFitHDFunc3D* cf = static_cast<CorrFitHDFunc3D*>(fHDMaps);
      Double_t X[3];
      for (int i = 0; i < 3; i++) {
        X[0] = x[i];
        for (int j = 0; j < 3; j++) {
          X[1] = y[j];
          for (int k = 0; k < 3; k++) {
            X[2]            = z[k];
            Double_t n_temp = cf->GetDenominatorHD()[xbin0 + i][ybin0 + j][zbin0 + k];
            if (n_temp < 0) continue;
            Double_t Cf = CalculateCF(X, params);
            val += n_temp * Cf;
          }
        }
      }
      return val * cf->GetDenominatorSum()[fBinX][fBinY][fBinZ];
    }
    return CalculateCF(xx, params);
  }

  void CorrFit3DCF::GetTF1s(Bool_t makeNew, EDrawMode drawMode) {
    const Int_t nPar  = GetParametersNo();
    TString className = this->ClassName();
    if (makeNew) {
      switch (drawMode) {
        case EDrawMode::kNormal: {
          fDrawFunc.resize(3);
          fDrawFunc[0].first = new TF1("funcX", this, &CorrFit3DCF::GetFunX, fRange[0], fRange[1], nPar, className, "GetFunX");
          fDrawFunc[1].first = new TF1("funcY", this, &CorrFit3DCF::GetFunY, fRange[2], fRange[3], nPar, className, "GetFunY");
          fDrawFunc[2].first = new TF1("funcZ", this, &CorrFit3DCF::GetFunZ, fRange[4], fRange[5], nPar, className, "GetFunZ");
        } break;
        case EDrawMode::kDiagonal1: {
          fDrawFunc.resize(7);
          fDrawFunc[0].first = new TF1("funcX", this, &CorrFit3DCF::GetFunX, fRange[0], fRange[1], nPar, className, "GetFunX");
          fDrawFunc[1].first = new TF1("funcY", this, &CorrFit3DCF::GetFunY, fRange[2], fRange[3], nPar, className, "GetFunY");
          fDrawFunc[2].first = new TF1("funcZ", this, &CorrFit3DCF::GetFunZ, fRange[4], fRange[5], nPar, className, "GetFunZ");
          fDrawFunc[3].first =
            new TF1("funcXY++", this, &CorrFit3DCF::GetFunXYpp, fRange[0], fRange[1], nPar, className, "GetFunXYpp");
          fDrawFunc[4].first =
            new TF1("funcYZ++", this, &CorrFit3DCF::GetFunYZpp, fRange[2], fRange[3], nPar, className, "GetFunXZpp");
          fDrawFunc[5].first =
            new TF1("funcXZ++", this, &CorrFit3DCF::GetFunXZpp, fRange[4], fRange[5], nPar, className, "GetFunYZpp");
          fDrawFunc[6].first =
            new TF1("funcXYZ+++", this, &CorrFit3DCF::GetFunXYZppp, fRange[0], fRange[1], nPar, className, "GetFunXYZppp");

        } break;
        case EDrawMode::kDiagonal2: {
          fDrawFunc.resize(13);
          fDrawFunc[0].first = new TF1("funcX", this, &CorrFit3DCF::GetFunX, fRange[0], fRange[1], nPar, className, "GetFunX");
          fDrawFunc[1].first = new TF1("funcY", this, &CorrFit3DCF::GetFunY, fRange[2], fRange[3], nPar, className, "GetFunY");
          fDrawFunc[2].first = new TF1("funcZ", this, &CorrFit3DCF::GetFunZ, fRange[4], fRange[5], nPar, className, "GetFunZ");
          fDrawFunc[3].first =
            new TF1("funcXY++", this, &CorrFit3DCF::GetFunXYpp, fRange[0], fRange[1], nPar, className, "GetFunXYpp");
          fDrawFunc[4].first =
            new TF1("funcXY+-", this, &CorrFit3DCF::GetFunXYpm, fRange[0], fRange[1], nPar, className, "GetFunXYpm");
          fDrawFunc[5].first =
            new TF1("funcYZ++", this, &CorrFit3DCF::GetFunYZpp, fRange[2], fRange[3], nPar, className, "GetFunYZpp");
          fDrawFunc[6].first =
            new TF1("funcYZ+-", this, &CorrFit3DCF::GetFunYZpm, fRange[2], fRange[3], nPar, className, "GetFunYZpm");
          fDrawFunc[7].first =
            new TF1("funcXZ++", this, &CorrFit3DCF::GetFunXZpp, fRange[4], fRange[5], nPar, className, "GetFunXZpp");
          fDrawFunc[8].first =
            new TF1("funcXZ+-", this, &CorrFit3DCF::GetFunXZpm, fRange[4], fRange[5], nPar, className, "GetFunXZpm");
          fDrawFunc[9].first =
            new TF1("funcXYZ+++", this, &CorrFit3DCF::GetFunXYZppp, fRange[0], fRange[1], nPar, className, "GetFunXYZppp");
          fDrawFunc[10].first =
            new TF1("funcXYZ+-+", this, &CorrFit3DCF::GetFunXYZpmp, fRange[0], fRange[1], nPar, className, "GetFunXYZpmp");
          fDrawFunc[11].first =
            new TF1("funcXYZ+--", this, &CorrFit3DCF::GetFunXYZpmm, fRange[0], fRange[1], nPar, className, "GetFunXYZpmm");
          fDrawFunc[12].first =
            new TF1("funcXYZ++-", this, &CorrFit3DCF::GetFunXYZppm, fRange[0], fRange[1], nPar, className, "GetFunXYZppm");

        } break;
      }
      if (fDrawOptions.DrawCf()) {}
    }
    for (auto func_pad : fDrawFunc) {
      SetParametersToTF1(func_pad.first);
      func_pad.first->SetLineColor(GetLineColor());
      func_pad.first->SetLineStyle(GetLineStyle());
      func_pad.first->SetLineWidth(GetLineWidth());
    }
  }

  void CorrFit3DCF::GetTH1s(EDrawMode drawMode) {
    if (fDrawHistograms.size() > 0) return;
    switch (drawMode) {
      case EDrawMode::kNormal: {
        fDrawHistograms.resize(3);
        TH1D** arr = ((Femto3DCF*) fCF)->GetDiagProj("cf", kFALSE);
        for (int i = 0; i < 3; i++)
          fDrawHistograms[i] = arr[i];
        delete[] arr;
      } break;
      case EDrawMode::kDiagonal1: {
        fDrawHistograms.resize(7);
        TH1D** arr = ((Femto3DCF*) fCF)->GetDiagProj("diag1", kFALSE);
        for (int i = 0; i < 7; i++)
          fDrawHistograms[i] = arr[i];
        delete[] arr;
      } break;
      case EDrawMode::kDiagonal2: {
        fDrawHistograms.resize(13);
        TH1D** arr = ((Femto3DCF*) fCF)->GetDiagProj("diag2", kFALSE);
        for (int i = 0; i < 13; i++)
          fDrawHistograms[i] = arr[i];
        delete[] arr;
      } break;
    }
  }

  Double_t CorrFit3DCF::GetFunXYpp(Double_t* x, Double_t* params) const {
    fBinX = fDenominatorHistogram->GetXaxis()->FindBin(x[0]);
    fBinY = fDenominatorHistogram->GetYaxis()->FindBin(x[0]);
    fBinZ = fDenominatorHistogram->GetZaxis()->FindBin((*fZbins)[0]);
    if (fBinCalc == kExtrapolated) {  // calculate "step function"
      Double_t X[3] = {
        fDenominatorHistogram->GetXaxis()->GetBinCenter(fBinX),
        fDenominatorHistogram->GetYaxis()->GetBinCenter(fBinY),
        fDenominatorHistogram->GetZaxis()->GetBinCenter(fBinZ),
      };
      return EvalCF(X, params);
    }

    // simple make simple average operation
    // TODO take width into account
    Double_t tempX[3] = {x[0], x[0], (*fZbins)[0]};
    return CalculateCF(tempX, params);
  }

  Double_t CorrFit3DCF::GetFunXYpm(Double_t* x, Double_t* params) const {
    fBinX = fDenominatorHistogram->GetXaxis()->FindBin(x[0]);
    fBinY = fDenominatorHistogram->GetYaxis()->FindBin(x[0]);
    fBinZ = fDenominatorHistogram->GetZaxis()->FindBin((*fZbins)[0]);
    fBinY = fDenominatorHistogram->GetNbinsY() - fBinY + 1;
    if (fBinCalc == kExtrapolated) {  // calculate "step function"
      Double_t X[3] = {
        fDenominatorHistogram->GetXaxis()->GetBinCenter(fBinX),
        fDenominatorHistogram->GetYaxis()->GetBinCenter(fBinY),
        fDenominatorHistogram->GetZaxis()->GetBinCenter(fBinZ),
      };
      return EvalCF(X, params);
    }

    // simple make simple average operation
    // TODO take width into account
    Double_t sY       = 2.0 * fYAxisf - x[0];
    Double_t tempX[3] = {x[0], sY, (*fZbins)[0]};
    return CalculateCF(tempX, params);
  }

  Double_t CorrFit3DCF::GetFunXZpp(Double_t* x, Double_t* params) const {
    fBinX = fDenominatorHistogram->GetXaxis()->FindBin(x[0]);
    fBinY = fDenominatorHistogram->GetYaxis()->FindBin((*fYbins)[0]);
    fBinZ = fDenominatorHistogram->GetZaxis()->FindBin(x[0]);
    if (fBinCalc == kExtrapolated) {  // calculate "step function"
      Double_t X[3] = {
        fDenominatorHistogram->GetXaxis()->GetBinCenter(fBinX),
        fDenominatorHistogram->GetYaxis()->GetBinCenter(fBinY),
        fDenominatorHistogram->GetZaxis()->GetBinCenter(fBinZ),
      };
      return EvalCF(X, params);
    }

    // simple make simple average operation
    // TODO take width into account
    Double_t tempX[3] = {x[0], (*fYbins)[0], x[0]};
    return CalculateCF(tempX, params);
  }

  Double_t CorrFit3DCF::GetFunXZpm(Double_t* x, Double_t* params) const {
    fBinX = fDenominatorHistogram->GetXaxis()->FindBin(x[0]);
    fBinY = fDenominatorHistogram->GetYaxis()->FindBin((*fYbins)[0]);
    fBinZ = fDenominatorHistogram->GetZaxis()->FindBin(x[0]);
    fBinZ = fDenominatorHistogram->GetNbinsZ() - fBinZ + 1;
    if (fBinCalc == kExtrapolated) {  // calculate "step function"
      Double_t X[3] = {
        fDenominatorHistogram->GetXaxis()->GetBinCenter(fBinX),
        fDenominatorHistogram->GetYaxis()->GetBinCenter(fBinY),
        fDenominatorHistogram->GetZaxis()->GetBinCenter(fBinZ),
      };
      return EvalCF(X, params);
    }

    // simple make simple average operation
    // TODO take width into account
    Double_t sZ       = 2.0 * fZAxisf - x[0];
    Double_t tempX[3] = {x[0], (*fYbins)[0], sZ};
    return CalculateCF(tempX, params);
  }

  Double_t CorrFit3DCF::GetFunYZpp(Double_t* x, Double_t* params) const {
    fBinX = fDenominatorHistogram->GetXaxis()->FindBin((*fXbins)[0]);
    fBinY = fDenominatorHistogram->GetYaxis()->FindBin(x[0]);
    fBinZ = fDenominatorHistogram->GetZaxis()->FindBin(x[0]);
    if (fBinCalc == kExtrapolated) {  // calculate "step function"
      Double_t X[3] = {
        fDenominatorHistogram->GetXaxis()->GetBinCenter(fBinX),
        fDenominatorHistogram->GetYaxis()->GetBinCenter(fBinY),
        fDenominatorHistogram->GetZaxis()->GetBinCenter(fBinZ),
      };
      return EvalCF(X, params);
    }

    // simple make simple average operation
    // TODO take width into account
    Double_t tempX[3] = {(*fXbins)[0], x[0], x[0]};
    return CalculateCF(tempX, params);
  }

  Double_t CorrFit3DCF::GetFunYZpm(Double_t* x, Double_t* params) const {
    fBinX = fDenominatorHistogram->GetXaxis()->FindBin((*fXbins)[0]);
    fBinY = fDenominatorHistogram->GetYaxis()->FindBin(x[0]);
    fBinZ = fDenominatorHistogram->GetZaxis()->FindBin(x[0]);
    fBinZ = fDenominatorHistogram->GetNbinsZ() - fBinZ + 1;
    if (fBinCalc == kExtrapolated) {  // calculate "step function"
      Double_t X[3] = {
        fDenominatorHistogram->GetXaxis()->GetBinCenter(fBinX),
        fDenominatorHistogram->GetYaxis()->GetBinCenter(fBinY),
        fDenominatorHistogram->GetZaxis()->GetBinCenter(fBinZ),
      };
      return EvalCF(X, params);
    }

    // simple make simple average operation
    // TODO take width into account
    Double_t sZ       = 2.0 * fZAxisf - x[0];
    Double_t tempX[3] = {(*fXbins)[0], x[0], sZ};
    return CalculateCF(tempX, params);
  }

  Double_t CorrFit3DCF::GetFunXYZppp(Double_t* x, Double_t* params) const {
    fBinX = fDenominatorHistogram->GetXaxis()->FindBin(x[0]);
    fBinY = fDenominatorHistogram->GetYaxis()->FindBin(x[0]);
    fBinZ = fDenominatorHistogram->GetZaxis()->FindBin(x[0]);
    if (fBinCalc == kExtrapolated) {  // calculate "step function"
      Double_t X[3] = {
        fDenominatorHistogram->GetXaxis()->GetBinCenter(fBinX),
        fDenominatorHistogram->GetYaxis()->GetBinCenter(fBinY),
        fDenominatorHistogram->GetZaxis()->GetBinCenter(fBinZ),
      };
      return EvalCF(X, params);
    }

    // simple make simple average operation
    // TODO take width into account
    Double_t tempX[3] = {x[0], x[0], x[0]};
    return CalculateCF(tempX, params);
  }

  Double_t CorrFit3DCF::GetFunXYZpmp(Double_t* x, Double_t* params) const {
    fBinX = fDenominatorHistogram->GetXaxis()->FindBin(x[0]);
    fBinY = fDenominatorHistogram->GetYaxis()->FindBin(x[0]);
    fBinZ = fDenominatorHistogram->GetZaxis()->FindBin(x[0]);
    fBinY = fDenominatorHistogram->GetNbinsY() - fBinY + 1;
    if (fBinCalc == kExtrapolated) {  // calculate "step function"
      Double_t X[3] = {
        fDenominatorHistogram->GetXaxis()->GetBinCenter(fBinX),
        fDenominatorHistogram->GetYaxis()->GetBinCenter(fBinY),
        fDenominatorHistogram->GetZaxis()->GetBinCenter(fBinZ),
      };
      return EvalCF(X, params);
    }

    // simple make simple average operation
    // TODO take width into account
    Double_t sY       = 2.0 * fYAxisf - x[0];
    Double_t tempX[3] = {x[0], sY, x[0]};
    return CalculateCF(tempX, params);
  }

  Double_t CorrFit3DCF::GetFunXYZpmm(Double_t* x, Double_t* params) const {
    fBinX = fDenominatorHistogram->GetXaxis()->FindBin(x[0]);
    fBinY = fDenominatorHistogram->GetYaxis()->FindBin(x[0]);
    fBinZ = fDenominatorHistogram->GetZaxis()->FindBin(x[0]);
    fBinY = fDenominatorHistogram->GetNbinsY() - fBinY + 1;
    fBinZ = fDenominatorHistogram->GetNbinsZ() - fBinZ + 1;
    if (fBinCalc == kExtrapolated) {  // calculate "step function"
      Double_t X[3] = {
        fDenominatorHistogram->GetXaxis()->GetBinCenter(fBinX),
        fDenominatorHistogram->GetYaxis()->GetBinCenter(fBinY),
        fDenominatorHistogram->GetZaxis()->GetBinCenter(fBinZ),
      };
      return EvalCF(X, params);
    }

    // simple make simple average operation
    // TODO take width into account
    Double_t sY       = 2.0 * fYAxisf - x[0];
    Double_t sZ       = 2.0 * fZAxisf - x[0];
    Double_t tempX[3] = {x[0], sY, sZ};
    return CalculateCF(tempX, params);
  }

  Double_t CorrFit3DCF::GetFunXYZppm(Double_t* x, Double_t* params) const {
    fBinX = fDenominatorHistogram->GetXaxis()->FindBin(x[0]);
    fBinY = fDenominatorHistogram->GetYaxis()->FindBin(x[0]);
    fBinZ = fDenominatorHistogram->GetZaxis()->FindBin(x[0]);
    fBinZ = fDenominatorHistogram->GetNbinsZ() - fBinZ + 1;
    if (fBinCalc == kExtrapolated) {  // calculate "step function"
      Double_t X[3] = {
        fDenominatorHistogram->GetXaxis()->GetBinCenter(fBinX),
        fDenominatorHistogram->GetYaxis()->GetBinCenter(fBinY),
        fDenominatorHistogram->GetZaxis()->GetBinCenter(fBinZ),
      };
      return EvalCF(X, params);
    }

    // simple make simple average operation
    // TODO take width into account
    Double_t sZ       = 2.0 * fZAxisf - x[0];
    Double_t tempX[3] = {x[0], x[0], sZ};
    return CalculateCF(tempX, params);
  }

  void CorrFit3DCF::Calculatef(Double_t width) {
    TH3* h = (TH3*) ((DividedHisto1D*) fCF)->GetNum();

    Int_t nbins;
    Double_t min, max;
    Hal::Std::GetAxisPar(*h, nbins, min, max, "x");
    fXAxisf = 0.5 * (min + max);
    Hal::Std::GetAxisPar(*h, nbins, min, max, "y");
    fYAxisf = 0.5 * (min + max);
    Hal::Std::GetAxisPar(*h, nbins, min, max, "z");
    fZAxisf             = 0.5 * (min + max);
    const Femto3DCF* CF = static_cast<Femto3DCF*>(fCF);
    Int_t middle_x      = CF->GetNum()->GetXaxis()->FindBin(0.0);
    Int_t middle_y      = CF->GetNum()->GetYaxis()->FindBin(0.0);
    Int_t middle_z      = CF->GetNum()->GetZaxis()->FindBin(0.0);
    fXbins->Resize(1);
    fYbins->Resize(1);
    fZbins->Resize(1);
    (*fXbins)[0] = CF->GetNum()->GetXaxis()->GetBinCenter(middle_x);
    (*fYbins)[0] = CF->GetNum()->GetYaxis()->GetBinCenter(middle_y);
    (*fZbins)[0] = CF->GetNum()->GetZaxis()->GetBinCenter(middle_z);
    if (width == -1) {  // center+edges
      fXbins->Resize(3);
      fYbins->Resize(3);
      fZbins->Resize(3);
      (*fXbins)[0] = CF->GetNum()->GetXaxis()->GetBinLowEdge(middle_x);
      (*fYbins)[0] = CF->GetNum()->GetYaxis()->GetBinLowEdge(middle_y);
      (*fZbins)[0] = CF->GetNum()->GetZaxis()->GetBinLowEdge(middle_z);
      (*fXbins)[1] = CF->GetNum()->GetXaxis()->GetBinCenter(middle_x);
      (*fYbins)[1] = CF->GetNum()->GetYaxis()->GetBinCenter(middle_y);
      (*fZbins)[1] = CF->GetNum()->GetZaxis()->GetBinCenter(middle_z);
      (*fXbins)[2] = CF->GetNum()->GetXaxis()->GetBinUpEdge(middle_x);
      (*fYbins)[2] = CF->GetNum()->GetYaxis()->GetBinUpEdge(middle_y);
      (*fZbins)[2] = CF->GetNum()->GetZaxis()->GetBinUpEdge(middle_z);
      width        = 0;
    } else if (width != 0) {
      fXbins->Resize(2 * width + 1);
      fYbins->Resize(2 * width + 1);
      fZbins->Resize(2 * width + 1);
      for (int i = 0; i <= width * 2; i++) {
        (*fXbins)[i] = CF->GetNum()->GetXaxis()->GetBinCenter(middle_x - width + i);
        (*fYbins)[i] = CF->GetNum()->GetYaxis()->GetBinCenter(middle_y - width + i);
        (*fZbins)[i] = CF->GetNum()->GetZaxis()->GetBinCenter(middle_z - width + i);
      }
    }
  }

  void CorrFit3DCF::RecalculateSmoothFunction() const {
    CorrFitHDFunc3D* cf = static_cast<CorrFitHDFunc3D*>(fHDMaps);
    Double_t X[3];
    for (int a = 0; a < cf->GetBinsHDX().GetSize(); a++) {
      Int_t i                = cf->GetBinsHDX()[a];
      Int_t j                = cf->GetBinsHDY()[a];
      Int_t k                = cf->GetBinsHDZ()[a];
      fBinX                  = cf->HDBinToBin(i);
      fBinY                  = cf->HDBinToBin(j);
      fBinZ                  = cf->HDBinToBin(k);
      X[0]                   = cf->EvalHDX(i);
      X[1]                   = cf->EvalHDY(j);
      X[2]                   = cf->EvalHDZ(k);
      Double_t CF            = CalculateCF(X, fTempParamsEval);
      cf->CFMapHD()[i][j][k] = CF;
    }
  }

  void CorrFit3DCF::SetFittingMask(const CorrFitMask& map) {
    const CorrFitMask3D* mask = dynamic_cast<const CorrFitMask3D*>(&map);
    if (mask == nullptr) return;
    if (!mask->AreCompatible(fCF)) return;
    if (fMask) delete fMask;
    fMask        = (CorrFitMask*) mask->Clone();
    fOwnRangeMap = kTRUE;
  }

}  // namespace Hal
