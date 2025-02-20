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
#include <TMath.h>
#include <TProfile.h>
#include <TROOT.h>


#include "Std.h"
#include "StdHist.h"


namespace Hal {

  ProfileAna2D::ProfileAna2D() {}

  ProfileAna2D::ProfileAna2D(TH2& h, Int_t poly) : fNParam(poly) {
    fHisto = (TH2*) h.Clone();
    fPolyAverage.resize(poly);
    fPolySigma.resize(poly);
    Int_t bins;
    Double_t low, high;
    Hal::Std::GetAxisPar(*fHisto, bins, low, high);
    fAverage = new TF1(Hal::Std::GetUniqueName("polyfit"),
                       this,
                       &ProfileAna2D::EvaluateAverage,
                       low,
                       high,
                       poly,
                       this->ClassName(),
                       "EvaluateAverage");
    fSigma   = new TF1(Hal::Std::GetUniqueName("polyfitsigma"),
                     this,
                     &ProfileAna2D::EvaluateSigma,
                     low,
                     high,
                     poly,
                     this->ClassName(),
                     "EvaluateSigma");
  }

  void ProfileAna2D::Analyze(EAnaType type) {
    if (fAnalyzed) return;
    fAnalyzed = kTRUE;
    /*switch (type) {
      case EAnaType::kRootProjAv: AnalyzeProj(); break;
      case EAnaType::kCustomProj: AnalyzeCustom(); break;
    }*/
    if (type == EAnaType::kCustomProjMax) fUseMax = kTRUE;
    AnalyzeCustom();
  }

  void ProfileAna2D::AnalyzeProj() {
    std::cout << "Not implemented yet" << std::endl;
    exit(0);
  }

  void ProfileAna2D::AnalyzeCustom() {
    Int_t binsX, binsY;
    Double_t lowX, highX, lowY, highY;
    Hal::Std::GetAxisPar(*fHisto, binsX, lowX, highX, "x");
    Hal::Std::GetAxisPar(*fHisto, binsY, lowY, highY, "y");

    // TF1* gaus = new TF1("gx", "gaus(0)", lowY, highY); //TODO build method that base on fitting
    TGraphErrors* GrAv = new TGraphErrors();
    TGraphErrors* GrSi = new TGraphErrors();
    int count          = 0;
    Double_t min       = lowX;
    Double_t max       = highX;
    Int_t startBin     = 1;
    Int_t endBin       = binsX;
    if (fXMin != fXMax) {
      startBin = fHisto->GetXaxis()->FindBin(fXMin);
      endBin   = fHisto->GetXaxis()->FindBin(fXMax);
    } else {
      fXMin = lowX;
      fXMax = highX;
    }
    for (int i = startBin; i <= endBin; i++) {
      std::vector<Double_t> valuesX, valuesY;
      Double_t mean, sigma, maxi;
      PrepareRow(valuesX, valuesY, mean, sigma, maxi, i);
      /*Double_t maxim = 0;
       for (auto x : yVal2)
        maxim = TMath::valuesY(x, maxim);
      Double_t normMax = maxim / TMath::Sqrt(sigma * TMath::Sqrt2());

       gaus->SetParLimits(0, normMax * 0.5, normMax * 1.5);  // norm
       gaus->SetParLimits(1, mean * 0.5, mean * 1.5);        // mean
       gaus->SetParLimits(2, sigma * 0.5, sigma * 1.5);

      TH1D *tt = new TH1D("temp","temp",binsY,lowY,highY);
      for(int j=1;j<=binsY;j++){
          tt->SetBinContent(j,fHisto->GetBinContent(i,j));
          tt->SetBinError(j,fHisto->GetBinError(i,j));
      }
      tt->Fit(gaus,"Q");
      */
      Double_t sum = 0;
      for (auto x : valuesY) {
        sum += x;
      }
      if (sum > fRowEntriesCut) {
        if (fUseMax) {
          GrAv->SetPoint(count, fHisto->GetXaxis()->GetBinCenter(i), maxi);
          GrAv->SetPointError(count, 0, sigma / TMath::Sqrt(sum));
        } else {
          GrAv->SetPoint(count, fHisto->GetXaxis()->GetBinCenter(i), mean);
          GrAv->SetPointError(count, 0, sigma / TMath::Sqrt(sum));
        }

        GrSi->SetPoint(count, fHisto->GetXaxis()->GetBinCenter(i), sigma);
        GrSi->SetPointError(count++, 0, sigma / TMath::Sqrt(2.0 * sum));
      }
    }

    auto vecAv = InitEstim(GrAv);
    auto vecSi = InitEstim(GrSi);

    for (int i = 0; i < vecAv.size(); i++) {
      fAverage->SetParameter(i, vecAv[i]);
      fSigma->SetParameter(i, vecSi[i]);
    }

    GrAv->Fit(fAverage, "Q");
    GrSi->Fit(fSigma, "Q");

    for (int i = 0; i < fNParam; i++) {
      fPolyAverage[i] = fAverage->GetParameter(i);
      fPolySigma[i]   = fSigma->GetParameter(i);
    }
    delete GrAv;
    delete GrSi;
    // delete gaus;
  }

