/*
 * HalDividedHisto1D.cxx
 *
 *  Created on: 13-10-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "HalDividedHisto.h"

#include "HalCout.h"
#include "HalHtmlCore.h"
#include "HalHtmlFile.h"
#include "HalHtmlTable.h"
#include "HalStd.h"
#include "HalStdHist.h"
#include "HalStdString.h"

#include <TAttFill.h>
#include <TAttLine.h>
#include <TAxis.h>
#include <TBrowser.h>
#include <TCanvas.h>
#include <TCollection.h>
#include <TH1.h>
#include <TLegend.h>
#include <TList.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TNamed.h>
#include <TObjString.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TVirtualPad.h>
#include <fstream>
#include <vector>


HalDividedHisto1D::HalDividedHisto1D() :
  fNumIsCloned(kFALSE),
  fDenIsCloned(kFALSE),
  fNum(nullptr),
  fDen(nullptr),
  fBinomial(kFALSE),
  fDim(0),
  fNormMin(nullptr),
  fNormMax(nullptr),
  fScale(0.0),
  fAxisName(""),
  fComment(""),
  fLabels(nullptr) {}

HalDividedHisto1D::HalDividedHisto1D(TString name, const Int_t dim) :
  fNumIsCloned(kFALSE),
  fDenIsCloned(kFALSE),
  fNum(NULL),
  fDen(NULL),
  fBinomial(kFALSE),
  fDim(dim),
  fNormMin(NULL),
  fNormMax(NULL),
  fScale(0),
  fLabels(NULL) {
  fNormMin = new Double_t[fDim];
  fNormMax = new Double_t[fDim];
  fName    = name;
  for (int i = 0; i < fDim; i++) {
    fNormMin[i] = fNormMax[i] = 0;
  }
  fComment = " ";
  fLabels  = new TList();
  fLabels->SetOwner(kTRUE);
  fLabels->SetName("Labels");
}

HalDividedHisto1D::HalDividedHisto1D(const HalDividedHisto1D& other) :
  fNumIsCloned(kFALSE),
  fDenIsCloned(kFALSE),
  fNum(NULL),
  fDen(NULL),
  fBinomial(other.fBinomial),
  fDim(other.fDim),
  fScale(other.fScale),
  fAxisName(other.fAxisName),
  fComment(other.fComment),
  fLabels(NULL) {
  if (other.fLabels) { fLabels = (TList*) other.fLabels->Clone(); }
  if (other.fNum) {
    fNum         = (TH1*) other.fNum->Clone();
    fNumIsCloned = kTRUE;
  }
  if (other.fDen) {
    fDen         = (TH1*) other.fDen->Clone();
    fDenIsCloned = kTRUE;
  }
  fNormMin = new Double_t[fDim];
  fNormMax = new Double_t[fDim];
  for (int i = 0; i < fDim; i++) {
    fNormMin[i] = other.fNormMin[i];
    fNormMax[i] = other.fNormMax[i];
  }
}

void HalDividedHisto1D::Normalize(TH1* h) const {
  if (fScale == 0) {
    for (int i = 0; i < fDim; i++) {
      Int_t binMinX  = fNum->GetXaxis()->FindBin(fNormMin[0]);
      Int_t binMaxX  = fNum->GetXaxis()->FindBin(fNormMax[0]);
      Double_t num   = fNum->Integral(binMinX, binMaxX);
      binMinX        = fDen->GetXaxis()->FindBin(fNormMin[0]);
      binMaxX        = fDen->GetXaxis()->FindBin(fNormMax[0]);
      Double_t den   = fDen->Integral(binMinX, binMaxX);
      Double_t scale = 1.0;
      if (num == 0) {
        HalCout::PrintInfo("Numerator integral is zero, set scale to one", Hal::EInfo::kLessWarning);
      } else {
        scale = den / num;
      }
      if (fNormMin[i] == fNormMax[i]) {
        HalCout::PrintInfo(Form("Same norm min and norm max on %i axis", i), Hal::EInfo::kLessWarning);
        scale = 1.0;
      }
      if (scale == 0.0) scale = 1.0;
      h->Scale(scale);
    }
  } else {
    h->Scale(fScale);
  }
}

void HalDividedHisto1D::AddNum(TH1* num, Option_t* opt) {
  if (fNumIsCloned && fNum != NULL) delete fNum;
  if (num == NULL) {
    HalCout::PrintInfo("HalDividedHisto  - trying to assign NULL Numerator", Hal::EInfo::kLessWarning);
    fNum = NULL;
    return;
  }
  TString option = opt;
  if (option.EqualTo("clone")) {
    fNumIsCloned = kTRUE;
    fNum         = (TH1*) num->Clone();
  } else {
    fNumIsCloned = kFALSE;
    fNum         = num;
  }
}

void HalDividedHisto1D::AddDen(TH1* den, Option_t* opt) {
  if (fDenIsCloned && fDen != NULL) delete fDen;
  if (den == NULL) {
    HalCout::PrintInfo("HalDividedHisto  - trying to assign NULL Denominator", Hal::EInfo::kLessWarning);
    fDen = NULL;
    return;
  }
  TString option = opt;
  if (option.EqualTo("clone")) {
    fDenIsCloned = kTRUE;
    fDen         = (TH1*) den->Clone();
  } else {
    fDenIsCloned = kFALSE;
    fDen         = den;
  }
}

void HalDividedHisto1D::AddNumDen(TH1* num, TH1* den, Option_t* opt) {
  AddNum(num, opt);
  AddDen(den, opt);
}

TH1* HalDividedHisto1D::GetNum() const { return fNum; }

TH1* HalDividedHisto1D::GetDen() const { return fDen; }

TH1* HalDividedHisto1D::GetHist(Bool_t normalized) const {
  TH1* res     = (TH1*) fNum->Clone();
  TString name = res->GetTitle();
  name         = name + "Divided";
  res->SetTitle(name);
  if (fBinomial) {
    res->Divide(res, fDen, 1, 1, "B");
  } else {
    res->Divide(fDen);
  }
  if (normalized) Normalize(res);
  SetHistoName(res);
  return res;
}

void HalDividedHisto1D::SetNorm(Double_t min, Double_t max, Int_t axis) {
  if (axis < 0) {
    HalCout::PrintInfo("HalDividedHisto too small norm axis", Hal::EInfo::kLessWarning);
    return;
  } else if (axis > fDim) {
    HalCout::PrintInfo(("HalDividedHist too large norm axis"), Hal::EInfo::kLessWarning);
    return;
  }
  if (min > max) {
    HalCout::PrintInfo("HalDividedHisto min> max", Hal::EInfo::kLessWarning);
    return;
  } else if (min == max) {
    HalCout::PrintInfo("HalDividedHisto min == max", Hal::EInfo::kLessWarning);
    return;
  }
  fNormMin[axis] = min;
  fNormMax[axis] = max;
}

void HalDividedHisto1D::SetScale(Double_t scale) { fScale = scale; }

void HalDividedHisto1D::Add(const HalObject* h) {
  HalDividedHisto1D* div = (HalDividedHisto1D*) h;
  if (div->fDim != this->fDim) {
    HalCout::PrintInfo(Form("Invalid dimensions in %s and %s", this->ClassName(), h->ClassName()), Hal::EInfo::kImportantError);
  }
  for (int i = 0; i < fDim; i++) {
    if (this->fNormMin[i] != div->fNormMin[i]) {
      HalCout::PrintInfo("Invalid minimum values in HalDividedHisto", Hal::EInfo::kLessError);
    } else if (this->fNormMax[i] != div->fNormMax[i]) {
      HalCout::PrintInfo("Invalid maximum values in HalDividedHisto", Hal::EInfo::kLessError);
    }
  }
  if ((this->fScale == 0 && div->fScale != 0) || (this->fScale > 0 && div->fScale == 0)) {
    HalCout::PrintInfo(("Incompatibile scales !"), Hal::EInfo::kLessError);
    fScale = 1.0;
  }
  if (fScale == 0) {  // use norms
    this->fNum->Add(div->fNum);
    this->fDen->Add(div->fDen);
  } else {
    this->fScale = (div->fScale + this->fScale) / 2.0;
    this->fNum->Add(div->fNum);
    this->fDen->Add(div->fDen);
  }
}

void HalDividedHisto1D::SetHistoName(TH1* h) const {
  switch (fDim) {
    case 1: h->GetYaxis()->SetTitle(fAxisName); break;
    case 2: h->GetZaxis()->SetTitle(fAxisName); break;
    default: break;
  }
}

void HalDividedHisto1D::SetAxisName(TString name) { fAxisName = name; }

Double_t HalDividedHisto1D::GetNormMin(Int_t no) const {
  if (no >= 0 && no < fDim) { return fNormMin[no]; }
  HalCout::PrintInfo(Form("%s::GetNormMin invalid no", ClassName()), Hal::EInfo::kLessWarning);
  return 0;
}

Double_t HalDividedHisto1D::GetNormMax(Int_t no) const {
  if (no >= 0 && no < fDim) { return fNormMax[no]; }
  HalCout::PrintInfo(Form("%s::GetNormMax invalid no", ClassName()), Hal::EInfo::kLessWarning);
  return 0;
}

Double_t HalDividedHisto1D::GetScale() const { return fScale; }

TString HalDividedHisto1D::HTMLExtract(Int_t counter, TString dir) const {
  Bool_t batch = gROOT->IsBatch();
  TString path = Form("%s/divided_%i", dir.Data(), counter);
  gSystem->MakeDirectory(path);
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
  gSystem->mkdir(path);
  c1->SaveAs(Form("%s/divided.root", path.Data()));
  delete c1;
  delete leg;
  delete num;
  delete den;
  delete cf;
  gROOT->SetBatch(batch);
  TString page = CommonExtract(counter, dir);
  return page;
}

TString HalDividedHisto1D::GetExtractType() const { return "Page"; }

TString HalDividedHisto1D::CommonExtract(Int_t counter, TString dir) const {
  TString path = Form("%s/divided_%i", dir.Data(), counter);
  gSystem->MakeDirectory(path);
  TString filename = Form("%s/divided.html", path.Data());
  HalHtmlFile file(filename, kFALSE);
  Int_t dimN    = GetNDim();
  Double_t* min = new Double_t[dimN];
  Double_t* max = new Double_t[dimN];
  for (int i = 0; i < dimN; i++) {
    min[i] = GetNormMin(i);
    max[i] = GetNormMax(i);
  }
  HalHtmlTable table("", "haltable", "");
  HalHtmlRow row1("", "dark_blue", "");
  row1.AddContent(HalHtmlCell("label"));
  row1.AddContent(HalHtmlCellCol("value", 2));
  table.AddContent(row1);
  HalHtmlRow row2("", "dark_blue", "");
  row2.AddContent(HalHtmlCell("Name"));
  row2.AddContent(HalHtmlCellCol(GetName(), 2));
  table.AddContent(row2);
  HalHtmlRow row3("", "dark_blue", "");
  row3.AddContent(HalHtmlCell("Comment"));
  row3.AddContent(HalHtmlCellCol(fComment, 2));
  table.AddContent(row3);
  if (GetScale() == 0) {
    HalHtmlRow rowS("", "dark_blue", "");
    rowS.AddContent(HalHtmlCell("Scale"));
    HalHtmlCellCol cellscale("0(disabled)", 2);
    rowS.AddContent(cellscale);
    table.AddContent(rowS);
  } else {
  }
  for (int i = 0; i < dimN; i++) {
    TString label;
    switch (i) {
      case 0: label = "x"; break;
      case 1: label = "y"; break;
      default: label = "z"; break;
    }
    HalHtmlRow row4("", "dark_blue", "");
    row4.AddContent(HalHtmlCell(Form("Norm on %s axis", label.Data())));
    row4.AddContent(HalHtmlCell(Form("%4.2f", min[i])));
    row4.AddContent(HalHtmlCell(Form("%4.2f", max[i])));
    table.AddContent(row4);
  }
  if (fBinomial) {
    HalHtmlRow row5("", "dark_blue", "");
    row5.AddContent(HalHtmlCell("Binomial"));
    row5.AddContent(HalHtmlCellCol("kTRUE", 2));
    table.AddContent(row5);
  } else {
    HalHtmlRow row5("", "dark_blue", "");
    row5.AddContent(HalHtmlCell("Binomial"));
    row5.AddContent(HalHtmlCellCol("kFALSE", 2));
    table.AddContent(row5);
  }
  file.AddContent(table);
  HalHtmlTable table2("", "haltable", "");
  HalHtmlRow row2_1("", "dark_blue", "");
  row2_1.AddContent(HalHtmlCell("No."));
  row2_1.AddContent(HalHtmlCell("Name/Value"));
  table2.AddContent(row2_1);
  if (fLabels)  // for backward compatilibiyt
    for (int i = 0; i < fLabels->GetEntries(); i++) {
      TObjString* obj = (TObjString*) fLabels->At(i);
      HalHtmlRow singleRow("", "light_blue", "");
      singleRow.AddContent(HalHtmlCell(Form("%i", i)));
      singleRow.AddContent(HalHtmlCell(obj->GetString()));
      table2.AddContent(singleRow);
    }
  file.AddContent(table2);
  TString file_name = Form("%s/divided.root", path.Data());
  std::ifstream f(file_name.Data());
  if (f.good()) {
    file.AddStringContent(HalHtmlCore::GetJsDiv("divided.root", "canvas;1"));
  } else {
    file.AddStringContent(GetPic());
  }
  file.Save();
  return HalHtmlCore::GetUrl(Form("divided_%i/divided.html", counter), this->ClassName());
}

TString HalDividedHisto1D::GetLabel(Int_t i) const {
  TObjString* obj_str = (TObjString*) fLabels->At(i);
  if (obj_str == NULL) return "";
  return obj_str->GetString();
}

Long64_t HalDividedHisto1D::Merge(TCollection* collection) {
  if (collection) {
    HalDividedHisto1D* pack = NULL;
    TIter iterator(collection);
    while ((pack = (HalDividedHisto1D*) iterator())) {
      Add(pack);
    }
  }
  return 1;
}

void HalDividedHisto1D::Browse(TBrowser* b) {
  gPad->Clear();
  TVirtualPad* c1 = gPad;
  Draw();
  gPad = c1;
  b->Add(fNum);
  b->Add(fDen);
}

void HalDividedHisto1D::SetBinomial(Bool_t binomial) { fBinomial = binomial; }

void HalDividedHisto1D::Fold1D(Double_t val, Option_t* opt) {
  TString option = opt;
  if (fNum == NULL || fDen == NULL) {
    HalCout::PrintInfo("lack of numerator and/or denominator folding cannot be done", Hal::EInfo::kLessWarning);
    return;
  }
  TAxis* ax = NULL;
  if (HalStd::FindParam(option, "x")) {
    ax = fNum->GetXaxis();
  } else if (HalStd::FindParam(option, "y")) {
    ax = fNum->GetYaxis();
  } else if (HalStd::FindParam(option, "z")) {
    ax = fNum->GetZaxis();
  }
  Int_t bin     = ax->FindBin(val);
  Double_t cent = ax->GetBinCenter(bin);
  Double_t low  = ax->GetBinLowEdge(bin);
  if (TMath::Abs(cent - val) > 1E-10 && TMath::Abs(low - val) > 1E-10) {
    HalCout::PrintInfo("value of folding is not equal a bin center of bin edge, folding "
                       "cannot be done",
                       Hal::EInfo::kLessWarning);
    return;
  }
  Folding1D(val, opt);
}

void HalDividedHisto1D::Folding1D(Double_t val, Option_t* /*axis*/) {
  // TODO improve this
  TH1* num_c          = (TH1*) fNum->Clone();
  TH1* den_c          = (TH1*) fDen->Clone();
  Double_t min        = fNum->GetXaxis()->GetBinLowEdge(1);
  Double_t max        = fNum->GetXaxis()->GetBinUpEdge(fNum->GetNbinsX());
  Double_t period     = max - min;
  Double_t middle_bin = fNum->GetXaxis()->FindBin(val);
  middle_bin          = fNum->GetBinCenter(middle_bin);
  for (int i = 1; i <= fNum->GetNbinsX(); i++) {
    Double_t x    = fNum->GetBinCenter(i);
    Double_t op_x = 2.0 * val - x;
    if (op_x > max) op_x = op_x - period;
    if (op_x <= min) op_x = op_x + period;
    Int_t j           = fNum->FindBin(op_x);
    Double_t N1       = num_c->GetBinContent(i);
    Double_t NE1      = num_c->GetBinError(i);
    Double_t D1       = den_c->GetBinContent(i);
    Double_t DE1      = den_c->GetBinError(i);
    Double_t N2       = num_c->GetBinContent(j);
    Double_t NE2      = num_c->GetBinError(j);
    Double_t D2       = den_c->GetBinContent(j);
    Double_t DE2      = den_c->GetBinError(j);
    Double_t Ncontent = N1 + N2;
    Double_t Nerror   = TMath::Sqrt(NE1 * NE1 + NE2 * NE2);
    Double_t Dcontent = D1 + D2;
    Double_t Derror   = TMath::Sqrt(DE1 * DE1 + DE2 * DE2);
    fNum->SetBinContent(i, Ncontent);
    fNum->SetBinError(i, Nerror);
    fDen->SetBinContent(i, Dcontent);
    fDen->SetBinError(i, Derror);
  }
  delete num_c;
  delete den_c;
}

