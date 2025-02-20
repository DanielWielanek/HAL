/*
 * FemtoCorrFunc1D.cxx
 *
 *  Created on: 26 cze 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoCorrFunc1D.h"

#include "Array.h"
#include "Cout.h"
#include "DividedHisto.h"
#include "FemtoPair.h"
#include "HtmlCore.h"
#include "HtmlFile.h"
#include "HtmlTable.h"
#include "ObjectMatrix.h"
#include "Std.h"

#include <initializer_list>

#include <RtypesCore.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TNamed.h>
#include <TROOT.h>
#include <TString.h>
#include <TSystem.h>


namespace Hal {
  TString FemtoCorrFunc1D::HTMLExtract(Int_t counter, TString dir) const {
    TString path = Form("%s/corrfunc_%i", dir.Data(), counter);
    gSystem->MakeDirectory(path);
    TString filename = Form("%s/corrfunc.html", path.Data());
    HtmlFile file(filename, kFALSE);
    HtmlTable table("", "haltable", "");
    HtmlRow row("", "dark_blue", "");
    row.AddContent(HtmlCellCol(fLabel, 2));
    row.AddContent(HtmlCell("value"));
    table.AddContent(row);
    for (int i = 0; i < fArray->GetSize(); i++) {
      DividedHisto1D* h = (DividedHisto1D*) fArray->At(i);
      HtmlRow row1("", "light_blue", "");
      if (!Angle()) {
        row1.AddContent(HtmlCell(Form("%4.2f", fRange.Get(i))));
        row1.AddContent(HtmlCell(Form("%4.2f", fRange.Get(i + 1))));
      } else {
        row1.AddContent(HtmlCell(Form("%4.2f&deg;", fRange.Get(i) * TMath::RadToDeg())));
        row1.AddContent(HtmlCell(Form("%4.2f&deg;", fRange.Get(i + 1) * TMath::RadToDeg())));
      }
      row1.AddContent(HtmlCell(h->HTMLExtract(i, path)));
      table.AddContent(row1);
    }
    if (fNumProp) {
      Bool_t batch = gROOT->IsBatch();
      gROOT->SetBatch(kTRUE);
      TCanvas* c = new TCanvas("canvas", "canvas", 0, 0, 1000, 1500);
      c->Divide(2, 1);
      c->cd(1);
      TH1D* hKt = (TH1D*) fNumProp->Clone();
      hKt->Draw();
      c->cd(2);
      hKt = (TH1D*) fDenProp->Clone();
      hKt->Draw();

      c->SaveAs(Form("%s/corrfunc_%i/kt.root", dir.Data(), counter));
      delete c;
      gROOT->SetBatch(batch);
    }
    HtmlRow row2("", "light_blue", "");
    row2.AddContent(HtmlCellCol("Comment:", 2));
    row2.AddContent(HtmlCell(HtmlCore::CheckBr(GetComment())));
    table.AddContent(row2);
    file.AddContent(table);
    file.AddStringContent(HtmlCore::GetJsDiv(path, "kt.root", "canvas;1"));
    AddHTMLCode(file);
    file.Save();
    return HtmlCore::GetUrl(Form("corrfunc_%i/corrfunc.html", counter), this->ClassName());
  }

  Bool_t FemtoCorrFunc1D::Check() {
    if (!fNumProp) {
      Cout::PrintInfo(Form("Missing histograms in %s, did you forget to set MakeHisto in c-tor?", ClassName()), EInfo::kError);
    }
    Int_t x_bins = fRange.GetSize() - 1;
    for (int i = 0; i < x_bins; i++) {
      DividedHisto1D* h = (DividedHisto1D*) fArray->At(i);
      TString histname  = h->GetName();
      h->SetName(Form("%s[%i]", histname.Data(), i));
    }
    for (int i = 1; i < fRange.GetSize(); i++) {
      if (fRange[i] < fRange[i - 1]) {
        Cout::PrintInfo(Form("Wrong order in %s %4.2f<%4.2f", this->ClassName(), fRange[i], fRange[i - 1]),
                        EInfo::kCriticalError);
        return kFALSE;
      }
    }
    return kTRUE;
  }

  void FemtoCorrFunc1D::SetBins(const std::initializer_list<double>& init) {
    if (fRange.GetSize() != (Int_t) init.size()) {
      Cout::PrintInfo(Form("Wrong size of array during call SetKtBins %i %i", fRange.GetSize(), (int) init.size()),
                      EInfo::kError);
      return;
    }
    std::initializer_list<double>::iterator it;
    Int_t pos = 0;
    for (it = init.begin(); it != init.end(); ++it) {
      fRange[pos++] = *it;
    }
  }

  FemtoCorrFunc1D::FemtoCorrFunc1D(const DividedHisto1D& h,
                                   std::initializer_list<Double_t> cuts,
                                   Int_t bins,
                                   TString nameTitle,
                                   TString xAxis,
                                   TString yAxis) :
    FemtoCorrFunc(h, cuts.size() - 1) {
    auto vec = Hal::Std::GetVector(cuts);
    fRange.MakeBigger(vec.size());
    for (unsigned int i = 0; i < vec.size(); i++) {
      fRange.Set(i, vec[i]);
    }
    fNumProp = new TH1D(nameTitle + " num", nameTitle + " num", bins, vec[0], vec[vec.size() - 1]);
    fDenProp = new TH1D(nameTitle + " den", nameTitle + " den", bins, vec[0], vec[vec.size() - 1]);
    fNumProp->SetXTitle(xAxis);
    fDenProp->SetXTitle(xAxis);
    fNumProp->SetYTitle(yAxis);
    fDenProp->SetYTitle(yAxis);
  }

  FemtoCorrFunc1D::~FemtoCorrFunc1D() {
    if (fNumProp) delete fNumProp;
    if (fDenProp) delete fDenProp;
  }

  void FemtoCorrFunc1D::FillNum(FemtoPair* pair) {
    Double_t val = GetPairValNum(pair);
    Int_t bin    = ConvertVal(val);
    if (bin < 0) return;
    fNumProp->Fill(val);
    ((DividedHisto1D*) fArray->At(bin))->FillNumObj(pair);
  }

  void FemtoCorrFunc1D::FillDenPerfect(FemtoPair* pair) {
    Double_t val = GetPairValDenPerf(pair);
    Int_t bin    = ConvertVal(val);
    if (bin < 0) return;
    fDenProp->Fill(val);
    ((DividedHisto1D*) fArray->At(bin))->FillDenObj(pair);
  }

  void FemtoCorrFunc1D::FillDenRotated(FemtoPair* pair) {
    Double_t val = GetPairValDenRot(pair);
    Int_t bin    = ConvertVal(val);
    if (bin < 0) return;
    fDenProp->Fill(val);
    ((DividedHisto1D*) fArray->At(bin))->FillDenObj(pair);
  }

  void FemtoCorrFunc1D::FillDenMixed(FemtoPair* pair) {
    Double_t val = GetPairValDenMix(pair);
    Int_t bin    = ConvertVal(val);
    if (bin < 0) return;
    fDenProp->Fill(val);
    ((DividedHisto1D*) fArray->At(bin))->FillDenObj(pair);
  }

  void FemtoCorrFunc1D::FillDenHemisphere(FemtoPair* pair) {
    Double_t val = GetPairValDenHemi(pair);
    Int_t bin    = ConvertVal(val);
    if (bin < 0) return;
    fDenProp->Fill(val);
    ((DividedHisto1D*) fArray->At(bin))->FillDenObj(pair);
  }

  void FemtoCorrFunc1D::FillDenCharged(FemtoPair* pair) {
    Double_t val = GetPairValDenChar(pair);
    Int_t bin    = ConvertVal(val);
    if (bin < 0) return;
    fDenProp->Fill(val);
    ((DividedHisto1D*) fArray->At(bin))->FillDenObj(pair);
  }

  void FemtoCorrFunc1D::Add(const Object* pack) {
    FemtoCorrFunc::Add(pack);
    auto fcf = static_cast<const FemtoCorrFunc1D*>(pack);
    if (fNumProp) {
      fNumProp->Add(fcf->fNumProp);
      fDenProp->Add(fcf->fDenProp);
    }
  }

  Int_t FemtoCorrFunc1D::ConvertVal(Double_t val) const {
    const Int_t size = fRange.GetSize();
    if (val < fRange.Get(0)) return -1;
    for (int i = 1; i < size; i++) {
      if (val < fRange.Get(i)) { return i - 1; }
    }
    return -1;
  }

} /* namespace Hal */
