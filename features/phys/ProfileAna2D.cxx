/*
 * ProfileAna2D.cxx
 *
 *  Created on: 9 gru 2024
 *      Author: daniel
 */

#include "ProfileAna2D.h"

#include <RtypesCore.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TH2.h>
#include <TLine.h>
#include <TMarker.h>
#include <TMath.h>
#include <TProfile.h>
#include <TROOT.h>

#include "ParameterSolver.h"
#include "Std.h"
#include "StdHist.h"


namespace Hal {

  ProfileAna2D::ProfileAna2D() {}

  ProfileAna2D::ProfileAna2D(TH2& h, Int_t poly) : fNParam(poly) {
    fHisto = (TH2*) h.Clone();
    fParamsAverage.resize(poly);
    fParamsSigmaLow.resize(poly);
    fParamsSigmaHigh.resize(poly);
    Int_t bins;
    Double_t low, high;
    Hal::Std::GetAxisPar(*fHisto, bins, low, high);
    fTotBinsX = bins;
    fTotMinX  = low;
    fTotMaxX  = high;
  }

  ProfileAna2D::ProfileAna2D(TH2& h, std::vector<Double_t> points) : ProfileAna2D(h, points.size()) { fPoints = points; }

  void ProfileAna2D::Analyze(TString opt) {
    if (fAnalyzed) return;
    fAnalyzed      = kTRUE;
    fAnalyzeOption = opt;
    if (!fCustomFit) {
      AnalyzePolyGaus();
    } else {
      AnalizeCustom();
    }
  }

  void ProfileAna2D::AnalyzeProj() {
    std::cout << "Not implemented yet" << std::endl;
    exit(0);
  }

  void ProfileAna2D::AnalyzePolyGaus() {
    std::cout << "---- " << ClassName() << " ----" << std::endl;
    std::cout << "Gauss Poly Analysis" << std::endl;
    fFittingPattern = "";
    for (int i = 0; i < fNParam; i++) {
      if (fFittingPattern.Length() == 0)
        fFittingPattern = fFittingPattern + Form("[%i]*pow(x,%i)", i, i);
      else
        fFittingPattern = fFittingPattern + Form("+[%i]*pow(x,%i)", i, i);
    }

    fFittingProjPattern = "[0] * exp(-0.5 * pow((x - [1]) / [2], 2))";
    InitializeAnalysis();

    if (Hal::Std::FindParam(fAnalyzeOption, UseFreeGaus())) {
      auto GrAv      = std::make_unique<TGraphErrors>();
      auto GrSiPlus  = std::make_unique<TGraphErrors>();
      auto GrSiMinus = std::make_unique<TGraphErrors>();
      int count      = 0;
      for (auto& slice : fData) {
        auto data             = ReCalculateFreeGauss(slice);
        fRmsLow[slice.fBin]   = data[fSigmaIdLow];
        fRmsHigh[slice.fBin]  = data[fSigmaIdHigh];
        fAverages[slice.fBin] = data[fMeanId];
        Double_t xGlob        = fValues[slice.fBin];
        int i                 = slice.fBin;
        Double_t sum          = data[EDataId::kEntries];
        GrAv->SetPoint(count, xGlob, fAverages[i]);
        GrAv->SetPointError(count, 0, fRmsHigh[i] / TMath::Sqrt(sum));
        GrSiMinus->SetPoint(count, xGlob, fRmsLow[i]);
        GrSiMinus->SetPointError(count, 0, fRmsLow[i] / TMath::Sqrt(2.0 * sum));
        GrSiPlus->SetPoint(count, xGlob, fRmsHigh[i]);
        GrSiPlus->SetPointError(count++, 0, fRmsHigh[i] / TMath::Sqrt(2.0 * sum));
      }

      AutoEstimParams(GrAv, GrSiMinus, GrSiPlus);
    } else if (Hal::Std::FindParam(fAnalyzeOption, UseRMSGaus())) {
      auto GrAv      = std::make_unique<TGraphErrors>();
      auto GrSiPlus  = std::make_unique<TGraphErrors>();
      auto GrSiMinus = std::make_unique<TGraphErrors>();
      int count      = 0;
      for (auto& slice : fData) {
        auto data      = ReCalculateGauss(slice);
        int i          = slice.fBin;
        fRmsLow[i]     = data[fSigmaIdLow];
        fRmsHigh[i]    = data[fSigmaIdHigh];
        fAverages[i]   = data[fMeanId];
        Double_t xGlob = fValues[i];
        Double_t sum   = data[EDataId::kEntries];
        GrAv->SetPoint(count, xGlob, fAverages[i]);
        GrAv->SetPointError(count, 0, fRmsHigh[i] / TMath::Sqrt(sum));
        GrSiMinus->SetPoint(count, xGlob, fRmsLow[i]);
        GrSiMinus->SetPointError(count, 0, fRmsLow[i] / TMath::Sqrt(2.0 * sum));
        GrSiPlus->SetPoint(count, xGlob, fRmsHigh[i]);
        GrSiPlus->SetPointError(count++, 0, fRmsHigh[i] / TMath::Sqrt(2.0 * sum));
      }
      AutoEstimParams(GrAv, GrSiMinus, GrSiPlus);
    }
  }