HalDividedHisto1D::~HalDividedHisto1D() {
  if (fNum && fNumIsCloned) { delete fNum; }
  if (fDen && fDenIsCloned) { delete fDen; }
  if (fNormMin) delete[] fNormMin;
  if (fNormMax) delete[] fNormMax;
  if (fLabels) delete fLabels;
}

void HalDividedHisto2D::Normalize(TH1* h) const {
  if (fScale == 0) {
    for (int i = 0; i < fDim; i++) {
      Double_t scale = 1.0;
      if (fNormMin[i] == fNormMax[i]) {
        HalCout::PrintInfo(Form("Same norm min and norm max on %i axis", i), Hal::EInfo::kLessWarning);
      } else {
        Int_t binMinX = fNum->GetXaxis()->FindBin(fNormMin[0]);
        Int_t binMaxX = fNum->GetXaxis()->FindBin(fNormMax[0]);
        Int_t binMinY = fNum->GetYaxis()->FindBin(fNormMin[1]);
        Int_t binMaxY = fNum->GetYaxis()->FindBin(fNormMax[1]);
        Double_t num  = ((TH2*) fNum)->Integral(binMinX, binMaxX, binMinY, binMaxY);
        binMinX       = fDen->GetXaxis()->FindBin(fNormMin[0]);
        binMaxX       = fDen->GetXaxis()->FindBin(fNormMax[0]);
        binMinY       = fDen->GetYaxis()->FindBin(fNormMin[1]);
        binMaxY       = fDen->GetYaxis()->FindBin(fNormMax[1]);
        Double_t den  = ((TH2*) fDen)->Integral(binMinX, binMaxX, binMinY, binMaxY);
        if (den == 0) {
          HalCout::PrintInfo("Numerator integral is zero, set scale to one", Hal::EInfo::kLessWarning);
        } else {
          scale = den / num;
        }
      }
      h->Scale(scale);
    }
  } else {
    h->Scale(fScale);
  }
}

