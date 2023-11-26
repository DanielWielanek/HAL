/*
 * FemtoDebug2DCF.cxx
 *
 *  Created on: 25 lis 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoDebug2DCF.h"
#include "FemtoConst.h"
#include "FemtoPair.h"

#include "StdString.h"

#include <RtypesCore.h>
#include <TAxis.h>
#include <TBrowser.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TMath.h>
#include <TROOT.h>
#include <TString.h>
#include <iostream>
#include <stddef.h>

namespace Hal {
  std::function<void(TH1*, Hal::FemtoPair*)>* FemtoDebug2DCF::fMagicFunction = {nullptr};
  FemtoDebug2DCF::FemtoDebug2DCF() : DividedHisto2D("xy", 2) {
    SetAxisNames(GetNum());
    SetAxisNames(GetDen());
  }

  void FemtoDebug2DCF::Browse(TBrowser* b) {
    gPad->Clear();
    TVirtualPad* c1 = gPad;
    Draw("all");
    gPad = c1;
    b->Add(fNum);
    b->Add(fDen);
  }

  TString FemtoDebug2DCF::HTMLExtract(Int_t counter, TString dir) const {
    Bool_t batch = gROOT->IsBatch();
    gROOT->SetBatch(kTRUE);
    TCanvas* c1 = new TCanvas("canvas", "canvas", 0, 0, 1000, 1500);
    c1->Divide(2, 2);
    c1->cd(1);
    TH2* cf = (TH2*) GetHist(kTRUE);
    cf->Draw("SURF1");
    c1->cd(2);
    TH2* num = (TH2*) GetNum()->Clone("temp_n");
    num->Draw("SURF1");
    c1->cd(3);
    TH2* den = (TH2*) GetDen()->Clone("temp_d");
    den->Draw("SURF1");
    c1->SaveAs(Form("%s/divided_%i/divided.root", dir.Data(), counter));
    gROOT->SetBatch(batch);
    delete num;
    delete den;
    delete cf;
    TString page = CommonExtract(counter, dir);
    return page;
  }

  void FemtoDebug2DCF::FillNumObj(TObject* obj) {
    FemtoPair* pair = (FemtoPair*) obj;
    (*fMagicFunction)(fNum, pair);
  }

  void FemtoDebug2DCF::FillDenObj(TObject* obj) {
    FemtoPair* pair = (FemtoPair*) obj;
    (*fMagicFunction)(fDen, pair);
  }

  FemtoDebug2DCF::FemtoDebug2DCF(TString name, Int_t phibins, Int_t eta_bins) :
    DividedHisto2D(name, phibins + 1, -0.5, double(phibins) + 0.5, eta_bins + 1, -0.5, double(eta_bins) - 1, 'D') {
    SetAxisNames(GetNum());
    SetAxisNames(GetDen());
  }

  FemtoDebug2DCF::~FemtoDebug2DCF() {}

  void FemtoDebug2DCF::Draw(Option_t* opt) {
    TString option = opt;
    if (Hal::Std::FindParam(option, "num", kTRUE)) {
      GetNum()->Draw("SURF1");
    } else if (Hal::Std::FindParam(option, "den", kTRUE)) {
      GetDen()->Draw("SURF1");
    } else if (Hal::Std::FindParam(option, "all", kTRUE)) {
      TVirtualPad* c1 = gPad;
      c1->Divide(2, 2);
      c1->cd(1);
      Bool_t rebin = kTRUE;
      if (Hal::Std::FindParam(option, "hd", kTRUE)) { rebin = kFALSE; }
      TH2* h       = (TH2*) GetHist(kTRUE);
      Int_t rebinX = 1;
      Int_t rebinY = 1;
      if (rebin) {
        if (h->GetNbinsX() > 20) { rebinX = h->GetNbinsX() / 20; }
        if (h->GetNbinsY() > 20) { rebinY = h->GetNbinsY() / 20; }
        h->Rebin2D(rebinX, rebinY);
        h->Scale(1.0 / ((Double_t) rebinX * rebinY));
      }
      h->Draw("COL");
      c1->cd(2);
      TH2* num = (TH2*) GetNum()->Clone("temp2dN");
      if (rebin) num->Rebin2D(rebinX, rebinY);
      num->Draw("COL");
      c1->cd(3);
      TH2* den = (TH2*) GetDen()->Clone("temp2dD");
      if (rebin) den->Rebin2D(rebinX, rebinY);
      den->Draw("COL");
      gPad = c1;
    } else {
      GetHist(kFALSE)->Draw("COL");
    }
  }

  void FemtoDebug2DCF::SetAxisNames(TH1* h) {
    if (h == NULL) return;
    h->GetXaxis()->SetTitle("Xval");
    h->GetYaxis()->SetTitle("Yval");
  }

  TObject* FemtoDebug2DCF::Clone(const char* /*c*/) const { return new FemtoDebug2DCF(*this); }

  FemtoDebug2DCF::FemtoDebug2DCF(const FemtoDebug2DCF& other) : Hal::DividedHisto2D(other) {}

} /* namespace Hal */