  Bool_t ProfileAna2D::Check(Double_t x, Double_t y, Double_t sigmaLow, Double_t sigmaHigh) const {
    Double_t expp    = fAverage->Eval(x);
    Double_t sigmaHi = (y - expp) / fSigmaHigh->Eval(x);
    Double_t sigmaLo = (expp - y) / fSigmaLow->Eval(x);
    if (sigmaLo < sigmaLow) return kFALSE;
    if (sigmaHi > sigmaHigh) return kFALSE;
    return kTRUE;
  }

  std::vector<Double_t> ProfileAna2D::Calculate(const std::vector<Double_t>& valuesX,
                                                const std::vector<Double_t>& valuesY) const {
    std::vector<Double_t> val(fMaxStatPars);
    Double_t sum    = 0;
    Double_t rms    = 0;
    Double_t ent    = 0;
    Double_t maxVal = 0;
    Double_t maxi   = 0;
    for (int i = 0; i < valuesX.size(); i++) {
      sum += valuesX[i] * valuesY[i];
      ent += valuesY[i];
      if (valuesY[i] > maxVal) {
        maxVal = valuesY[i];
        maxi   = valuesX[i];
      }
    }
    Double_t mean = sum / TMath::Max(1., ent);
    for (int i = 0; i < valuesX.size(); i++) {
      Double_t delta = valuesX[i] - mean;
      rms += valuesY[i] * delta * delta;
    }

    Double_t sigma                 = TMath::Sqrt(rms / TMath::Max(1.0, (double) ent));
    val[EDataId::kMean]            = mean;
    val[EDataId::kEntries]         = ent;
    val[EDataId::kMaxX]            = maxi;
    val[EDataId::kMaxY]            = maxVal;
    val[EDataId::kSigmaPlus]       = sigma;
    val[EDataId::kSigmaMinus]      = sigma;
    val[EDataId::kSum]             = sum;
    val[EDataId::kMeanError]       = sigma / TMath::Sqrt(ent);
    val[EDataId::kSigmaPlusError]  = sigma / TMath::Sqrt(2.0 * ent);
    val[EDataId::kSigmaMinusError] = sigma / TMath::Sqrt(2.0 * ent);
    return val;
  }

  void ProfileAna2D::GetRow(std::vector<Double_t>& valuesX,
                            std::vector<Double_t>& valuesY,
                            std::vector<Double_t>& errorY,
                            const Int_t row) const {
    valuesX.clear();
    valuesY.clear();
    errorY.clear();
    for (int j = 1; j <= fHisto->GetNbinsY(); j++) {
      double inc = fHisto->GetBinContent(row, j);
      valuesX.push_back(fHisto->GetYaxis()->GetBinCenter(j));
      valuesY.push_back(fHisto->GetBinContent(row, j));
      errorY.push_back(fHisto->GetBinError(row, j));
    }
  }