  Bool_t ProfileAna2D::Check(Double_t x, Double_t y, Double_t sigmaLow, Double_t sigmaHi) const {
    Double_t expp  = fAverage->Eval(x);
    Double_t sigma = (y - expp) / fSigma->Eval(x);
    double sum     = 0;
    for (int i = 0; i < fNParam; i++) {
      sum += fPolySigma[i] * TMath::Power(x, i);
    }
    if (sigma < sigmaLow) return kFALSE;
    if (sigma > sigmaHi) return kFALSE;
    return kTRUE;
  }

  Double_t ProfileAna2D::EvaluateSigma(Double_t* x, Double_t* p) const {
    Double_t sum = 0;
    for (int i = 0; i < fNParam; i++) {
      sum += p[i] * TMath::Power(x[0], i);
    }
    return sum;
  }

  Double_t ProfileAna2D::EvaluateAverage(Double_t* x, Double_t* p) const {
    Double_t sum = 0;
    for (int i = 0; i < fNParam; i++) {
      sum += p[i] * TMath::Power(x[0], i);
    }
    return sum;
  }

  void ProfileAna2D::Calculate(const std::vector<Double_t>& valuesX,
                               const std::vector<Double_t>& valuesY,
                               Double_t& mean,
                               Double_t& sigma,
                               Double_t& maxi) const {
    Double_t sum    = 0;
    Double_t rms    = 0;
    Double_t ent    = 0;
    Double_t maxVal = 0;
    for (int i = 0; i < valuesX.size(); i++) {
      sum += valuesX[i] * valuesY[i];
      ent += valuesY[i];
      if (valuesY[i] > maxVal) {
        maxVal = valuesY[i];
        maxi   = valuesX[i];
      }
    }
    mean = sum / TMath::Max(1., ent);
    for (int i = 0; i < valuesX.size(); i++) {
      Double_t delta = valuesX[i] - mean;
      rms += valuesY[i] * delta * delta;
    }

    sigma = TMath::Sqrt(rms / TMath::Max(1.0, (double) ent));
  }

  void ProfileAna2D::PrepareRow(std::vector<Double_t>& valuesX,
                                std::vector<Double_t>& valuesY,
                                Double_t& mean,
                                Double_t& sigma,
                                Double_t& maxi,
                                const Int_t row) const {
    Double_t sum    = 0;
    Double_t counts = 0;
    std::vector<Double_t> xVal, yVal;
    for (int j = 1; j <= fHisto->GetNbinsY(); j++) {
      double inc = fHisto->GetBinContent(row, j);
      xVal.push_back(fHisto->GetYaxis()->GetBinCenter(j));
      yVal.push_back(fHisto->GetBinContent(row, j));
    }
    Calculate(xVal, yVal, mean, sigma, maxi);
    std::vector<Double_t> xVal2, yVal2;
    if (fRmsCut > 0) {
      for (int j = 0; j < xVal.size(); j++) {
        double x    = xVal[j];
        double y    = yVal[j];
        Double_t dx = TMath::Abs(x - mean) / sigma;
        if (dx <= fRmsCut) {
          xVal2.push_back(x);
          yVal2.push_back(y);
        }
      }
    } else {
      xVal2 = xVal;
      yVal2 = yVal;
    }
    Calculate(xVal2, yVal2, mean, sigma, maxi);
    valuesX = xVal2;
    valuesY = yVal2;
  }

