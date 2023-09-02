/*
 * FemtoCorrFunc2Db.cxx
 *
 *  Created on: 26 cze 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoCorrFunc2D.h"

#include "Cout.h"
#include "FemtoPair.h"
#include "HtmlCore.h"
#include "HtmlFile.h"
#include "HtmlTable.h"
#include "ObjectMatrix.h"
#include "Std.h"

#include <RtypesCore.h>
#include <TCanvas.h>
#include <TNamed.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>

namespace Hal {
  FemtoCorrFunc2D::FemtoCorrFunc2D(const DividedHisto1D* h, TString labelX, TString labelY, Int_t sizeX, Int_t sizeY) :
    FemtoCorrFunc(h, sizeX * sizeY), fLabelX(labelX), fLabelY(labelY) {
    fRangeX.MakeBigger(sizeX + 1);
    fRangeY.MakeBigger(sizeY + 1);
  }

  FemtoCorrFunc2D::FemtoCorrFunc2D(const DividedHisto1D& h, TString labelX, TString labelY, Int_t sizeX, Int_t sizeY) :
    FemtoCorrFunc(h, sizeX * sizeY), fLabelX(labelX), fLabelY(labelY) {
    fRangeX.MakeBigger(sizeX + 1);
    fRangeY.MakeBigger(sizeY + 1);
  }

  TString FemtoCorrFunc2D::HTMLExtract(Int_t counter, TString dir) const {
    TString path = Form("%s/corrfunc_%i", dir.Data(), counter);
    gSystem->MakeDirectory(path);
    TString filename = Form("%s/corrfunc.html", path.Data());
    HtmlFile file(filename, kFALSE);
    HtmlTable table("", "haltable", "");
    table.SetDefaultClass();
    HtmlRow row1;
    row1.AddContent(HtmlCell(Form("%s/%s", fLabelX.Data(), fLabelY.Data())));
    row1.SetClass(HtmlTableRowClass::DarkBlue());
    auto GetStringContent = [&](Double_t a, Double_t b, Char_t flag) {
      TString res = "";
      if (Angle(flag)) {
        res = ((Form("%4.2f - %4.2f&deg; ", a * TMath::RadToDeg(), b * TMath::RadToDeg())));
      } else {
        res = ((Form("%4.2f - %4.2f ", a, b)));
      }
      return res;
    };
    for (int i = 0; i < fRangeX.GetSize() - 1; i++) {
      HtmlCell cell;
      cell.SetStringContent(GetStringContent(fRangeX.Get(i), fRangeX.Get(i + 1), 'x'));
      cell.SetClass(Hal::HtmlTableRowClass::DarkBlue());
      row1.AddContent(cell);
    }
    table.AddContent(row1);
    const Int_t y_size = fRangeY.GetSize() - 1;
    const Int_t x_size = fRangeX.GetSize() - 1;
    for (int i = 0; i < y_size; i++) {
      HtmlRow row2;
      HtmlCell celltmp;
      celltmp.SetStringContent(GetStringContent(fRangeY.Get(i), fRangeY.Get(i + 1), 'y'));
      celltmp.SetClass(HtmlTableRowClass::DarkBlue());
      row2.AddContent(celltmp);
      for (int j = 0; j < x_size; j++) {
        Int_t pos         = i * x_size + j;
        DividedHisto1D* h = (DividedHisto1D*) fArray->At(pos);
        HtmlCell cell(h->HTMLExtract(pos, path));
        row2.AddContent(cell);
      }
      table.AddContent(row2);
    }
    HtmlRow row2("", "light_blue", "");
    row2.AddContent(HtmlCellCol("Comment:", 2));
    row2.AddContent(HtmlCellCol(HtmlCore::CheckBr(GetComment()), x_size - 1));
    table.AddContent(row2);
    file.AddContent(table);
    HtmlTable table2("", "haltable", "");
    table2.SetDefaultClass();
    HtmlRow row3;
    row3.SetClass(HtmlTableRowClass::DarkBlue());
    row3.AddSimpleCells({"Numerator", "Denominator"});
    table2.AddContent(row3);
    HtmlRow row4;
    row4.SetClass(HtmlTableRowClass::LightBlue());
    HtmlCell cell1, cell2;
    cell1.SetStringContent(HtmlCore::GetJsDiv("num.root", "Cnum;1", "colz", "dd1"));
    cell2.SetStringContent(HtmlCore::GetJsDiv("den.root", "Cden;1", "colz", "dd2"));
    Bool_t batch = gROOT->IsBatch();
    gStyle->SetOptStat(0);
    gROOT->SetBatch();
    TCanvas* cnum = new TCanvas("Cnum");
    fNumProp->DrawClone("colz");
    cnum->SaveAs(Form("%s/num.root", path.Data()));
    TCanvas* cden = new TCanvas("Cden");
    fDenProp->DrawClone("colz");
    cden->SaveAs(Form("%s/den.root", path.Data()));
    delete cnum, cden;
    gROOT->SetBatch(batch);
    row4.AddContent(cell1);
    row4.AddContent(cell2);
    table2.AddContent(row4);
    file.AddContent(table2);
    file.Save();
    return HtmlCore::GetUrl(Form("corrfunc_%i/corrfunc.html", counter), this->ClassName());
  }

  Bool_t FemtoCorrFunc2D::Check() {
    for (int i = 1; i < fRangeX.GetSize(); i++) {
      if (fRangeX[i] < fRangeX[i - 1]) {
        Cout::PrintInfo(Form("Wrong order in X-axis in %s", this->ClassName()), EInfo::kCriticalError);
        return kFALSE;
      }
    }
    for (int i = 1; i < fRangeY.GetSize(); i++) {
      if (fRangeY[i] < fRangeY[i - 1]) {
        Cout::PrintInfo(Form("Wrong order in Y-axis in %s", this->ClassName()), EInfo::kCriticalError);
        return kFALSE;
      }
    }
    Int_t x_bins = fRangeX.GetSize() - 1;
    for (int i = 0; i < x_bins; i++) {
      for (int j = 0; j < fRangeY.GetSize() - 1; j++) {
        DividedHisto1D* h = (DividedHisto1D*) fArray->At(i + j * x_bins);
        TString histname  = h->GetName();
        histname.ReplaceAll(Form("[%i]", i + j * x_bins), Form("[%i][%i]", i, j));
      }
    }
    return InitPropMon();
  }

  FemtoCorrFunc2D::~FemtoCorrFunc2D() {
    if (fNumProp) {
      delete fNumProp;
      delete fDenProp;
    }
  }

  DividedHisto1D* FemtoCorrFunc2D::GetCF(Int_t i, Int_t j) const {
    Int_t pos = i + j * (fRangeX.GetSize() - 1);
    { return (DividedHisto1D*) fArray->At(pos); };
  }

  FemtoCorrFunc::FemtoCorrFunc() : Object(), fArray(nullptr) {}

  void FemtoCorrFunc2D::FillNum(FemtoPair* pair) {
    auto par  = GetPairValNum(pair);
    Int_t bin = ConvertVal(par);
    if (bin < 0) return;
    fNumProp->Fill(par.first, par.second);
    ((DividedHisto1D*) fArray->At(bin))->FillNumObj(pair);
  }

  void FemtoCorrFunc2D::FillDenMixed(FemtoPair* pair) {
    auto par  = GetPairValDenMix(pair);
    Int_t bin = ConvertVal(par);
    if (bin < 0) return;
    fDenProp->Fill(par.first, par.second);
    ((DividedHisto1D*) fArray->At(bin))->FillDenObj(pair);
  }

  void FemtoCorrFunc2D::FillDenRotated(FemtoPair* pair) {
    auto par  = GetPairValDenMix(pair);
    Int_t bin = ConvertVal(par);
    if (bin < 0) return;
    fDenProp->Fill(par.first, par.second);
    ((DividedHisto1D*) fArray->At(bin))->FillDenObj(pair);
  }

  void FemtoCorrFunc2D::FillDenHemisphere(FemtoPair* pair) {
    auto par  = GetPairValDenHemi(pair);
    Int_t bin = ConvertVal(par);
    if (bin < 0) return;
    fDenProp->Fill(par.first, par.second);
    ((DividedHisto1D*) fArray->At(bin))->FillDenObj(pair);
  }

  void FemtoCorrFunc2D::FillDenCharged(Hal::FemtoPair* pair) {
    auto par  = GetPairValDenChar(pair);
    Int_t bin = ConvertVal(par);
    if (bin < 0) return;
    fDenProp->Fill(par.first, par.second);
    ((DividedHisto1D*) fArray->At(bin))->FillDenObj(pair);
  }

  void FemtoCorrFunc2D::FillDenPerfect(Hal::FemtoPair* pair) {
    auto par  = GetPairValDenPerf(pair);
    Int_t bin = ConvertVal(par);
    if (bin < 0) return;
    fDenProp->Fill(par.first, par.second);
    ((DividedHisto1D*) fArray->At(bin))->FillDenObj(pair);
  }

  Int_t FemtoCorrFunc2D::ConvertVal(std::pair<Double_t, Double_t> vals) const {
    const Int_t sizeX = fRangeX.GetSize();
    int xbin          = -1;
    if (vals.first < fRangeX.Get(0)) return -1;
    for (int i = 1; i < sizeX; i++) {
      if (vals.first < fRangeX.Get(i)) {
        xbin = i - 1;
        break;
      }
    }
    int ybin          = -1;
    const Int_t sizeY = fRangeY.GetSize();
    if (vals.second < fRangeY.Get(0)) return -1;
    for (int i = 1; i < sizeY; i++) {
      if (vals.second < fRangeY.Get(i)) {
        ybin = i - 1;
        break;
      }
    }
    return xbin + ybin * (fRangeX.GetSize() - 1);
  }

  FemtoCorrFunc2D::FemtoCorrFunc2D(const FemtoCorrFunc2D& other) :
    fLabelX(other.fLabelX),
    fLabelY(other.fLabelY),
    fRangeX(other.fRangeX),
    fRangeY(other.fRangeY),
    fNumProp(nullptr),
    fDenProp(nullptr) {
    if (other.fNumProp) {
      fNumProp = (TH2D*) other.fNumProp;
      fDenProp = (TH2D*) other.fDenProp;
    }
  }

  Bool_t FemtoCorrFunc2D::InitPropMon() {
    Double_t mins[2] = {fRangeX[0], fRangeY[0]};
    Double_t maxs[2] = {fRangeX[fRangeX.GetSize() - 1], fRangeY[fRangeY.GetSize() - 1]};
    fNumProp         = new TH2D("NumProp", "NumProp", fBinsX, mins[0], maxs[0], fBinsY, mins[1], maxs[1]);
    fDenProp         = new TH2D("DenProp", "DenProp", fBinsX, mins[0], maxs[0], fBinsY, mins[1], maxs[1]);
    fNumProp->SetXTitle(fLabelX);
    fDenProp->SetYTitle(fLabelY);
    return kTRUE;
  }

} /* namespace Hal */