HalDividedHisto2D::HalDividedHisto2D() : HalDividedHisto1D() {}

TString HalDividedHisto2D::HTMLExtract(Int_t counter, TString dir) const {
  TString path = Form("%s/divided_%i", dir.Data(), counter);
  gSystem->MakeDirectory(path);
  Bool_t batch = gROOT->IsBatch();
  gROOT->SetBatch(kTRUE);
  TCanvas* c1 = new TCanvas("canvas", "canvas", 0, 0, 1000, 1500);
  c1->Divide(2, 2);
  c1->cd(1);
  TH2* cf = (TH2*) GetHist(kTRUE);
  cf->Draw("colz");
  c1->cd(3);
  TH2* num = (TH2*) GetNum()->Clone();
  TH2* den = (TH2*) GetDen()->Clone();
  den->Draw("colz");
  c1->cd(4);
  num->Draw("colz");
  c1->cd(2);
  c1->SaveAs(Form("%s/divided_%i/divided.root", dir.Data(), counter));
  // create text file
  gROOT->SetBatch(batch);
  TString page = CommonExtract(counter, dir);
  return page;
}

void HalDividedHisto2D::SetHistoName(TH1* h) const { h->GetZaxis()->SetName(fAxisName); }

HalDividedHisto2D& HalDividedHisto2D::operator=(const HalDividedHisto2D& other) {
  HalDividedHisto1D::operator=(other);
  return *this;
}

void HalDividedHisto2D::Folding1D(Double_t val, Option_t* axis) {
  TAxis *X, *Y;
  TString opt = axis;
  Int_t order = 0;
  if (HalStd::FindParam(opt, "x")) {
    X = fNum->GetXaxis();
    Y = fNum->GetYaxis();
  } else {
    order = 1;
    Y     = fNum->GetXaxis();
    X     = fNum->GetYaxis();
  }
  TH2* num_c          = (TH2*) fNum->Clone();
  TH2* den_c          = (TH2*) fDen->Clone();
  Double_t min        = X->GetBinLowEdge(1);
  Double_t max        = X->GetBinUpEdge(X->GetNbins());
  Double_t period     = max - min;
  Double_t middle_bin = X->FindBin(val);
  middle_bin          = X->GetBinCenter(middle_bin);
  for (int i = 1; i <= X->GetNbins(); i++) {
    Double_t x    = X->GetBinCenter(i);
    Double_t op_x = 2.0 * val - x;
    if (op_x > max) op_x = op_x - period;
    if (op_x <= min) op_x = op_x + period;
    Int_t j = X->FindBin(op_x);
    for (int k = 1; k <= Y->GetNbins(); k++) {
      Int_t a, b, c, d;
      if (order == 1) {  // around y-axis
        a = k;
        b = i;
        c = k;
        d = j;
      } else {
        a = i;
        b = k;
        c = j;
        d = k;
      }
      Double_t N1       = num_c->GetBinContent(a, b);
      Double_t NE1      = num_c->GetBinError(a, b);
      Double_t D1       = den_c->GetBinContent(a, b);
      Double_t DE1      = den_c->GetBinError(a, b);
      Double_t N2       = num_c->GetBinContent(c, d);
      Double_t NE2      = num_c->GetBinError(c, d);
      Double_t D2       = den_c->GetBinContent(c, d);
      Double_t DE2      = den_c->GetBinError(c, d);
      Double_t Ncontent = N1 + N2;
      Double_t Nerror   = TMath::Sqrt(NE1 * NE1 + NE2 * NE2);
      Double_t Dcontent = D1 + D2;
      Double_t Derror   = TMath::Sqrt(DE1 * DE1 + DE2 * DE2);
      fNum->SetBinContent(a, b, Ncontent);
      fNum->SetBinError(a, b, Nerror);
      fDen->SetBinContent(a, b, Dcontent);
      fDen->SetBinError(a, b, Derror);
    }
  }
  delete num_c;
  delete den_c;
}

