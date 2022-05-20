/*
 * Femto1DCF.cxx
 *
 *  Created on: 12-03-2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Femto1DCF.h"

#include "CorrFit1DCF.h"
#include "Cout.h"
#include "FemtoPair.h"
#include "Std.h"
#include "HtmlCore.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TAttLine.h>
#include <TAxis.h>
#include <TBrowser.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TLegend.h>
#include <TMathBase.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TROOT.h>
#include <TSystem.h>
#include <iostream>


namespace Hal {

  void Femto1DCF::DrawScaled(Double_t scale, Option_t* opt) {
    TH1* h = GetHist(kFALSE);
    h->Scale(scale);
    h->Draw(opt);
  }

  TString Femto1DCF::GetPic() const {
    TObjArray* clones = new TObjArray();
    clones->SetOwner(kTRUE);
    clones->AddLast(new TObjString("divided_0.png"));
    clones->AddLast(new TObjString("divided_1.png"));
    clones->AddLast(new TObjString("divided_2.png"));
    clones->AddLast(new TObjString("divided_3.png"));
    TString pic_text = HtmlCore::ClickablePic("femto_pic", clones, 996, 1472);
    delete clones;
    return pic_text;
  }

  void Femto1DCF::SetAxisNames(TH1* h) {
    if (h) {
      h->GetXaxis()->SetTitle(Femto::KinematicsToAxisLabel(fFrame, 0, 1));
      h->GetYaxis()->SetTitle(Femto::KinematicsToAxisLabel(fFrame, 1, 1));
      SetAxisName(Femto::KinematicsToAxisLabel(fFrame, 3, 1));
    }
  }

  void Femto1DCF::FillNumObj(TObject* obj) {
    FemtoPair* pair = (FemtoPair*) obj;
    if (pair->IsAbs()) {
      fNum->Fill(TMath::Abs(pair->GetT()), pair->GetWeight());
    } else {
      fNum->Fill(pair->GetT(), pair->GetWeight());
    }
  }

  void Femto1DCF::FillDenObj(TObject* obj) {
    FemtoPair* pair = (FemtoPair*) obj;
    if (pair->IsAbs()) {
      fDen->Fill(TMath::Abs(pair->GetT()), pair->GetWeight());
    } else {
      fDen->Fill(pair->GetT(), pair->GetWeight());
    }
  }

  Femto1DCF::Femto1DCF() : DividedHisto1D(), fFrame(Femto::EKinematics::kLCMS) {}

  Femto1DCF::Femto1DCF(const Femto1DCF& other) : DividedHisto1D(other), fFrame(other.fFrame) {}

  Femto1DCF::~Femto1DCF() {
    // TODO Auto-generated destructor stub
  }

  Femto1DCF::Femto1DCF(TString name, Femto::EKinematics frame) : DividedHisto1D(name, 1), fFrame(frame) {
    SetName(name);
    AddLabel(Femto::KinematicsToLabel(fFrame));
  }

  Femto1DCF::Femto1DCF(TString name, Int_t bins, Double_t min, Double_t max, Femto::EKinematics frame) :
    DividedHisto1D(name, bins, min, max), fFrame(frame) {
    SetAxisNames(fNum);
    SetAxisNames(fDen);
    fNum->Sumw2();
    fDen->Sumw2();
    AddLabel(Femto::KinematicsToLabel(fFrame));
  }

  void Femto1DCF::AddNum(TH1* h, Option_t* opt) {
    DividedHisto1D::AddNum(h, opt);
    SetAxisNames(fNum);
  }

  void Femto1DCF::AddDen(TH1* h, Option_t* opt) {
    DividedHisto1D::AddDen(h, opt);
    SetAxisNames(fDen);
  }

  TString Femto1DCF::HTMLExtract(Int_t counter, TString dir) const {
    TString path = Form("%s/divided_%i", dir.Data(), counter);
    gSystem->MakeDirectory(path);
    Bool_t batch = gROOT->IsBatch();
    gROOT->SetBatch(kTRUE);
    TCanvas* c1 = new TCanvas("canvas", "canvas", 0, 0, 1000, 1500);
    c1->Divide(2, 1);
    c1->cd(1);
    TH1* cf = GetHist(kTRUE);
    cf->Draw();
    c1->cd(2);
    TH1* num = (TH1*) GetNum()->Clone();
    TH1* den = (TH1*) GetDen()->Clone();
    den->SetLineColor(kRed);
    num->SetLineColor(kGreen);
    den->Draw();
    num->Draw("SAME");
    TLegend* leg = new TLegend(0.4, 0.8, 0.9, 0.9);
    leg->AddEntry(num, "Numerator", "LPM");
    leg->AddEntry(den, "Denominator", "LPM");
    leg->Draw("SAME");
    c1->SaveAs(Form("%s/divided.root", path.Data()));
    delete c1;
    delete num;
    delete den;
    delete cf;
    delete leg;
    gROOT->SetBatch(batch);
    TString page = CommonExtract(counter, dir);
    return page;
  }

  void Femto1DCF::Browse(TBrowser* b) {
    gPad->Clear();
    TVirtualPad* c1 = gPad;
    Draw("all");
    gPad = c1;
    b->Add(fNum);
    b->Add(fDen);
  }

  Array_1<Float_t>* Femto1DCF::ExportToFlatNum() const {
    TH1D* h                = (TH1D*) GetHist(kFALSE);
    Array_1<Float_t>* data = new Array_1<Float_t>(h->GetNbinsX());
    for (int i = 1; i <= h->GetNbinsX(); i++) {
      (*data)[i - 1] = h->GetBinContent(i);
    }
    delete h;
    return data;
  }

  void Femto1DCF::Print(Option_t* opt) const {
    DividedHisto1D::Print(opt);
    TString text = Form("Frame : %s", Femto::KinematicsToLabel(fFrame).Data());
    Cout::Text(text, "L", kWhite);
  }

  void Femto1DCF::Fit(CorrFit1DCF* fit) { fit->Fit(this); }

  void Femto1DCF::FitDummy(CorrFit1DCF* fit) { fit->FitDummy(this); }
}  // namespace Hal
