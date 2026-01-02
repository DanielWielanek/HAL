/*
 * PeakFitterX.cxx
 *
 *  Created on: 2 sty 2026
 *      Author: daniel
 */

#include "PeakFitterX.h"

#include <RtypesCore.h>
#include <TAxis.h>
#include <TF1.h>
#include <TFitResultPtr.h>
#include <TGraph.h>
#include <TH1.h>
#include <TLegend.h>
#include <TMathBase.h>
#include <TString.h>
#include <vector>

#include "Std.h"

namespace Hal {

  PeakFitterX::PeakFitterX() {}

  Double_t PeakFitterX::EvalNumSignal(Double_t* x, Double_t* params) const {
    return fData->Eval(x[0]) - fBackgroundFunction->Eval(x[0]);
  }

  void PeakFitterX::SetData(const TGraphErrors& gr) {
    if (fData) delete fData;
    fData = (TGraphErrors*) gr.Clone();
    MakeFilteredData();
  }

  void PeakFitterX::SetData(const TH1& data) {
    if (fData) delete fData;
    fData = new TGraphErrors();
    for (int i = 1; i <= data.GetNbinsX(); i++) {
      fData->SetPoint(i - 1, data.GetXaxis()->GetBinCenter(i), data.GetBinContent(i));
      fData->SetPointError(i - 1, 0, data.GetBinError(i));
    }
    MakeFilteredData();
  }

  void PeakFitterX::SetPeakRegion(Double_t lo, Double_t hi) {
    fEx[0] = TMath::Min(lo, hi);
    fEx[1] = TMath::Max(lo, hi);
  }

  void PeakFitterX::FitBackground() { fFilteredData->Fit(fBackgroundFunction, "RQ"); }

  void PeakFitterX::FitSignal(TString opt) {
    if (fTotalFunction) delete fTotalFunction;
    if (!fSignalFunction) {
      std::cout << "No signal function, using numerical formula" << std::endl;
      fSignalFunction = new TF1(Hal::Std::GetUniqueName("func_draw"),
                                this,
                                &PeakFitterX::EvalNumSignal,
                                fEx[0],
                                fEx[1],
                                0,
                                this->ClassName(),
                                "EvalNumSignal");
    }
    Double_t lo = fData->GetPointX(0);
    Double_t hi = fData->GetPointX(fData->GetN() - 1);
    if (Hal::Std::FindParam(opt, "peak") && fEx[0] != fEx[1]) {
      lo = fEx[0];
      hi = fEx[1];
    }
    fTotalFunction = new TF1(
      Hal::Std::GetUniqueName("xx"),
      [&](double* x, double* p) { return fSignalFunction->EvalPar(x, p) + fBackgroundFunction->Eval(x[0]); },
      lo,
      hi,
      fSignalFunction->GetNpar());
    for (int i = 0; i < fSignalFunction->GetNpar(); i++) {
      Double_t parmin, parmax;
      fSignalFunction->GetParLimits(i, parmin, parmax);
      if (parmin == parmax)
        fTotalFunction->FixParameter(i, parmin);
      else { fTotalFunction->SetParLimits(i, parmin, parmax); }
    }
    fData->Fit(fTotalFunction, "R");
    fTotalFunction->SetRange(fData->GetPointX(0), fData->GetPointX(fData->GetN() - 1));
    for (int i = 0; i < fTotalFunction->GetNpar(); i++) {
      fSignalFunction->FixParameter(i, fTotalFunction->GetParameter(i));
    }
  }

  void PeakFitterX::Draw(Option_t* option) {
    TString opt = option;
    if (Hal::Std::FindParam(opt, "autocolor", kTRUE)) {
      fSignalFunction->SetLineColor(kBlack);
      fTotalFunction->SetLineColor(kRed);
      fBackgroundFunction->SetLineColor(kGreen);
      fBackgroundFunction->SetLineStyle(7);
      fData->SetMarkerStyle(kFullCircle);
    }
    Bool_t drawSignal = false, drawBackground = false, drawTotal = false, drawData = false;
    drawData       = Hal::Std::FindParam(opt, "data", kTRUE);
    drawBackground = Hal::Std::FindParam(opt, "bckg", kTRUE);
    drawTotal      = Hal::Std::FindParam(opt, "tot", kTRUE);
    drawSignal     = Hal::Std::FindParam(opt, "sig", kTRUE);
    if (drawData) fData->Draw("AP");

    if (Hal::Std::FindParam(opt, "zero", kTRUE)) { fData->SetMinimum(0); }
    if (drawTotal) fTotalFunction->Draw("SAME");
    if (drawBackground) fBackgroundFunction->Draw("SAME");
    if (drawSignal) fSignalFunction->Draw("SAME");
    if (Hal::Std::FindParam(opt, "legend", kTRUE)) {
      Double_t sigval  = fSignalFunction->Integral(fEx[0], fEx[1]);
      Double_t totval  = fTotalFunction->Integral(fEx[0], fEx[1]);
      Double_t bckgval = fBackgroundFunction->Integral(fEx[0], fEx[1]);
      TLegend* leg     = new TLegend(0.7, 0.7, 0.99, 0.99);
      leg->SetHeader(Form("S/S+B = %4.2f", sigval / totval));
      if (drawTotal) leg->AddEntry(fTotalFunction, Form("Total %4.2f", totval));
      if (drawSignal) leg->AddEntry(fSignalFunction, Form("Signal %4.2f", sigval));
      if (drawBackground) leg->AddEntry(fBackgroundFunction, Form("Background %4.2f", bckgval));
      if (drawData) leg->AddEntry(fData, "Data");
      leg->Draw("SAME");
    }
  }

  void PeakFitterX::MakeFilteredData() {
    if (fFilteredData) delete fFilteredData;
    fFilteredData = new TGraphErrors();
    int count     = 0;
    for (int i = 0; i < fData->GetN(); i++) {
      Bool_t reject = false;
      Double_t x, y, ex = 0, ey;
      fData->GetPoint(i, x, y);
      ey = fData->GetErrorY(i);
      if (fEx[0] <= fEx[1]) {
        if (x >= fEx[0] && x <= fEx[1]) reject = true;
      }
      if (fRejectingPointsInt.size()) {
        for (auto z : fRejectingPointsInt) {
          if (z == i + 1) reject = true;
        }
      }
      if (reject) continue;
      fFilteredData->SetPoint(count, x, y);
      fFilteredData->SetPointError(count, ex, ey);
      count++;
    }
  }

  Double_t PeakFitterX::GetB() const { return fBackgroundFunction->Integral(fEx[0], fEx[1]); }

  Double_t PeakFitterX::GetS() const { return fSignalFunction->Integral(fEx[0], fEx[1]); }

  PeakFitterX::~PeakFitterX() {
    if (fData) delete fData;
    if (fFilteredData) delete fFilteredData;
    if (fTotalFunction) delete fTotalFunction;
  }


} /* namespace Hal */