  void ProfileAna2D::Draw(Option_t* opt) {
    Int_t binsX, binsY;
    Double_t lowX, highX, lowY, highY;
    Hal::Std::GetAxisPar(*fHisto, binsX, lowX, highX, "x");
    TF1* average = new TF1(
      Hal::Std::GetUniqueName("polyfit"), this, &ProfileAna2D::EvaluateMean, lowX, highX, 0, this->ClassName(), "EvaluateMean");
    TF1* sigmaLo = new TF1(Hal::Std::GetUniqueName("sigmafitLo"),
                           this,
                           &ProfileAna2D::EvalNSigmaMinus,
                           lowX,
                           highX,
                           1,
                           this->ClassName(),
                           "EvalNSigmaMinus");
    TF1* sigmaHi = new TF1(Hal::Std::GetUniqueName("sigmafitHi"),
                           this,
                           &ProfileAna2D::EvalNSigmaPlus,
                           lowX,
                           highX,
                           1,
                           this->ClassName(),
                           "EvalNSigmaPlus");

    sigmaLo->SetLineStyle(7);
    sigmaHi->SetLineStyle(7);
    sigmaHi->FixParameter(0, 1);
    sigmaLo->FixParameter(0, 1);

    TString option = opt;
    if (Hal::Std::FindParam(option, "colz", kTRUE)) { fHisto->Draw("colz"); }
    Double_t sigma = 1;
    if (Hal::Std::FindParam(option, "sig2", kTRUE)) sigma = 2;
    if (Hal::Std::FindParam(option, "sig3", kTRUE)) sigma = 3;
    Bool_t subplots = false;
    if (Hal::Std::FindParam(option, "raw", kTRUE)) subplots = true;

    sigmaLo->FixParameter(0, -sigma);
    sigmaHi->FixParameter(0, sigma);
    average->SetLineColor(fColor);
    sigmaLo->SetLineColor(fColor);
    sigmaHi->SetLineColor(fColor);


    if (subplots) {
      TF1* rmsLo = new TF1(Hal::Std::GetUniqueName("rmsLo"),
                           this,
                           &ProfileAna2D::EvalRawRMSMinus,
                           lowX,
                           highX,
                           0,
                           this->ClassName(),
                           "EvalRawRMSMinus");
      TF1* rmsHi = new TF1(Hal::Std::GetUniqueName("rmsHi"),
                           this,
                           &ProfileAna2D::EvalRawRMSPlus,
                           lowX,
                           highX,
                           0,
                           this->ClassName(),
                           "EvalRawRMSPlus");
      TF1* avmax = new TF1(
        Hal::Std::GetUniqueName("avmax"), this, &ProfileAna2D::EvalRawMean, lowX, highX, 0, this->ClassName(), "EvalRawMean");
      rmsLo->SetLineColor(fColor2);
      rmsHi->SetLineColor(fColor2);
      avmax->SetLineColor(fColor2);
      rmsLo->SetLineStyle(7);
      rmsHi->SetLineStyle(7);
      int width = 5;
      rmsLo->SetLineWidth(width);
      rmsHi->SetLineWidth(width);
      avmax->SetLineWidth(width);
      avmax->Draw("SAME");
      rmsHi->Draw("SAME");
      rmsLo->Draw("SAME");
    }
    average->Draw("SAME");
    sigmaLo->Draw("SAME");
    sigmaHi->Draw("SAME");
  }

  ProfileAna2D::~ProfileAna2D() {
    if (fAverage) delete fAverage;
    if (fHisto) delete fHisto;
    if (fSigmaLow) delete fSigmaLow;
    if (fSigmaHigh) delete fSigmaHigh;
  }

  std::vector<Double_t> ProfileAna2D::InitEstimPoly(const std::unique_ptr<TGraphErrors>& gr) const {
    std::vector<Double_t> valuesX;
    std::vector<Double_t> valuesY;
    for (int i = 0; i < gr->GetN(); i++) {
      Double_t x, y;
      gr->GetPoint(i, x, y);
      valuesX.push_back(x);
      valuesY.push_back(y);
    }
    std::vector<Double_t> res, xI, yI;
    const Int_t N = valuesX.size();
    const Int_t m = fNParam;
    if (fPoints.size() == 0) {
      double step = static_cast<double>(N - 1) / (m - 1);
      for (int i = 0; i < m; ++i) {
        int index = static_cast<int>(i * step + 0.5);
        xI.push_back(valuesX[index]);
        yI.push_back(valuesY[index]);
      }
    } else {
      for (unsigned int i = 0; i < fPoints.size(); i++) {
        yI.push_back(gr->Eval(fPoints[i]));
        xI.push_back(fPoints[i]);
      }
    }
    return Hal::Std::LagrangeInterpol(xI, yI);
  }