void HalDividedHisto2D::Folding2D(Double_t valX, Double_t valY, Option_t* opt) {
  TString option = opt;
  if (option.Length() == 1) {
    Fold1D(valX, "x");
    Fold1D(valY, "y");
    return;
  }
  TH2* num_c       = (TH2*) fNum->Clone();
  TH2* den_c       = (TH2*) fDen->Clone();
  Double_t minX    = fNum->GetXaxis()->GetBinLowEdge(1);
  Double_t maxX    = fNum->GetXaxis()->GetBinUpEdge(fNum->GetNbinsX());
  Double_t minY    = fNum->GetYaxis()->GetBinLowEdge(1);
  Double_t maxY    = fNum->GetYaxis()->GetBinUpEdge(fNum->GetNbinsY());
  Double_t periodX = maxX - minX;
  Double_t periodY = maxY - minY;
  for (int i = 1; i <= fNum->GetNbinsX(); i++) {
    for (int j = 1; j <= fNum->GetNbinsY(); j++) {
      Double_t x    = fNum->GetXaxis()->GetBinCenter(i);
      Double_t y    = fNum->GetYaxis()->GetBinCenter(j);
      Double_t op_x = 2.0 * valX - x;
      Double_t op_y = 2.0 * valY - y;
      if (op_x > maxX) op_x = op_x - periodX;
      if (op_x <= minX) op_x = op_x + periodX;
      if (op_y > maxY) op_y = op_y - periodY;
      if (op_y <= minY) op_y = op_y + periodY;
      Int_t k           = fNum->GetXaxis()->FindBin(op_x);
      Int_t l           = fNum->GetYaxis()->FindBin(op_y);
      Double_t N1       = num_c->GetBinContent(i, j);
      Double_t NE1      = num_c->GetBinError(i, j);
      Double_t D1       = den_c->GetBinContent(i, j);
      Double_t DE1      = den_c->GetBinError(i, j);
      Double_t N2       = num_c->GetBinContent(k, l);
      Double_t NE2      = num_c->GetBinError(k, l);
      Double_t D2       = den_c->GetBinContent(k, l);
      Double_t DE2      = den_c->GetBinError(k, l);
      Double_t Ncontent = N1 + N2;
      Double_t Nerror   = TMath::Sqrt(NE1 * NE1 + NE2 * NE2);
      Double_t Dcontent = D1 + D2;
      Double_t Derror   = TMath::Sqrt(DE1 * DE1 + DE2 * DE2);
      fNum->SetBinContent(i, j, Ncontent);
      fNum->SetBinError(i, j, Nerror);
      fDen->SetBinContent(i, j, Dcontent);
      fDen->SetBinError(i, j, Derror);
    }
  }
  delete num_c;
  delete den_c;
}

void HalDividedHisto2D::Fold2D(Double_t valX, Double_t valY, Option_t* opt) {
  TString option = opt;
  if (fNum == NULL || fDen == NULL) {
    HalCout::PrintInfo("lack of numerator and/or denominator folding cannot be done", Hal::EInfo::kLessWarning);
    return;
  }
  TAxis* ax = NULL;
  TAxis* ay = NULL;
  if (option.EqualTo("yx")) {
    Double_t temp = valX;
    valX          = valY;
    valY          = temp;
    option        = "xy";
    ax            = fNum->GetXaxis();
    ay            = fNum->GetYaxis();
  }
  if (option.EqualTo("zy")) {
    Double_t temp = valX;
    valX          = valY;
    valY          = temp;
    option        = "yz";
    ax            = fNum->GetYaxis();
    ay            = fNum->GetZaxis();
  }
  if (option.EqualTo("zx")) {
    Double_t temp = valX;
    valX          = valY;
    valY          = temp;
    option        = "xz";
    ax            = fNum->GetXaxis();
    ay            = fNum->GetZaxis();
  }
  if (option.EqualTo("x")) {
    ax = fNum->GetYaxis();
    ay = fNum->GetZaxis();
  }
  if (option.EqualTo("y")) {
    ax = fNum->GetXaxis();
    ay = fNum->GetZaxis();
  }
  if (option.EqualTo("z")) {
    ax = fNum->GetXaxis();
    ay = fNum->GetYaxis();
  }
  if (option.EqualTo("xy")) {
    ax = fNum->GetXaxis();
    ay = fNum->GetYaxis();
  }
  if (option.EqualTo("yz")) {
    ax = fNum->GetYaxis();
    ay = fNum->GetZaxis();
  }
  if (option.EqualTo("xz")) {
    ax = fNum->GetXaxis();
    ay = fNum->GetZaxis();
  }
  if (ax == NULL || ay == NULL) {
    HalCout::PrintInfo("Wrong option in 2D folding", Hal::EInfo::kLessWarning);
    return;
  }
  Int_t binX     = ax->FindBin(valX);
  Int_t binY     = ay->FindBin(valY);
  Double_t centX = ax->GetBinCenter(binX);
  Double_t lowX  = ax->GetBinLowEdge(binX);
  Double_t centY = ay->GetBinCenter(binY);
  Double_t lowY  = ay->GetBinLowEdge(binY);
  if (TMath::Abs(centX - valX) > 1E-10 && TMath::Abs(lowX - valX) > 1E-10) {
    HalCout::PrintInfo("value of folding on X axis is not equal a bin center of bin edge, "
                       "folding cannot be done",
                       Hal::EInfo::kLessWarning);
    return;
  }
  if (TMath::Abs(centY - valY) > 1E-10 && TMath::Abs(lowY - valY) > 1E-10) {
    HalCout::PrintInfo("value of folding on Y axis is not equal a bin center of bin edge, "
                       "folding cannot be done",
                       Hal::EInfo::kLessWarning);
    return;
  }
  Folding2D(valX, valY, option);
}

void HalDividedHisto2D::Browse(TBrowser* b) {
  gPad->Clear();
  TVirtualPad* c1 = gPad;
  if (!this->InheritsFrom("HalDividedHisto3D")) {  // not 3d
    Draw("all+colz");
  } else {
    Draw();
  }
  gPad = c1;
  b->Add(fNum);
  b->Add(fDen);
}

HalDividedHisto2D::~HalDividedHisto2D() {}

void HalDividedHisto3D::Normalize(TH1* h) const {
  if (fScale == 0) {
    for (int i = 0; i < fDim; i++) {
      Double_t scale = 1.0;
      if (fNormMin[i] == fNormMax[i]) {
        HalCout::PrintInfo(Form("Same norm min and norm max on %i axis", i), Hal::EInfo::kLessWarning);
        scale = 1.0;
      } else {
        Int_t binMinX = fNum->GetXaxis()->FindBin(fNormMin[0]);
        Int_t binMaxX = fNum->GetXaxis()->FindBin(fNormMax[0]);
        Int_t binMinY = fNum->GetYaxis()->FindBin(fNormMin[1]);
        Int_t binMaxY = fNum->GetYaxis()->FindBin(fNormMax[1]);
        Int_t binMinZ = fNum->GetZaxis()->FindBin(fNormMin[2]);
        Int_t binMaxZ = fNum->GetZaxis()->FindBin(fNormMax[2]);

        Double_t num = ((TH3*) fNum)->Integral(binMinX, binMaxX, binMinY, binMaxY, binMinZ, binMaxZ);

        binMinX = fDen->GetXaxis()->FindBin(fNormMin[0]);
        binMaxX = fDen->GetXaxis()->FindBin(fNormMax[0]);
        binMinY = fDen->GetYaxis()->FindBin(fNormMin[1]);
        binMaxY = fDen->GetYaxis()->FindBin(fNormMax[1]);
        binMinZ = fDen->GetZaxis()->FindBin(fNormMin[2]);
        binMaxZ = fDen->GetZaxis()->FindBin(fNormMax[2]);

        Double_t den = ((TH3*) fDen)->Integral(binMinX, binMaxX, binMinY, binMaxY, binMinZ, binMaxZ);

        if (den == 0) {
          HalCout::PrintInfo("Numerator integral is zero, set scale to one", Hal::EInfo::kLessWarning);
          scale = 1.0;
        } else {
          scale = den / num;
        }
      }
      h->Scale(scale);
    }
  } else {
    h->Scale(fScale);
  }
}

HalDividedHisto3D::HalDividedHisto3D() : HalDividedHisto2D() {}

