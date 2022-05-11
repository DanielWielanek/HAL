/*
 * FemtoDoubleRatio1DCF.cxx
 *
 *  Created on: 21 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoDoubleRatio1DCF.h"

#include "FemtoPair.h"
#include "HalStdString.h"

#include <TAttLine.h>
#include <TAxis.h>
#include <TBrowser.h>
#include <TCanvas.h>
#include <TCollection.h>
#include <TH1.h>
#include <TList.h>
#include <TMathBase.h>
#include <TNamed.h>
#include <TROOT.h>
#include <TString.h>
#include <TSystem.h>
#include <TVirtualPad.h>


namespace Hal {
  FemtoDoubleRatio1DCF::FemtoDoubleRatio1DCF(TString name, Femto::EKinematics kin) :
    DividedHisto1D(name, 3), fFrame(kin), fNumSide(NULL), fDenSide(NULL), fNumLong(NULL), fDenLong(NULL) {}

  FemtoDoubleRatio1DCF::FemtoDoubleRatio1DCF(TString name, Int_t bins, Double_t min, Double_t max, Femto::EKinematics kin) :
    FemtoDoubleRatio1DCF(name, kin) {
    SetName(name);

    fNum     = new TH1D(Form("%s_NumOut", name.Data()), Form("%s_NumOut", name.Data()), min, max);
    fDen     = new TH1D(Form("%s_DenOut", name.Data()), Form("%s_DenOut", name.Data()), min, max);
    fNumSide = new TH1D(Form("%s_NumSide", name.Data()), Form("%s_NumSide", name.Data()), min, max);
    fDenSide = new TH1D(Form("%s_DenSide", name.Data()), Form("%s_DenSide", name.Data()), min, max);
    fNumLong = new TH1D(Form("%s_NumLong", name.Data()), Form("%s_NumLong", name.Data()), min, max);
    fDenLong = new TH1D(Form("%s_DenLong", name.Data()), Form("%s_DenLong", name.Data()), min, max);
    SetAxisNames(fNum, fDen, "out");
    SetAxisNames(fNumSide, fDenSide, "out");
    SetAxisNames(fNumLong, fDenLong, "long");
  }

  void FemtoDoubleRatio1DCF::Browse(TBrowser* b) {
    gPad->Clear();
    TVirtualPad* c1 = gPad;
    Draw();
    b->Add(fNum);
    b->Add(fNumSide);
    b->Add(fNumLong);
    b->Add(fDen);
    b->Add(fDenSide);
    b->Add(fDenLong);
    gPad = c1;
  }

  void FemtoDoubleRatio1DCF::Draw(Option_t* opt) {
    if (gPad == NULL) new TCanvas();
    TVirtualPad* c1 = gPad;
    TVirtualPad* c2 = gPad;
    TString option  = opt;
    TString same    = "";
    if (HalStd::FindParam(option, "same")) { same = "SAME"; }
    if (option == "num") {
      if (c1->GetListOfPrimitives()->GetEntries() < 3) c2->Divide(3, 1);
      c2->cd(1);
      GetOutNum()->Draw(same);
      c2->cd(2);
      GetSideNum()->Draw(same);
      c2->cd(3);
      GetLongNum()->Draw(same);

    } else if (option == "den") {
      if (c1->GetListOfPrimitives()->GetEntries() < 3) c2->Divide(3, 1);
      c2->cd(1);
      GetOutDen()->Draw(same);
      c2->cd(2);
      GetSideDen()->Draw(same);
      c2->cd(3);
      GetLongDen()->Draw(same);
    } else {
      if (c1->GetListOfPrimitives()->GetEntries() < 3) c2->Divide(3, 1);
      TH1D* cfx = GetCFOut(kTRUE);
      TH1D* cfy = GetCFOut(kTRUE);
      TH1D* cfz = GetCFOut(kTRUE);
      c2->cd(1);
      cfx->Draw(same);
      c2->cd(2);
      cfy->Draw(same);
      c2->cd(3);
      cfz->Draw(same);
    }
    gPad = c1;
  }

  TString FemtoDoubleRatio1DCF::HTMLExtract(Int_t counter, TString dir) const {
    Bool_t batch = gROOT->IsBatch();
    TString path = Form("%s/divided_%i", dir.Data(), counter);
    gSystem->MakeDirectory(path);
    gROOT->SetBatch(kTRUE);
    TCanvas* c1 = new TCanvas("canvas", "canvas", 0, 0, 1000, 1500);
    c1->Divide(3, 3);
    c1->cd(1);
    TH1D* cf1 = GetCFOut(kTRUE);
    cf1->SetObjectStat(kFALSE);
    cf1->Draw();
    c1->cd(2);
    TH1D* cf2 = GetCFSide(kTRUE);
    cf2->SetObjectStat(kFALSE);
    cf2->Draw();
    c1->cd(3);
    TH1D* cf3 = GetCFLong(kTRUE);
    cf3->SetObjectStat(kFALSE);
    cf3->Draw();
    TH1D** nums = new TH1D*[3];
    TH1D** dens = new TH1D*[3];
    nums[0]     = (TH1D*) GetOutNum()->Clone();
    nums[1]     = (TH1D*) GetSideNum()->Clone();
    nums[2]     = (TH1D*) GetLongNum()->Clone();
    dens[0]     = (TH1D*) GetOutDen()->Clone();
    dens[1]     = (TH1D*) GetSideDen()->Clone();
    dens[2]     = (TH1D*) GetLongDen()->Clone();

    for (int i = 0; i < 3; i++) {
      c1->cd(3 + i);
      nums[i]->SetLineColor(kGreen);
      nums[i]->SetObjectStat(kFALSE);
      nums[i]->Draw();
    }
    for (int i = 0; i < 3; i++) {
      c1->cd(6 + i);
      dens[i]->SetLineColor(kRed);
      dens[i]->SetObjectStat(kFALSE);
      dens[i]->Draw();
    }
    gSystem->mkdir(path);
    c1->SaveAs(Form("%s/divided.root", path.Data()));
    delete c1;
    delete cf1;
    delete cf2;
    delete cf3;
    for (int i = 0; i < 3; i++) {
      delete nums[i];
      delete dens[i];
    }
    delete[] nums;
    delete[] dens;
    gROOT->SetBatch(batch);
    TString page = CommonExtract(counter, dir);
    return page;
  }

  void FemtoDoubleRatio1DCF::Rebin(Int_t ngroup, Option_t* opt) {
    TString option = opt;
    if (option == "out") {
      fNum->Rebin(ngroup);
      fDen->Rebin(ngroup);
    } else if (option == "side") {
      fNumSide->Rebin(ngroup);
      fDenSide->Rebin(ngroup);
    } else if (option == "long") {
      fNumLong->Rebin(ngroup);
      fDenLong->Rebin(ngroup);
    } else {
      Rebin(ngroup, "out");
      Rebin(ngroup, "side");
      Rebin(ngroup, "long");
    }
  }

  void FemtoDoubleRatio1DCF::SetAxisNames(TH1* h1, TH1* h2, TString label) {
    if (h1 == NULL || h2 == NULL) return;
    switch (fFrame) {
      case Femto::EKinematics::kPRF:
        h1->GetXaxis()->SetTitle(Form("k*^{%s} [GeV/c]", label.Data()));
        h2->GetXaxis()->SetTitle(Form("k*^{%s} [GeV/c]", label.Data()));
        SetAxisName("CF(k*)");
        break;
      case Femto::EKinematics::kLCMS:
        h1->GetXaxis()->SetTitle(Form("q_{inv}^{%s} [GeV/c]", label.Data()));
        h2->GetXaxis()->SetTitle(Form("q_{inv}^{%s} [GeV/c]", label.Data()));
        SetAxisName("CF(q)");
        break;
      default: break;
    }
  }

  void FemtoDoubleRatio1DCF::SetNum(TH1D* outnum, TH1D* sidenum, TH1D* longnum, Bool_t clone) {
    fNum     = outnum;
    fNumSide = outnum;
    fNumLong = longnum;
    if (clone) {
      fNumIsCloned = kTRUE;
      fDenIsCloned = kTRUE;
      fNum         = (TH1D*) outnum->Clone();
      fNumSide     = (TH1D*) outnum->Clone();
      fNumLong     = (TH1D*) longnum->Clone();
    } else {
      fNumIsCloned = kFALSE;
      fNum         = outnum;
      fNumSide     = outnum;
      fNumLong     = longnum;
    }
  }

  void FemtoDoubleRatio1DCF::SetDen(TH1D* outden, TH1D* sideden, TH1D* longden, Bool_t clone) {
    if (clone) {
      fDenIsCloned = kTRUE;
      fDen         = (TH1D*) outden->Clone();
      fDenSide     = (TH1D*) outden->Clone();
      fDenLong     = (TH1D*) longden->Clone();
    } else {
      fDenIsCloned = kFALSE;
      fDen         = outden;
      fDenSide     = outden;
      fDenLong     = longden;
    }
  }

  TH1D* FemtoDoubleRatio1DCF::GetCFOut(Bool_t normalized) const { return GetCF(normalized, 0); }

  TH1D* FemtoDoubleRatio1DCF::GetCFSide(Bool_t normalized) const { return GetCF(normalized, 1); }

  TH1D* FemtoDoubleRatio1DCF::GetCFLong(Bool_t normalized) const { return GetCF(normalized, 2); }

  void FemtoDoubleRatio1DCF::Normalize(TH1D* h, TH1D* num, TH1D* den, Int_t dir) const {
    if (fScale != 0) h->Scale(fScale);
    Int_t bin_min  = h->GetXaxis()->FindBin(fNormMin[dir]);
    Int_t bin_max  = h->GetXaxis()->FindBin(fNormMax[dir]);
    Double_t int_a = num->Integral(bin_min, bin_max);
    Double_t int_b = den->Integral(bin_min, bin_max);
    if (int_a != 0 && int_b != 0) { h->Scale(int_b / int_a); }
  }

  TH1D* FemtoDoubleRatio1DCF::GetCF(Bool_t normalized, Int_t dir) const {
    TH1 *num, *den;
    switch (dir) {
      case 0:
        num = fNum;
        den = fDen;
        break;
      case 1:
        num = fNumSide;
        den = fDenSide;
        break;
      case 2:
        num = fNumLong;
        den = fDenLong;
        break;
    }
    TString numname  = num->GetName();
    TString newname  = numname.ReplaceAll("Num", "CF");
    TString numtitle = num->GetTitle();
    TString newtitle = numtitle.ReplaceAll("Num", "CF");
    TH1D* cf         = (TH1D) num->Clone(newname);
    cf->SetTitle(newtitle);
    cf->SetYTitle(GetAxisName());
    if (normalized) Normalize(cf, (TH1D*) num, (TH1D*) den, dir);
    return cf;
  }

  void FemtoDoubleRatio1DCF::Add(const Object* h) {
    DividedHisto1D::Add(h);
    FemtoDoubleRatio1DCF* other = (FemtoDoubleRatio1DCF*) h;
    fNumLong->Add(other->fNumLong);
    fDenLong->Add(other->fDenLong);
    fNumSide->Add(other->fNumSide);
    fDenSide->Add(other->fDenSide);
  }

  FemtoDoubleRatio1DCF::FemtoDoubleRatio1DCF(const FemtoDoubleRatio1DCF& other) : DividedHisto1D(other) {
    fNumSide = (TH1D*) other.fNumSide->Clone();
    fDenSide = (TH1D*) other.fDenSide->Clone();
    fNumLong = (TH1D*) other.fNumLong->Clone();
    fDenLong = (TH1D*) other.fDenLong->Clone();
    fFrame   = other.fFrame;
  }

  void FemtoDoubleRatio1DCF::FillNumObj(TObject* ob) {
    FemtoPair* pair = (FemtoPair*) ob;
    Double_t total  = TMath::Abs(pair->GetT());
    Double_t weight = pair->GetWeight();
    if (pair->GetX() >= 0) {
      fNum->Fill(total, weight);
    } else {
      fNum->Fill(-total, weight);
    }
    if (pair->GetY() >= 0) {
      fNumSide->Fill(total, weight);
    } else {
      fNumSide->Fill(-total, weight);
    }
    if (pair->GetZ() >= 0) {
      fNumLong->Fill(total, weight);
    } else {
      fNumLong->Fill(-total, weight);
    }
  }

  void FemtoDoubleRatio1DCF::FillDenObj(TObject* obj) {
    FemtoPair* pair = (FemtoPair*) obj;
    Double_t total  = TMath::Abs(pair->GetT());
    Double_t weight = pair->GetWeight();
    if (pair->GetX() >= 0) {
      fDen->Fill(total, weight);
    } else {
      fDen->Fill(-total, weight);
    }
    if (pair->GetY() >= 0) {
      fDenSide->Fill(total, weight);
    } else {
      fDenSide->Fill(-total, weight);
    }
    if (pair->GetZ() >= 0) {
      fDenLong->Fill(total, weight);
    } else {
      fDenLong->Fill(-total, weight);
    }
  }

  FemtoDoubleRatio1DCF::FemtoDoubleRatio1DCF() :
    DividedHisto1D(),
    fNumSide(nullptr),
    fDenSide(nullptr),
    fNumLong(nullptr),
    fDenLong(nullptr),
    fFrame(Femto::EKinematics::kLCMS) {}

  FemtoDoubleRatio1DCF::~FemtoDoubleRatio1DCF() {
    if (fNumIsCloned) {
      delete fNumSide;
      delete fNumLong;
    }
    if (fDenIsCloned) {
      delete fDenSide;
      delete fDenLong;
    }
  }
}  // namespace Hal