  void ProfileAna2D::Draw(Option_t* opt) {
    Int_t binsX, binsY;
    Double_t lowX, highX, lowY, highY;
    Hal::Std::GetAxisPar(*fHisto, binsX, lowX, highX, "x");

    TF1* average = new TF1(
      Hal::Std::GetUniqueName("polyfit"), this, &ProfileAna2D::EvalDrawAv, lowX, highX, 0, this->ClassName(), "EvalDrawAv");


    TF1* sigmaLo = new TF1(Hal::Std::GetUniqueName("sigmafitLo"),
                           this,
                           &ProfileAna2D::EvalDrawAvShift,
                           lowX,
                           highX,
                           1,
                           this->ClassName(),
                           "EvalDrawAvShift");
    TF1* sigmaHi = new TF1(Hal::Std::GetUniqueName("sigmafitHi"),
                           this,
                           &ProfileAna2D::EvalDrawAvShift,
                           lowX,
                           highX,
                           1,
                           this->ClassName(),
                           "EvalDrawAvShift");

    sigmaLo->SetLineStyle(7);
    sigmaHi->SetLineStyle(7);


    TString option = opt;
    if (Hal::Std::FindParam(option, "colz", kTRUE)) { fHisto->Draw("colz"); }
    Double_t sigma = 1;
    if (Hal::Std::FindParam(option, "sig2", kTRUE)) sigma = 2;
    if (Hal::Std::FindParam(option, "sig3", kTRUE)) sigma = 3;


    sigmaLo->FixParameter(0, -sigma);
    sigmaHi->FixParameter(0, sigma);
    average->SetLineColor(fColor);
    sigmaLo->SetLineColor(fColor);
    sigmaHi->SetLineColor(fColor);
    average->Draw("SAME");
    sigmaLo->Draw("SAME");
    sigmaHi->Draw("SAME");
  }

  Double_t ProfileAna2D::EvalDrawAv(Double_t* x, Double_t* p) const {
    Double_t sum = 0;
    for (int i = 0; i < fNParam; i++) {
      sum += fPolyAverage[i] * TMath::Power(x[0], i);
    }
    return sum;
  }

  Double_t ProfileAna2D::EvalDrawAvShift(Double_t* x, Double_t* p) const {
    Double_t av  = EvalDrawAv(x, p);
    Double_t sum = 0;
    for (int i = 0; i < fNParam; i++) {
      sum += fPolySigma[i] * TMath::Power(x[0], i);
    }
    return av + p[0] * sum;
  }


  ProfileAna2D::~ProfileAna2D() {
    if (fAverage) delete fAverage;
    if (fSigma) delete fSigma;
    if (fHisto) delete fHisto;
  }

  std::vector<Double_t> ProfileAna2D::InitEstim(TGraphErrors* gr) const {
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
    double step   = static_cast<double>(N - 1) / (m - 1);
    for (int i = 0; i < m; ++i) {
      int index = static_cast<int>(i * step + 0.5);
      xI.push_back(valuesX[index]);
      yI.push_back(valuesY[index]);
    }
    return Hal::Std::LagrangeInterpol(xI, yI);
  }

  Double_t ProfileAna2D::Eval2DimGauss(Double_t* x, Double_t* p) const {
    for (int i = 0; i < fNParam; i++) {
      fAverage->SetParameter(i, p[1 + i]);
      fSigma->SetParameter(i, p[1 + i + fNParam]);
    }

    Double_t sigma     = fSigma->Eval(x[0]);
    Double_t mean      = fAverage->Eval(x[0]);
    Double_t full_func = p[0] * TMath::Exp(-TMath::Power(x[1] - mean, 2.0) / (2.0 * sigma * sigma));
    return full_func;
  }

} /* namespace Hal */