TString HalDividedHisto3D::HTMLExtract(Int_t counter, TString dir) const {
  TString path = Form("%s/divided_%i", dir.Data(), counter);
  gSystem->MakeDirectory(path);
  TString page = CommonExtract(counter, dir);
  Bool_t batch = gROOT->IsBatch();
  gROOT->SetBatch(kTRUE);
  TString name = "Divided 1D";
  TCanvas* c1  = new TCanvas(name, name, 0, 0, 800, 600);
  c1->Divide(2, 3);
  c1->cd(1);
  TH3* num = (TH3*) GetNum();
  num->Draw(Draw_3D_option);
  c1->cd(2);
  // c1->GetPad(2)->SetLogy(1);
  num->Draw(Draw_3D_option);
  c1->cd(3);
  TH3* den = (TH3*) GetDen();
  den->Draw(Draw_3D_option);
  c1->cd(4);
  // c1->GetPad(4)->SetLogy(1);
  den->Draw(Draw_3D_option);
  c1->cd(5);
  TH3* fun = (TH3*) GetHist(kTRUE);
  fun->Draw(Draw_3D_option);
  c1->cd(6);
  // c1->GetPad(6)->SetLogy(1);
  fun->Draw(Draw_3D_option);
  c1->SaveAs(Form("%s/divided_%i/divided.png", dir.Data(), counter));
  gROOT->SetBatch(batch);
  return page;
}

TH2D* HalDividedHisto3D::GetProjection2D(Double_t min, Double_t max, Option_t* opt) const {
  TString option = opt;
  TH3* histo     = NULL;
  if (HalStd::FindParam(option, "num", kTRUE)) {
    histo = (TH3*) GetNum()->Clone();
  } else if (HalStd::FindParam(option, "den", kTRUE)) {
    histo = (TH3*) GetDen()->Clone();
  } else {
    histo = (TH3*) GetHist(kTRUE);
  }
  TH2D* proj = HalStd::GetProjection2D(histo, min, max, opt);
  delete histo;
  return proj;
}

HalDividedHisto3D& HalDividedHisto3D::operator=(const HalDividedHisto3D& other) {
  HalDividedHisto2D::operator=(other);
  return *this;
}

void HalDividedHisto3D::Folding1D(Double_t val, Option_t* axis) {
  TAxis *X, *Y, *Z;
  TString opt = axis;
  Int_t order = 0;
  if (HalStd::FindParam(opt, "x")) {
    X = fNum->GetXaxis();
    Y = fNum->GetYaxis();
    Z = fNum->GetZaxis();
  } else if (HalStd::FindParam(opt, "y")) {
    order = 1;
    Y     = fNum->GetXaxis();
    X     = fNum->GetYaxis();
    Z     = fNum->GetZaxis();
  } else if (HalStd::FindParam(opt, "z")) {
    order = 2;
    X     = fNum->GetZaxis();
    Y     = fNum->GetYaxis();
    Z     = fNum->GetXaxis();
  } else {
    X = Y = Z = NULL;
    HalCout::PrintInfo("Wrong option in  HalDividedHisto3D Folding", Hal::EInfo::kLessWarning);
  }
  Double_t middle_bin = X->FindBin(val);
  middle_bin          = X->GetBinCenter(middle_bin);
  TH3* num_c          = (TH3*) fNum->Clone();
  TH3* den_c          = (TH3*) fDen->Clone();
  Double_t min        = X->GetBinLowEdge(1);
  Double_t max        = X->GetBinUpEdge(fNum->GetNbinsX());
  Double_t period     = max - min;
  for (int i = 1; i <= X->GetNbins(); i++) {
    Double_t x    = X->GetBinCenter(i);
    Double_t op_x = 2.0 * val - x;
    if (op_x > max) op_x = op_x - period;
    if (op_x <= min) op_x = op_x + period;
    Int_t j = X->FindBin(op_x);
    for (int k = 1; k <= Y->GetNbins(); k++) {
      for (int l = 1; l <= Z->GetNbins(); l++) {
        Int_t a, b, c, d, e, f;
        if (order == 0) {
          a = i;
          b = k;
          c = l;
          d = j;
          e = k;
          f = l;
        } else if (order == 1) {
          a = k;
          b = i;
          c = l;
          d = k;
          e = j;
          f = l;
        } else {
          a = k;
          b = l;
          c = i;
          d = k;
          e = l;
          f = j;
        }
        Double_t N1       = num_c->GetBinContent(a, b, c);
        Double_t NE1      = num_c->GetBinError(a, b, c);
        Double_t D1       = den_c->GetBinContent(a, b, c);
        Double_t DE1      = den_c->GetBinError(a, b, c);
        Double_t N2       = num_c->GetBinContent(d, e, f);
        Double_t NE2      = num_c->GetBinError(d, e, f);
        Double_t D2       = den_c->GetBinContent(d, e, f);
        Double_t DE2      = den_c->GetBinError(d, e, f);
        Double_t Ncontent = N1 + N2;
        Double_t Nerror   = TMath::Sqrt(NE1 * NE1 + NE2 * NE2);
        Double_t Dcontent = D1 + D2;
        Double_t Derror   = TMath::Sqrt(DE1 * DE1 + DE2 * DE2);
        fNum->SetBinContent(a, b, c, Ncontent);
        fNum->SetBinError(a, b, c, Nerror);
        fDen->SetBinContent(a, b, c, Dcontent);
        fDen->SetBinError(a, b, c, Derror);
      }
    }
  }
  delete num_c;
  delete den_c;
}

void HalDividedHisto3D::Folding2D(Double_t valX, Double_t valY, Option_t* opt) {
  TAxis *X, *Y, *Z;
  Int_t order    = 0;
  X              = fNum->GetXaxis();
  Y              = fNum->GetYaxis();
  Z              = fNum->GetZaxis();
  TString option = opt;
  if (option.EqualTo("xy")) {
    order = 1;
  } else if (option.EqualTo("xz")) {
    order = 2;
  } else if (option.EqualTo("yz")) {
    order = 3;
  } else if (option.EqualTo("x")) {
    Fold1D(valX, "y");
    Fold1D(valY, "z");
    return;
  } else if (option.EqualTo("y")) {
    Fold1D(valX, "x");
    Fold1D(valY, "z");
    return;
  } else if (option.EqualTo("z")) {
    Fold1D(valX, "x");
    Fold1D(valY, "y");
    return;
  } else {
    return;
  }
  switch (opt[0]) {
    case 'x': X = fNum->GetXaxis(); break;
    case 'y': X = fNum->GetYaxis(); break;
    case 'z': X = fNum->GetZaxis(); break;
  }
  switch (opt[1]) {
    case 'y': Y = fNum->GetYaxis(); break;
    case 'z': Y = fNum->GetZaxis(); break;
  }
  TH3* num_c       = (TH3*) fNum->Clone();
  TH3* den_c       = (TH3*) fDen->Clone();
  Double_t minX    = X->GetBinLowEdge(1);
  Double_t maxX    = X->GetBinUpEdge(X->GetNbins());
  Double_t periodX = maxX - minX;
  Double_t minY    = Y->GetBinLowEdge(1);
  Double_t maxY    = Y->GetBinUpEdge(Y->GetNbins());
  Double_t periodY = maxY - minY;
  for (int i = 1; i <= X->GetNbins(); i++) {
    Double_t x    = X->GetBinCenter(i);
    Double_t op_x = 2.0 * valX - x;
    if (op_x > maxX) op_x = op_x - periodX;
    if (op_x <= minX) op_x = op_x + periodX;
    Int_t u = X->FindBin(op_x);
    for (int j = 1; j <= Y->GetNbins(); j++) {
      Double_t y    = Y->GetBinCenter(j);
      Double_t op_y = 2.0 * valY - y;
      if (op_y > maxY) op_y = op_y - periodY;
      if (op_y <= minY) op_y = op_y + periodY;
      Int_t v = Y->FindBin(op_y);
      for (int k = 1; k <= Z->GetNbins(); k++) {
        Int_t a, b, c, d, e, f;
        switch (order) {
          case 1: {  // xy
            a = i;
            b = j;
            c = k;

            d = u;
            e = v;
            f = k;
          } break;
          case 2: {  // xz
            a = i;
            b = k;
            c = j;

            d = u;
            e = k;
            f = v;
          } break;
          case 3: {  // yz
            a = k;
            b = i;
            c = j;

            d = k;
            e = u;
            f = v;
          } break;
        }
        Double_t N1  = num_c->GetBinContent(a, b, c);
        Double_t NE1 = num_c->GetBinError(a, b, c);
        Double_t D1  = den_c->GetBinContent(a, b, c);
        Double_t DE1 = den_c->GetBinError(a, b, c);
        Double_t N2  = num_c->GetBinContent(d, e, f);
        Double_t NE2 = num_c->GetBinError(d, e, f);
        Double_t D2  = den_c->GetBinContent(d, e, f);
        Double_t DE2 = den_c->GetBinError(d, e, f);

        Double_t Ncontent = N1 + N2;
        Double_t Nerror   = TMath::Sqrt(NE1 * NE1 + NE2 * NE2);
        Double_t Dcontent = D1 + D2;
        Double_t Derror   = TMath::Sqrt(DE1 * DE1 + DE2 * DE2);
        fNum->SetBinContent(a, b, c, Ncontent);
        fNum->SetBinError(a, b, c, Nerror);
        fDen->SetBinContent(a, b, c, Dcontent);
        fDen->SetBinError(a, b, c, Derror);
      }
    }
  }
  delete num_c;
  delete den_c;
}