  Double_t ProfileAna2D::EvalRawMean(Double_t* x, Double_t* p) const {
    double step = (fTotMaxX - fTotMinX) / fTotBinsX;
    int point   = (x[0] - fTotMinX) / step + 1;
    if (point >= fTotBinsX) { point = fTotBinsX - 1; }
    return fAverages[point];
  }

  Double_t ProfileAna2D::EvalRawRMSPlus(Double_t* x, Double_t* p) const {
    double step = (fTotMaxX - fTotMinX) / fTotBinsX;
    int point   = (x[0] - fTotMinX) / step + 1;
    if (point >= fTotBinsX) { point = fTotBinsX - 1; }
    return fAverages[point] + fRmsHigh[point];
  }

  Double_t ProfileAna2D::EvalRawRMSMinus(Double_t* x, Double_t* p) const {
    double step = (fTotMaxX - fTotMinX) / fTotBinsX;
    int point   = (x[0] - fTotMinX) / step + 1;
    if (point >= fTotBinsX) { point = fTotBinsX - 1; }
    return fAverages[point] - fRmsLow[point];
  }

  void ProfileAna2D::DrawSlice(Double_t val, TString opt) {
    TH1D* copy   = Hal::Std::GetProjection1D(fHisto, val, val, "y");
    Double_t max = Hal::Std::GetMaximum({copy});
    if (max == 0) max = 1;
    copy->Scale(1.0 / max);
    copy->GetYaxis()->SetTitle("Scaled entries");
    copy->SetTitle(Form("ProjX at %4.4f", val));
    copy->SetMaximum(1.1);
    copy->SetMinimum(0);
    copy->SetMarkerColor(fColor);
    copy->SetMarkerStyle(kFullCircle);
    copy->SetLineColor(fColor);
    copy->Draw("same");

    Double_t th1rms     = copy->GetRMS();
    Double_t th1mean    = copy->GetMean();
    const Double_t yrms = 0.606531;

    if (Hal::Std::FindParam(opt, "stat")) {
      TMarker* m = new TMarker(th1mean, yrms, 33);
      m->SetMarkerSize(2);
      m->SetMarkerColor(fColor2);
      m->Draw("SAME");
      TLine* l = new TLine(th1mean - th1rms, yrms, th1mean + th1rms, yrms);
      l->SetLineColor(fColor2);
      l->SetLineWidth(2);
      l->Draw("SAME");

      l = new TLine(th1mean - th1rms, yrms - 0.05, th1mean - th1rms, yrms + 0.05);
      l->SetLineColor(fColor2);
      l->SetLineWidth(2);
      l->Draw("SAME");
      l = new TLine(th1mean + th1rms, yrms - 0.05, th1mean + th1rms, yrms + 0.05);
      l->SetLineColor(fColor2);
      l->SetLineWidth(2);
      l->Draw("SAME");
    }


    Int_t bins;
    Double_t xmin, xmax;
    Hal::Std::GetAxisPar(*copy, bins, xmin, xmax, "x");

    TF1* gauss = new TF1("gauss", fFittingProjPattern, xmin, xmax);

    Double_t av      = fAverage->Eval(val);
    Double_t sigmaLo = fSigmaLow->Eval(val);
    Double_t sigmaHi = fSigmaHigh->Eval(val);
    double x         = val;
    gauss->SetParameters(1.0, av, sigmaHi);
    gauss->SetLineColor(fColor);
    gauss->SetNpx(1000);
    gauss->SetTitle("Normalized Gaussian; x; f(x)");
    gauss->Draw("same");
    auto drawLine = [&](Double_t valx, Int_t style = 1) {
      TLine* l = new TLine(valx, 0, valx, 1.1);
      l->SetLineStyle(style);
      l->SetLineColor(fColor);
      l->Draw("same");
    };

    if (Hal::Std::FindParam(opt, "av", kTRUE)) { drawLine(av); }
    if (Hal::Std::FindParam(opt, "sig1", kTRUE)) {
      drawLine(av - sigmaLo, 7);
      drawLine(av + sigmaHi, 7);
    }

    if (Hal::Std::FindParam(opt, "sig2", kTRUE)) {
      drawLine(av - 2.0 * sigmaLo, 7);
      drawLine(av + 2.0 * sigmaHi, 7);
    }
    if (Hal::Std::FindParam(opt, "sig3", kTRUE)) {
      drawLine(av - 3.0 * sigmaLo, 7);
      drawLine(av + 3.0 * sigmaHi, 7);
    }
  }

