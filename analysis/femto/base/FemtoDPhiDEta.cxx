/*
 * FemtoDPhiDEta.cxx
 *
 *  Created on: 30 lis 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoDPhiDEta.h"
#include "FemtoPair.h"
#include <TAxis.h>
#include <TBrowser.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TMath.h>
#include <TROOT.h>
#include <TString.h>

#include "StdString.h"

namespace Hal {
  FemtoDPhiDEta::FemtoDPhiDEta() : DividedHisto2D("#phi#eta", 2) {
    AddLabel(Femto::KinematicsToLabel(Femto::EKinematics::kPHIETA));
    SetAxisName(Femto::KinematicsToAxisLabel(Femto::EKinematics::kPHIETA, 3, 2));
    SetAxisNames(GetNum());
    SetAxisNames(GetDen());
  }

  void FemtoDPhiDEta::Browse(TBrowser* b) {
    gPad->Clear();
    TVirtualPad* c1 = gPad;
    Draw("all");
    gPad = c1;
    b->Add(fNum);
    b->Add(fDen);
  }

  TString FemtoDPhiDEta::HTMLExtract(Int_t counter, TString dir) const {
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

  void FemtoDPhiDEta::FillNumObj(TObject* obj) {
    FemtoPair* pair = (FemtoPair*) obj;
    ((TH2D*) fNum)->Fill(pair->GetX(), pair->GetY(), pair->GetWeight());
  }

  void FemtoDPhiDEta::FillDenObj(TObject* obj) {
    FemtoPair* pair = (FemtoPair*) obj;
    ((TH2D*) fDen)->Fill(pair->GetX(), pair->GetY(), pair->GetWeight());
  }

  FemtoDPhiDEta::FemtoDPhiDEta(TString name, Int_t phibins, Int_t eta_bins, Double_t min, Double_t max) :
    DividedHisto2D(name, phibins, -TMath::Pi() * 0.5, TMath::Pi() * 1.5, eta_bins, min, max, 'D') {
    AddLabel(Femto::KinematicsToLabel(Femto::EKinematics::kPHIETA));
    SetAxisName(Femto::KinematicsToAxisLabel(Femto::EKinematics::kPHIETA, 3, 2));
    SetAxisNames(GetNum());
    SetAxisNames(GetDen());
  }

  FemtoDPhiDEta::~FemtoDPhiDEta() {}

  void FemtoDPhiDEta::Draw(Option_t* opt) {
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
      h->Draw("SURF1");
      c1->cd(2);
      TH2* num = (TH2*) GetNum()->Clone("temp2dN");
      if (rebin) num->Rebin2D(rebinX, rebinY);
      num->Draw("SURF1");
      c1->cd(3);
      TH2* den = (TH2*) GetDen()->Clone("temp2dD");
      if (rebin) den->Rebin2D(rebinX, rebinY);
      den->Draw("SURF1");
      gPad = c1;
    } else {
      GetHist(kFALSE)->Draw("SURF1");
    }
  }

  void FemtoDPhiDEta::SetAxisNames(TH1* h) {
    if (h == NULL) return;
    TH2* h2 = (TH2*) h;
    h2->GetXaxis()->SetTitle(Femto::KinematicsToAxisLabel(Femto::EKinematics::kPHIETA, 0, 2));
    h2->GetYaxis()->SetTitle(Femto::KinematicsToAxisLabel(Femto::EKinematics::kPHIETA, 1, 2));
    h2->GetZaxis()->SetTitle(Femto::KinematicsToAxisLabel(Femto::EKinematics::kPHIETA, 2, 2));
  }
}  // namespace Hal