void HalDividedHisto3D::Fold3D(Double_t valX, Double_t valY, Double_t valZ, Option_t* opt) {
  TString option = opt;
  if (option.EqualTo("xyz")) {
    TH3* num_c       = (TH3*) fNum->Clone();
    TH3* den_c       = (TH3*) fDen->Clone();
    TAxis* X         = fNum->GetXaxis();
    TAxis* Y         = fNum->GetYaxis();
    TAxis* Z         = fNum->GetZaxis();
    Double_t minX    = X->GetBinLowEdge(1);
    Double_t maxX    = X->GetBinUpEdge(X->GetNbins());
    Double_t periodX = maxX - minX;
    Double_t minY    = Y->GetBinLowEdge(1);
    Double_t maxY    = Y->GetBinUpEdge(Y->GetNbins());
    Double_t periodY = maxY - minY;
    Double_t minZ    = Z->GetBinLowEdge(1);
    Double_t maxZ    = Z->GetBinUpEdge(Z->GetNbins());
    Double_t periodZ = maxZ - minZ;
    for (int i = 1; i <= X->GetNbins(); i++) {
      Double_t x    = X->GetBinCenter(i);
      Double_t op_x = 2.0 * valX - x;
      if (op_x > maxX) op_x = op_x - periodX;
      if (op_x <= minX) op_x = op_x + periodX;
      Int_t u = X->FindBin(op_x);
      for (int j = 1; j <= Y->GetNbins(); j++) {
        Double_t y    = Y->GetBinCenter(j);
        Double_t op_y = 2.0 * valY - y;
        if (op_y > maxY) op_y = op_y - periodY;
        if (op_y <= minY) op_y = op_y + periodY;
        Int_t v = Y->FindBin(op_y);
        for (int k = 1; k <= Z->GetNbins(); k++) {
          Double_t z    = Z->GetBinCenter(k);
          Double_t op_z = 2.0 * valZ - z;
          if (op_z > maxZ) op_z = op_z - periodZ;
          if (op_z <= minZ) op_z = op_z + periodZ;
          Int_t w = Z->FindBin(op_z);

          Double_t N1  = num_c->GetBinContent(i, j, k);
          Double_t NE1 = num_c->GetBinError(i, j, k);
          Double_t D1  = den_c->GetBinContent(i, j, k);
          Double_t DE1 = den_c->GetBinError(i, j, k);
          Double_t N2  = num_c->GetBinContent(u, v, w);
          Double_t NE2 = num_c->GetBinError(u, v, w);
          Double_t D2  = den_c->GetBinContent(u, v, w);
          Double_t DE2 = den_c->GetBinError(u, v, w);

          Double_t Ncontent = N1 + N2;
          Double_t Nerror   = TMath::Sqrt(NE1 * NE1 + NE2 * NE2);
          Double_t Dcontent = D1 + D2;
          Double_t Derror   = TMath::Sqrt(DE1 * DE1 + DE2 * DE2);
          fNum->SetBinContent(i, j, k, Ncontent);
          fNum->SetBinError(i, j, k, Nerror);
          fDen->SetBinContent(i, j, k, Dcontent);
          fDen->SetBinError(i, j, k, Derror);
        }
      }
    }
    delete num_c;
    delete den_c;
  } else {
    Fold1D(valX, "x");
    Fold1D(valY, "y");
    Fold1D(valZ, "z");
  }
}

HalDividedHisto3D::~HalDividedHisto3D() {}

void HalDividedHisto3D::SetHistoName(TH1* /*h*/) const {}

void HalDividedHisto1D::SetComment(TString comment) { fComment = comment; }

void HalDividedHisto1D::Rebin(Int_t ngroup, Option_t* opt) {
  TString option   = opt;
  Bool_t num_rebin = HalStd::FindParam(option, "num");
  Bool_t den_rebin = HalStd::FindParam(option, "den");
  if ((!num_rebin) && (!den_rebin)) {
    num_rebin = kTRUE;
    den_rebin = kTRUE;
  }
  if (num_rebin && fNum != NULL) { fNum->Rebin(ngroup); }
  if (den_rebin && fDen != NULL) { fDen->Rebin(ngroup); }
}

void HalDividedHisto2D::Rebin(Int_t ngroup, Option_t* opt) {
  TString option   = opt;
  Bool_t num_rebin = HalStd::FindParam(option, "num");
  Bool_t den_rebin = HalStd::FindParam(option, "den");
  Bool_t rebin_x   = kFALSE;
  Bool_t rebin_y   = kFALSE;
  if (HalStd::FindParam(option, "y")) {
    rebin_y = kTRUE;
  } else {
    rebin_x = kTRUE;
  }
  if ((!num_rebin) && (!den_rebin)) {
    num_rebin = kTRUE;
    den_rebin = kTRUE;
  }
  if (num_rebin && fNum != NULL) {
    if (rebin_x) ((TH2*) fNum)->RebinX(ngroup);
    if (rebin_y) ((TH2*) fNum)->RebinY(ngroup);
  }
  if (den_rebin && fDen != NULL) {
    if (rebin_x) ((TH2*) fDen)->RebinX(ngroup);
    if (rebin_y) ((TH2*) fDen)->RebinY(ngroup);
  }
}

void HalDividedHisto3D::Rebin(Int_t ngroup, Option_t* opt) {
  TString option   = opt;
  Bool_t num_rebin = HalStd::FindParam(option, "num");
  Bool_t den_rebin = HalStd::FindParam(option, "den");
  Bool_t rebin_x   = kFALSE;
  Bool_t rebin_y   = kFALSE;
  Bool_t rebin_z   = kFALSE;
  if (option.Contains("x") || option.Contains("X")) { rebin_x = kTRUE; }
  if (option.Contains("y") || option.Contains("Y")) { rebin_y = kTRUE; }
  if (option.Contains("z") || option.Contains("Z")) { rebin_z = kTRUE; }
  if (rebin_y == kFALSE && rebin_z == kFALSE) { rebin_x = kTRUE; }
  if ((!num_rebin) && (!den_rebin)) {
    num_rebin = kTRUE;
    den_rebin = kTRUE;
  }
  if (num_rebin && fNum != NULL) {
    if (rebin_x) ((TH3*) fNum)->RebinX(ngroup);
    if (rebin_y) ((TH3*) fNum)->RebinY(ngroup);
    if (rebin_z) ((TH3*) fNum)->RebinZ(ngroup);
  }
  if (den_rebin && fDen != NULL) {
    if (rebin_x) ((TH3*) fDen)->RebinX(ngroup);
    if (rebin_y) ((TH3*) fDen)->RebinY(ngroup);
    if (rebin_z) ((TH3*) fDen)->RebinZ(ngroup);
  }
}