  void ProfileAna2D::RemoveOutliers(std::vector<Double_t>& valuesX,
                                    std::vector<Double_t>& valuesY,
                                    std::vector<Double_t>& errorY,
                                    Double_t av,
                                    Double_t rms) const {
    std::vector<Double_t> newX, newY, newYE;
    for (unsigned int i = 0; i < valuesX.size(); i++) {
      double y = valuesX[i];
      if (y < av - fOutLiersCut * rms) continue;
      if (y > av + fOutLiersCut * rms) continue;
      newX.push_back(valuesX[i]);
      newY.push_back(valuesY[i]);
      newYE.push_back(errorY[i]);
    }
    valuesX = newX;
    valuesY = newY;
    errorY  = newYE;
  }

  void ProfileAna2D::InitializeAnalysis() {
    Int_t binsX, binsY;
    Double_t lowX, highX;
    Hal::Std::GetAxisPar(*fHisto, binsX, lowX, highX, "x");
    Hal::Std::GetAxisPar(*fHisto, binsY, fYMin, fYMax, "y");
    if (fAverage) {
      delete fAverage;
      delete fSigmaLow;
      delete fSigmaHigh;
    }
    fAverage = new TF1(Hal::Std::GetUniqueName("polyfit"), fFittingPattern, fTotMinX, fTotMaxX);
    if (fFreeSigma) fFittingSigmaPattern = fFittingPattern;
    fSigmaHigh = new TF1(Hal::Std::GetUniqueName("polyfitsigma"), fFittingSigmaPattern, fTotMinX, fTotMaxX);
    fSigmaLow  = new TF1(Hal::Std::GetUniqueName("polyfitsigma"), fFittingSigmaPattern, fTotMinX, fTotMaxX);
    fData.clear();
    int count      = 0;
    Double_t min   = lowX;
    Double_t max   = highX;
    Int_t startBin = 1;
    Int_t endBin   = binsX;
    if (fXMin != fXMax) {
      startBin = fHisto->GetXaxis()->FindBin(fXMin);
      endBin   = fHisto->GetXaxis()->FindBin(fXMax);
    } else {
      fXMin = lowX;
      fXMax = highX;
    }
    fAverages.resize(fHisto->GetXaxis()->GetNbins() + 2);
    fRmsLow.resize(fHisto->GetXaxis()->GetNbins() + 2);
    fRmsHigh.resize(fHisto->GetXaxis()->GetNbins() + 2);
    fValues.resize(fHisto->GetXaxis()->GetNbins() + 2);
    for (int i = 0; i < fHisto->GetXaxis()->GetNbins() + 2; i++) {
      fAverages[i] = 0;
      fRmsLow[i] = fRmsHigh[i] = -1;
      fValues[i]               = fHisto->GetXaxis()->GetBinCenter(i);
    }
    for (int i = startBin; i <= endBin; i++) {
      std::vector<Double_t> valuesX, valuesY, errorY;
      GetRow(valuesX, valuesY, errorY, i);
      auto data = Calculate(valuesX, valuesY);
      if (fOutLiersCut > 0) RemoveOutliers(valuesX, valuesY, errorY, data[EDataId::kMean], data[EDataId::kSigmaPlus]);
      Double_t sum = 0;
      for (auto x : valuesY)
        sum += x;
      if (fEntriesThreshold > sum) continue;
      if (fBinsThreshold > valuesX.size()) continue;
      SliceData slice;
      slice.fX   = valuesX;
      slice.fY   = valuesY;
      slice.fYe  = errorY;
      slice.fBin = i;
      fData.push_back(slice);
    }
  }

