/*
 * CorrHisto.cxx
 *
 *  Created on: 11 kwi 2024
 *      Author: daniel
 */

#include "CorrelationHisto.h"

#include "Cout.h"
#include "Std.h"

#include <TAxis.h>
#include <TCanvas.h>
#include <TH2D.h>
#include <TVirtualPad.h>

#include <iostream>

namespace Hal {
  CorrelationHisto::CorrelationHisto(Int_t nParams, Int_t nTest) :
    fNParams(nParams), fNTest(TMath::Abs(nTest)), fAutoFill(kTRUE) {}

  void CorrelationHisto::ConfigParam(Int_t par, Int_t bins, Double_t min, Double_t max, TString name) {
    fAxesConf.insert(fAxesConf.begin() + par, Hal::HistogramAxisConf(name, bins, min, max));
    fAutoFill = kFALSE;
    fNTest    = -1;
  }

  CorrelationHisto::CorrelationHisto(std::initializer_list<std::initializer_list<Double_t>> vals, Int_t nTest) {
    for (auto i : vals) {
      fNParams++;
      auto vec = Hal::Std::GetVector(i);
      fAxesConf.push_back(Hal::HistogramAxisConf(Form("par %i", fNParams - 1), vec[0], vec[1], vec[2]));
    }
    fNTest    = -1;
    fAutoFill = kFALSE;
  }

  void CorrelationHisto::Fill(std::vector<Double_t> params) {
    if (params.size() != fNParams) return;
    //    HalCoutDebug();
    if (fAutoFill) {
      //      HalCoutDebug();
      if (fTestValues.size() < fNTest) {

        fTestValues.push_back(params);
        return;
      } else {
        fAutoFill = kFALSE;
        AutoInitialize();
      }
    } else {
      if (fNTest == -1) {
        Initialize();
        fNTest = 0;
      }
    }
    //    HalCoutDebug();
    int count = 0;
    for (int i = 0; i < fNParams; i++) {
      double x = params[i];
      for (int j = i; j < fNParams; j++) {
        double y = params[j];
        fHistograms[count++]->Fill(x, y);
      }
    }
  }

  void CorrelationHisto::Initialize() {
    fNHistograms = (fNParams + 1) * fNParams / 2;
    fHistograms  = new TH2D*[fNHistograms];
    int count    = 0;
    for (int i = 0; i < fNParams; i++) {
      for (int j = i; j < fNParams; j++) {
        count++;
        TString title          = Form("SB_%i", count);
        fHistograms[count - 1] = new TH2D(title,
                                          title,
                                          fAxesConf[i].GetNBins(),
                                          fAxesConf[i].GetMin(),
                                          fAxesConf[i].GetMax(),
                                          fAxesConf[j].GetNBins(),
                                          fAxesConf[j].GetMin(),
                                          fAxesConf[j].GetMax());
        fHistograms[count - 1]->GetXaxis()->SetTitle(fAxesConf[i].GetName());
        fHistograms[count - 1]->GetYaxis()->SetTitle(fAxesConf[j].GetName());
        fHistograms[count - 1]->GetXaxis()->CenterTitle();
        fHistograms[count - 1]->GetYaxis()->CenterTitle();
      }
    }
  }

  void CorrelationHisto::AutoInitialize() {
    int count = 0;
    for (int par = 0; par < fNParams; par++) {
      double mini = 1E+9, maxi = 1E-9, av, sum = 0;
      double n = fNTest;
      for (unsigned int ent = 0; ent < fNTest; ent++) {
        double j = fTestValues[ent][par];
        sum += j;
        mini = TMath::Min(mini, j);
        maxi = TMath::Max(maxi, j);
      }
      av         = sum / n;
      double rms = 0;
      for (unsigned int ent = 0; ent < fNTest; ent++) {
        double j = fTestValues[ent][par];
        rms += (av - j) * (av - j);
      }
      rms  = TMath::Sqrt(rms / n);
      mini = TMath::Min(mini, av - rms * 4.0);
      maxi = TMath::Max(maxi, av + rms * 4.0);
      fAxesConf.push_back(Hal::HistogramAxisConf(Form("%i", count++), 200, mini, maxi));
    }

    Initialize();
    for (auto i : fTestValues) {
      FillUnchecked(i);
    }
    fTestValues.clear();
  }

  void CorrelationHisto::FillUnchecked(std::vector<Double_t> params) {
    int count = 0;
    for (int i = 0; i < fNParams; i++) {
      double x = params[i];
      for (int j = i; j < fNParams; j++) {
        double y = params[j];
        fHistograms[count++]->Fill(x, y);
      }
    }
  }

  void CorrelationHisto::Draw(Option_t* /*opt*/) {
    TCanvas* c = new TCanvas();
    auto pads  = Hal::Std::GetGridPad(fNParams, fNParams, 0.124, 0.124);
    int count  = 0;
    for (int i = 0; i < fNParams; i++) {
      for (int j = i; j < fNParams; j++) {
        pads[i][j]->cd();
        fHistograms[count++]->Draw("col");
      }
    }
  }

  CorrelationHisto::~CorrelationHisto() {
    if (fHistograms) {
      for (int i = 0; i < fNHistograms; i++) {
        delete fHistograms[i];
      }
      delete[] fHistograms;
    }
  }

}  // namespace Hal