void HalDividedHisto1D::AddLabel(TString label) { fLabels->AddLast(new TObjString(label)); }

HalDividedHisto1D& HalDividedHisto1D::operator=(const HalDividedHisto1D& other) {
  if (this != &other) {
    if (fNum) delete fNum;
    if (fDen) delete fDen;
    if (other.fNum != NULL) fNum = (TH1*) other.fNum->Clone();
    if (other.fDen != NULL) fDen = (TH1*) other.fDen->Clone();
    for (int i = 0; i < fDim; i++) {
      fNormMin[i] = other.fNormMin[i];
      fNormMax[i] = other.fNormMax[i];
    }
    fScale    = other.fScale;
    fAxisName = other.fAxisName;
    fComment  = other.fComment;
    if (fLabels) {
      delete fLabels;
      fLabels = NULL;
    }
    if (other.fLabels) fLabels = (TList*) other.fLabels->Clone();
  }
  return *this;
}

void HalDividedHisto1D::Draw(Option_t* opt) {
  if (gPad == NULL) new TCanvas();
  TString option = opt;
  if (HalStd::FindParam(option, "num", kTRUE)) {
    fNum->Draw(option);
  } else if (HalStd::FindParam(option, "den", kTRUE)) {
    fDen->Draw(option);
  } else if (HalStd::FindParam(option, "all", kTRUE)) {
    TVirtualPad* c1 = gPad;
    if (gPad->GetListOfPrimitives()->GetEntries() < 4) gPad->Divide(2, 2);
    c1->cd(1);
    fNum->Draw(option);
    c1->cd(2);
    fDen->Draw(option);
    c1->cd(3);
    TH1* h = this->GetHist(kTRUE);
    h->SetTitle(fComment);
    h->Draw(option);
    c1->cd(4);
    TH1* num_copy = (TH1*) fNum->Clone("temp_num");
    TH1* den_copy = (TH1*) fDen->Clone("temp_den");
    num_copy->GetYaxis()->SetTitle(Form("%s (scaled)", fNum->GetYaxis()->GetTitle()));
    num_copy->SetLineColor(kRed + 1);
    den_copy->SetLineColor(kGreen + 1);
    Double_t num_scale = 1.0 / num_copy->GetEntries();
    Double_t den_scale = 1.0 / den_copy->GetEntries();
    num_copy->Scale(num_scale);
    den_copy->Scale(den_scale);
    num_copy->SetStats(kFALSE);
    TLegend* leg = new TLegend(0.4, 0.8, 0.9, 0.9);
    leg->AddEntry(num_copy, Form("Numerator x %E", num_scale), "LPM");
    leg->AddEntry(den_copy, Form("Denominator x %E", den_scale), "LPM");
    num_copy->Draw(option);
    den_copy->Draw("SAME");
    leg->SetFillStyle(3002);
    leg->Draw("SAME");
    gPad = c1;
  } else {
    GetHist(kTRUE)->Draw(opt);
  }
}

Int_t HalDividedHisto1D::GetLabelsNo() const {
  if (fLabels == NULL)
    return 0;
  else
    return fLabels->GetEntries();
}

TString HalDividedHisto1D::GetPic() const {
  return Form("<img border=\"0\" src=\"divided.png\"   width=\"996\" height=\"1472\">");
}

HalDividedHisto1D::HalDividedHisto1D(TString name, Int_t nbins, Double_t min, Double_t max, Char_t type) :
  HalDividedHisto1D(name, 1) {
  std::vector<TString> out;
  out = HalStd::ExplodeString(name, ';');
  TString x_axis, y_axis;
  if (out.size() > 1) {
    name   = out[0];
    x_axis = out[1];
  }
  if (out.size() > 2) y_axis = out[2];
  switch (type) {
    case 'D':
      fNum = new TH1D(name + "_Num", name + "_Num", nbins, min, max);
      fDen = new TH1D(name + "_Den", name + "_Den", nbins, min, max);
      break;
    case 'I':
      fNum = new TH1I(name + "_Num", name + "_Num", nbins, min, max);
      fDen = new TH1I(name + "_Den", name + "_Den", nbins, min, max);
      break;
    case 'F':
      fNum = new TH1F(name + "_Num", name + "_Num", nbins, min, max);
      fDen = new TH1F(name + "_Den", name + "_Den", nbins, min, max);
      break;
    default:
      fNum = new TH1D(name + "_Num", name + "_Num", nbins, min, max);
      fDen = new TH1D(name + "_Den", name + "_Den", nbins, min, max);
      break;
  }
  if (out.size() > 1) {
    fNum->GetXaxis()->SetTitle(x_axis);
    fDen->GetXaxis()->SetTitle(x_axis);
  }
  if (out.size() > 2) {
    fNum->GetYaxis()->SetTitle(y_axis);
    fDen->GetYaxis()->SetTitle(y_axis);
  }
  fNumIsCloned = fDenIsCloned = kTRUE;
}

HalDividedHisto2D::HalDividedHisto2D(TString name,
                                     Int_t binsX,
                                     Double_t minX,
                                     Double_t maxX,
                                     Int_t binsY,
                                     Double_t minY,
                                     Double_t maxY,
                                     Char_t type) :
  HalDividedHisto2D(name, 2) {
  std::vector<TString> out = HalStd::ExplodeString(name, ';');
  TString x_axis, y_axis, z_axis;
  if (out.size() > 1) {
    name   = out[0];
    x_axis = out[1];
  }
  if (out.size() > 2) y_axis = out[2];
  if (out.size() > 3) z_axis = out[3];
  switch (type) {
    case 'D':
      fNum = new TH2D(name + "_Num", name + "_Num", binsX, minX, maxX, binsY, minY, maxY);
      fDen = new TH2D(name + "_Den", name + "_Den", binsX, minX, maxX, binsY, minY, maxY);
      break;
    case 'I':
      fNum = new TH2I(name + "_Num", name + "_Num", binsX, minX, maxX, binsY, minY, maxY);
      fDen = new TH2I(name + "_Den", name + "_Den", binsX, minX, maxX, binsY, minY, maxY);
      break;
    case 'F':
      fNum = new TH2F(name + "_Num", name + "_Num", binsX, minX, maxX, binsY, minY, maxY);
      fDen = new TH2F(name + "_Den", name + "_Den", binsX, minX, maxX, binsY, minY, maxY);
      break;
    default:
      fNum = new TH2D(name + "_Num", name + "_Num", binsX, minX, maxX, binsY, minY, maxY);
      fDen = new TH2D(name + "_Den", name + "_Den", binsX, minX, maxX, binsY, minY, maxY);
      break;
  }
  if (out.size() > 1) {
    fNum->GetXaxis()->SetTitle(x_axis);
    fDen->GetXaxis()->SetTitle(x_axis);
  }
  if (out.size() > 2) {
    fNum->GetYaxis()->SetTitle(y_axis);
    fDen->GetYaxis()->SetTitle(y_axis);
  }
  if (out.size() > 3) {
    fNum->GetZaxis()->SetTitle(z_axis);
    fDen->GetZaxis()->SetTitle(z_axis);
  }
  SetOwner(kTRUE);
}