  void ProfileAna2D::AnalizeCustom() {
    std::cout << "---- " << ClassName() << " ----" << std::endl;
    std::cout << "Custom Analysis" << std::endl;
    InitializeAnalysis();

    auto GrAv      = std::make_unique<TGraphErrors>();
    auto GrSiPlus  = std::make_unique<TGraphErrors>();
    auto GrSiMinus = std::make_unique<TGraphErrors>();
    int count      = 0;
    for (auto& slice : fData) {
      auto data      = ReCalculateFree(slice);
      int i          = slice.fBin;
      fRmsLow[i]     = data[fSigmaIdLow];
      fRmsHigh[i]    = data[fSigmaIdHigh];
      fAverages[i]   = data[fMeanId];
      Double_t xGlob = fValues[i];
      Double_t sum   = data[EDataId::kEntries];
      GrAv->SetPoint(count, xGlob, fAverages[i]);
      GrAv->SetPointError(count, 0, fRmsHigh[i] / TMath::Sqrt(sum));
      GrSiMinus->SetPoint(count, xGlob, fRmsLow[i]);
      GrSiMinus->SetPointError(count, 0, fRmsLow[i] / TMath::Sqrt(2.0 * sum));
      GrSiPlus->SetPoint(count, xGlob, fRmsHigh[i]);
      GrSiPlus->SetPointError(count++, 0, fRmsHigh[i] / TMath::Sqrt(2.0 * sum));
    }
    AutoEstimParams(GrAv, GrSiMinus, GrSiPlus);
  }

  void ProfileAna2D::SetSigmaId(Int_t idA, Int_t idB) {
    fSigmaIdLow  = idA;
    fSigmaIdHigh = idB;
  }

  void ProfileAna2D::AnalyzePolyGausFitSlice() {}

  std::vector<Double_t> ProfileAna2D::ReCalculateGauss(const SliceData& data) const { return Calculate(data.fX, data.fY); }

  std::vector<Double_t> ProfileAna2D::ReCalculateFree(const SliceData& data) const {

    auto val   = ReCalculateGauss(data);
    TH1D* copy = Hal::Std::GetProjection1D(fHisto, fValues[data.fBin], fValues[data.fBin], "y");
    copy->Reset();
    for (int i = 0; i < data.fX.size(); i++) {
      int bin = copy->GetXaxis()->FindBin(data.fX[i]);
      copy->SetBinContent(bin, data.fY[i]);
      copy->SetBinError(bin, data.fYe[i]);
    }
    Int_t bins;
    Double_t xmin, xmax;
    Hal::Std::GetAxisPar(*copy, bins, xmin, xmax, "x");
    TF1* f         = new TF1("tempfit", fFittingProjPattern, xmin, xmax);
    double maximum = Hal::Std::GetMaximum({copy});
    double mean    = copy->GetMean();
    double rms     = copy->GetRMS();
    f->SetParameter(0, maximum);
    f->SetParameter(1, mean);
    f->SetParameter(2, rms);
    f->SetParLimits(0, maximum * 0.5, maximum * 1.5);
    f->SetParLimits(1, mean * 0.5, mean * 1.5);
    f->SetParLimits(2, rms * 0.5, rms * 1.5);

    copy->Fit(f, "WMRQ");

    val[EDataId::kMean]            = f->GetParameter(1);
    val[EDataId::kMeanError]       = f->GetParError(1);
    val[EDataId::kMaxY]            = f->GetParameter(0);
    val[EDataId::kSigmaMinus]      = f->GetParameter(2);
    val[EDataId::kSigmaPlus]       = f->GetParameter(2);
    val[EDataId::kSigmaMinusError] = f->GetParError(2);
    val[EDataId::kSigmaPlusError]  = f->GetParError(2);
    delete f;
    delete copy;
    return val;
  }

  std::vector<Double_t> ProfileAna2D::ReCalculateFreeGauss(const SliceData& data) const {

    std::vector<Double_t> val(fMaxStatPars);
    Double_t sum    = 0;
    Double_t rms    = 0;
    Double_t ent    = 0;
    Double_t maxVal = 0;
    Double_t maxi   = 0;
    for (int i = 0; i < data.fX.size(); i++) {
      sum += data.fX[i] * data.fY[i];
      ent += data.fY[i];
      if (data.fY[i] > maxVal) {
        maxVal = data.fY[i];
        maxi   = data.fX[i];
      }
    }
    Double_t mean = sum / TMath::Max(1., ent);
    for (int i = 0; i < data.fX.size(); i++) {
      Double_t delta = data.fX[i] - mean;
      rms += data.fY[i] * delta * delta;
    }

    Double_t sigma            = TMath::Sqrt(rms / TMath::Max(1.0, (double) ent));
    val[EDataId::kMean]       = mean;
    val[EDataId::kEntries]    = ent;
    val[EDataId::kMaxX]       = maxi;
    val[EDataId::kMaxY]       = maxVal;
    val[EDataId::kSigmaPlus]  = sigma;
    val[EDataId::kSigmaMinus] = sigma;
    val[EDataId::kSum]        = sum;


    TF1* f           = new TF1("funcgauss", fFittingProjPattern, fYMin, fYMax);
    TGraphErrors* gr = new TGraphErrors();
    for (unsigned int i = 0; i < data.fX.size(); i++) {
      gr->SetPoint(i, data.fX[i], data.fY[i]);
      gr->SetPointError(i, 0, data.fY[i]);
    }
    f->SetParameter(0, maxVal);
    f->SetParLimits(0, maxVal * 0.9, maxVal * 1.1);
    f->SetParameter(1, mean);
    f->SetParameter(2, sigma);
    gr->Fit(f, "Q");


    val[EDataId::kMean]            = f->GetParameter(1);
    val[EDataId::kMeanError]       = f->GetParError(1);
    val[EDataId::kSigmaPlus]       = f->GetParameter(2);
    val[EDataId::kSigmaMinus]      = f->GetParameter(2);
    val[EDataId::kSigmaPlusError]  = f->GetParError(2);
    val[EDataId::kSigmaMinusError] = f->GetParError(2);
    delete f;
    delete gr;

    return val;
  }

  Double_t ProfileAna2D::EvalSigmaPlus(Double_t* x, Double_t* p) const { return fSigmaHigh->Eval(*x); }

  Double_t ProfileAna2D::EvalSigmaMinus(Double_t* x, Double_t* p) const { return fSigmaLow->Eval(*x); }

  Double_t ProfileAna2D::EvaluateMean(Double_t* x, Double_t* p) const {
    Double_t val = fAverage->Eval(*x);
    if (TMath::IsNaN(val)) return 0;
    return val;
  }

  std::vector<Double_t>
  ProfileAna2D::InitEstimFunc(const std::unique_ptr<TGraphErrors>& gr, TF1* f, const std::vector<Double_t>& points) const {
    std::vector<Double_t> valuesX;
    std::vector<Double_t> valuesY;
    for (int i = 0; i < gr->GetN(); i++) {
      Double_t x, y;
      gr->GetPoint(i, x, y);
      valuesX.push_back(x);
      valuesY.push_back(y);
    }
    std::vector<Double_t> res, xI, yI;
    const Int_t N = valuesX.size();
    const Int_t m = fNParam;
    if (points.size() == 0) {
      double step = static_cast<double>(N - 1) / (m - 1);
      for (int i = 0; i < m; ++i) {
        int index = static_cast<int>(i * step + 0.5);
        xI.push_back(valuesX[index]);
        yI.push_back(valuesY[index]);
      }
    } else {
      for (unsigned int i = 0; i < points.size(); i++) {
        yI.push_back(gr->Eval(points[i]));
        xI.push_back(points[i]);
      }
    }
    auto solver = Hal::ParameterSolver(xI, yI, f);
    if (fStartParamsMain.size()) {
      auto solution = solver.Solve(fStartParamsMain, "print");
      solver.ReleaseFunc();  // to not delete TF1
      return solution;
    } else {
      auto solution = solver.Solve();
      solver.ReleaseFunc();  // to not delete TF1
      return solution;
    }
  }