HalDividedHisto3D::HalDividedHisto3D(TString name,
                                     Int_t binsX,
                                     Double_t minX,
                                     Double_t maxX,
                                     Int_t binsY,
                                     Double_t minY,
                                     Double_t maxY,
                                     Int_t binsZ,
                                     Double_t minZ,
                                     Double_t maxZ,
                                     Char_t type) :
  HalDividedHisto3D(name) {
  std::vector<TString> out = HalStd::ExplodeString(name, ';');
  TString x_axis, y_axis, z_axis;
  if (out.size() > 1) {
    name   = out[0];
    x_axis = out[1];
  }
  if (out.size() > 2) y_axis = out[2];
  if (out.size() > 3) z_axis = out[3];
  switch (type) {
    case 'D':
      fNum = new TH3D(name + "_Num", name + "_Num", binsX, minX, maxX, binsY, minY, maxY, binsZ, minZ, maxZ);
      fDen = new TH3D(name + "_Den", name + "_Den", binsX, minX, maxX, binsY, minY, maxY, binsZ, minZ, maxZ);
      break;
    case 'I':
      fNum = new TH3I(name + "_Num", name + "_Num", binsX, minX, maxX, binsY, minY, maxY, binsZ, minZ, maxZ);
      fDen = new TH3I(name + "_Den", name + "_Den", binsX, minX, maxX, binsY, minY, maxY, binsZ, minZ, maxZ);
      break;
    case 'F':
      fNum = new TH3F(name + "_Num", name + "_Num", binsX, minX, maxX, binsY, minY, maxY, binsZ, minZ, maxZ);
      fDen = new TH3F(name + "_Den", name + "_Den", binsX, minX, maxX, binsY, minY, maxY, binsZ, minZ, maxZ);
      break;
    default:
      fNum = new TH3D(name + "_Num", name + "_Num", binsX, minX, maxX, binsY, minY, maxY, binsZ, minZ, maxZ);
      fDen = new TH3D(name + "_Den", name + "_Den", binsX, minX, maxX, binsY, minY, maxY, binsZ, minZ, maxZ);
      break;
  }
  if (out.size() > 1) {
    fNum->GetXaxis()->SetTitle(x_axis);
    fDen->GetXaxis()->SetTitle(x_axis);
  }
  if (out.size() > 2) {
    fNum->GetYaxis()->SetTitle(y_axis);
    fDen->GetYaxis()->SetTitle(y_axis);
  }
  if (out.size() > 3) {
    fNum->GetZaxis()->SetTitle(z_axis);
    fDen->GetZaxis()->SetTitle(z_axis);
  }
  SetOwner(kTRUE);
}

HalDividedHisto2D::HalDividedHisto2D(const HalDividedHisto2D& other) : HalDividedHisto1D(other) {}

HalDividedHisto3D::HalDividedHisto3D(const HalDividedHisto3D& other) : HalDividedHisto2D(other) {}

Double_t HalDividedHisto1D::CalculateNorm(Double_t min, Double_t max) const {
  Int_t binA = fNum->GetXaxis()->FindBin(min);
  Int_t binB = fNum->GetXaxis()->FindBin(max);
  Double_t N = 0;
  Double_t D = 0;
  for (int i = binA; i <= binB; i++) {
    N += fNum->GetBinContent(i);
    D += fDen->GetBinContent(i);
  }
  return D / N;
}

Double_t HalDividedHisto2D::CalculateNorm(Double_t minX, Double_t maxX, Double_t minY, Double_t maxY) const {
  Int_t binXm = fNum->GetXaxis()->FindBin(minX);
  Int_t binXM = fNum->GetXaxis()->FindBin(maxX);
  Int_t binYm = fNum->GetYaxis()->FindBin(minY);
  Int_t binYM = fNum->GetYaxis()->FindBin(maxY);
  Double_t N  = 0;
  Double_t D  = 0;
  for (int i = binXm; i <= binXM; i++) {
    for (int j = binYm; j <= binYM; j++) {
      N += fNum->GetBinContent(i, j);
      D += fDen->GetBinContent(i, j);
    }
  }
  return D / N;
}

Double_t
HalDividedHisto3D::CalculateNorm(Double_t minX, Double_t maxX, Double_t minY, Double_t maxY, Double_t minZ, Double_t maxZ) const {
  Int_t binXm = fNum->GetXaxis()->FindBin(minX);
  Int_t binXM = fNum->GetXaxis()->FindBin(maxX);
  Int_t binYm = fNum->GetYaxis()->FindBin(minY);
  Int_t binYM = fNum->GetYaxis()->FindBin(maxY);
  Int_t binZm = fNum->GetZaxis()->FindBin(minZ);
  Int_t binZM = fNum->GetZaxis()->FindBin(maxZ);
  Double_t N  = 0;
  Double_t D  = 0;
  for (int i = binXm; i <= binXM; i++) {
    for (int j = binYm; j <= binYM; j++) {
      for (int k = binZm; k <= binZM; k++) {
        N += fNum->GetBinContent(i, j, k);
        D += fDen->GetBinContent(i, j, k);
      }
    }
  }
  return D / N;
}

TH1D* HalDividedHisto2D::Projection2DTo1D(Double_t min, Double_t max, Option_t* opt) const {
  TString option = opt;
  TH2* histo     = NULL;
  if (HalStd::FindParam(option, "num", kTRUE)) {
    histo = (TH2*) GetNum()->Clone();
  } else if (HalStd::FindParam(option, "den", kTRUE)) {
    histo = (TH2*) GetDen()->Clone();
  } else {
    histo = (TH2*) GetHist(kTRUE);
  }
  TH1D* projection = HalStd::GetProjection1D(histo, min, max, option);
  delete histo;
  return projection;
}

TH1D* HalDividedHisto3D::Projection3DTo1D(Double_t min1, Double_t max1, Double_t min2, Double_t max2, Option_t* opt) const {
  TString option = opt;
  TH3* histo     = NULL;
  if (HalStd::FindParam(option, "num", kTRUE)) {
    histo = (TH3*) GetNum()->Clone();
  } else if (HalStd::FindParam(option, "den", kTRUE)) {
    histo = (TH3*) GetDen()->Clone();
  } else if (HalStd::FindParam(option, "separate", kTRUE)) {
    TH1D* num1d = HalStd::GetProjection1D((TH3*) fNum, min1, max1, min2, max2, option);
    TH1D* den1d = HalStd::GetProjection1D((TH3*) fDen, min1, max1, min2, max2, option);
    num1d->Divide(den1d);
    delete den1d;
    return num1d;
  } else {
    histo = (TH3*) GetHist(kTRUE);
  }
  TH1D* projection = HalStd::GetProjection1D(histo, min1, max1, min2, max2, option);
  delete histo;
  return projection;
}

void HalDividedHisto1D::Print(Option_t* /*opt*/) const {
  HalCout::Text(this->ClassName(), "R", kWhite);
  if (GetNum() != nullptr) {
    const TAxis* x = GetNum()->GetXaxis();
    TString bins   = Form("Nbins: %i", x->GetNbins());
    TString min    = Form("Xmin:%4.3f", x->GetBinLowEdge(1));
    TString max    = Form("Xmax:%4.3f", x->GetBinUpEdge(x->GetNbins()));
    HalCout::Text(bins, "R", kWhite);
    HalCout::Text(min, "R", kWhite);
    HalCout::Text(max, "R", kWhite);
    if (fDim > 1) {
      const TAxis* y = GetNum()->GetYaxis();
      bins           = Form("Nbins: %i", y->GetNbins());
      min            = Form("Ymin:%4.3f", y->GetBinLowEdge(1));
      max            = Form("Ymax:%4.3f", y->GetBinUpEdge(y->GetNbins()));
      HalCout::Text(bins, "R", kWhite);
      HalCout::Text(min, "R", kWhite);
      HalCout::Text(max, "R", kWhite);
    }
    if (fDim > 2) {
      const TAxis* z = GetNum()->GetZaxis();
      bins           = Form("Nbins: %i", z->GetNbins());
      min            = Form("Zmin:%4.3f", z->GetBinLowEdge(1));
      max            = Form("Zmax:%4.3f", z->GetBinUpEdge(z->GetNbins()));
      HalCout::Text(bins, "R", kWhite);
      HalCout::Text(min, "R", kWhite);
      HalCout::Text(max, "R", kWhite);
    }
  }
}