  void ProfileAna2D::AutoEstimParams(const std::unique_ptr<TGraphErrors>& av,
                                     const std::unique_ptr<TGraphErrors>& sigMinus,
                                     const std::unique_ptr<TGraphErrors>& sigPlus) {


    if (fFreeSigma) {
      auto vecAv      = InitEstimFunc(av, fAverage, fPoints);
      auto vecSiMinus = InitEstimFunc(sigMinus, fSigmaLow, fPoints);
      auto vecSiPlus  = InitEstimFunc(sigPlus, fSigmaHigh, fPoints);
      for (int i = 0; i < fNParam; i++) {
        fAverage->SetParameter(i, vecAv[i]);
        fSigmaLow->SetParameter(i, vecSiMinus[i]);
        fSigmaHigh->SetParameter(i, vecSiPlus[i]);
      }
      if (!Hal::Std::FindParam(fAnalyzeOption, UseFixed())) {
        av->Fit(fAverage, "Q");
        sigPlus->Fit(fSigmaHigh, "Q");
        sigMinus->Fit(fSigmaLow, "Q");
      }
    } else {
      /** finish average calculation first - we need them for sigmas ! **/
      auto vecAv = InitEstimFunc(av, fAverage, fPoints);
      if (fStartParamsMain.size() == fNParam) { vecAv = fStartParamsMain; }
      for (int i = 0; i < fNParam; i++) {
        fAverage->SetParameter(i, vecAv[i]);
      }
      if (!Hal::Std::FindParam(fAnalyzeOption, UseFixed())) av->Fit(fAverage, "R");
      for (int i = 0; i < fNParam; i++)
        fParamsAverage[i] = fAverage->GetParameter(i);
      auto estimHigh = InitEstimFunc(sigPlus, fSigmaHigh, fPointsSigma);
      auto estimLow  = InitEstimFunc(sigMinus, fSigmaLow, fPointsSigma);
      for (int i = 0; i < sigPlus->GetN(); i++) {
        fSigmaHigh->SetParameter(i, estimHigh[i]);
        fSigmaLow->SetParameter(i, estimLow[i]);
      }
      if (!Hal::Std::FindParam(fAnalyzeOption, UseFixed())) {
        sigPlus->Fit(fSigmaHigh, "Q");
        sigMinus->Fit(fSigmaLow, "Q");
      }
    }
    for (int i = 0; i < fNParam; i++)
      fParamsAverage[i] = fAverage->GetParameter(i);
    for (int i = 0; i < fSigmaLow->GetNpar(); i++) {
      fParamsSigmaLow[i]  = fSigmaLow->GetParameter(i);
      fParamsSigmaHigh[i] = fSigmaHigh->GetParameter(i);
    }
  }

  Double_t ProfileAna2D::EvalNSigmaPlus(Double_t* x, Double_t* p) const {
    Double_t val  = fAverage->Eval(*x);
    Double_t val2 = fSigmaHigh->Eval(*x);
    if (TMath::IsNaN(val + val2)) return 0;
    return val + p[0] * val2;
  }

  Double_t ProfileAna2D::EvalNSigmaMinus(Double_t* x, Double_t* p) const {
    Double_t val  = fAverage->Eval(*x);
    Double_t val2 = fSigmaLow->Eval(*x);
    if (TMath::IsNaN(val + val2)) return 0;
    return val + p[0] * val2;
  }

  Double_t ProfileAna2D::GetFittedMean(Double_t x) const { return fAverage->Eval(x); }

  Double_t ProfileAna2D::GetFittedSigmaLo(Double_t x) const { return fSigmaLow->Eval(x); }


  Double_t ProfileAna2D::GetFittedSigmaHi(Double_t x) const { return fSigmaHigh->Eval(x); }

} /* namespace Hal */
