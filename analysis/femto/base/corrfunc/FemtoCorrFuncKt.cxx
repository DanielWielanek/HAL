/*
 * CorrFuncCollectionKt.cxx
 *
 *  Created on: 4 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoCorrFuncKt.h"

#include "Array.h"
#include "Cout.h"
#include "DividedHisto.h"
#include "FemtoPair.h"
#include "Std.h"
#include "HtmlCore.h"
#include "HtmlFile.h"
#include "ObjectMatrix.h"

#include <TCanvas.h>
#include <TH1.h>
#include <TMath.h>
#include <TROOT.h>
#include <TString.h>
#include <initializer_list>

#include <iostream>

namespace Hal {
  FemtoCorrFuncKt::FemtoCorrFuncKt(const DividedHisto1D& h,
                                   const std::initializer_list<double>& init,
                                   Int_t ktBins,
                                   Double_t ktMin,
                                   Double_t ktMax) :
    FemtoCorrFunc1D(h, "k_{T} [GeV/c]", init.size() - 1),
    fKtNum(new TH1D("k_{T num}", "k_{T} Numerator ;k_{T} [GeV/c];N_{pairs}", ktBins, ktMin, ktMax)),
    fKtDen(new TH1D("k_{T den}", "k_{T} Denominator ;k_{T} [GeV/c];N_{pairs}", ktBins, ktMin, ktMax)) {
    SetBins(init);
  }

  FemtoCorrFuncKt::FemtoCorrFuncKt() : FemtoCorrFunc1D(), fKtNum(nullptr), fKtDen(nullptr) {}

  void FemtoCorrFuncKt::AddHTMLCode(HtmlFile& file) const {
    if (fKtNum == nullptr) return;  // old nicafemto
    file.AddStringContent(HtmlCore::GetJsDiv("kt.root", "canvas;1"));
  }

  TString FemtoCorrFuncKt::HTMLExtract(Int_t no, TString dir) const {
    TString path = FemtoCorrFunc1D::HTMLExtract(no, dir);
    if (fKtNum != nullptr) {
      Bool_t batch = gROOT->IsBatch();
      gROOT->SetBatch(kTRUE);
      TCanvas* c = new TCanvas("canvas", "canvas", 0, 0, 1000, 1500);
      c->Divide(2, 1);
      c->cd(1);
      TH1D* hKt = (TH1D*) fKtNum->Clone();
      hKt->Draw();
      c->cd(2);
      hKt = (TH1D*) fKtDen->Clone();
      hKt->Draw();

      c->SaveAs(Form("%s/corrfunc_%i/kt.root", dir.Data(), no));
      delete c;
      gROOT->SetBatch(batch);
    }
    return path;
  }

  void FemtoCorrFuncKt::Add(const Object* pack) {
    FemtoCorrFunc1D::Add(pack);
    const FemtoCorrFuncKt* kt = dynamic_cast<const FemtoCorrFuncKt*>(pack);
    if (fKtNum && kt) {
      fKtNum->Add(kt->fKtNum);
      fKtDen->Add(kt->fKtDen);
    }
  }

  FemtoCorrFuncKt::~FemtoCorrFuncKt() {
    if (fKtNum) delete fKtNum;
    if (fKtDen) delete fKtDen;
  }

  void FemtoCorrFuncKt::FillNum(FemtoPair* pair) {
    Double_t px      = pair->GetPx1() + pair->GetPx2();
    Double_t py      = pair->GetPy1() + pair->GetPy2();
    Double_t kt      = TMath::Sqrt(px * px + py * py) * 0.5;
    const Int_t size = fRange.GetSize();
    fKtNum->Fill(kt);
    if (kt < fRange.Get(0)) return;
    for (int i = 1; i < size; i++) {
      if (kt < fRange.Get(i)) {
        ((DividedHisto1D*) fArray->At(i - 1))->FillNumObj(pair);
        return;
      }
    }
  }

  void FemtoCorrFuncKt::FillDenRotated(FemtoPair* pair) {
    Double_t px      = pair->GetPx1() - pair->GetPx2();
    Double_t py      = pair->GetPy1() - pair->GetPy2();
    Double_t kt      = TMath::Sqrt(px * px + py * py) * 0.5;
    const Int_t size = fRange.GetSize();
    fKtDen->Fill(kt);
    if (kt < fRange.Get(0)) return;
    for (int i = 1; i < size; i++) {
      if (kt < fRange.Get(i)) {
        ((DividedHisto1D*) fArray->At(i - 1))->FillDenObj(pair);
        return;
      }
    }
  }

  void FemtoCorrFuncKt::FillDenMixed(FemtoPair* pair) {
    Double_t px = pair->GetPx1() + pair->GetPx2();
    Double_t py = pair->GetPy1() + pair->GetPy2();
    Double_t kt = TMath::Sqrt(px * px + py * py) * 0.5;
    fKtDen->Fill(kt);
    const Int_t size = fRange.GetSize();
    if (kt < fRange.Get(0)) return;
    for (int i = 1; i < size; i++) {
      if (kt < fRange.Get(i)) {
        ((DividedHisto1D*) fArray->At(i - 1))->FillDenObj(pair);
        return;
      }
    }
  }

  Bool_t FemtoCorrFuncKt::Check() {
    for (int i = 1; i < fRange.GetSize(); i++) {
      if (fRange[i] < fRange[i - 1]) {
        Cout::PrintInfo(Form("Wrong order in %s %4.2f<%4.2f", this->ClassName(), fRange[i], fRange[i - 1]),
                        EInfo::kImportantError);
        return kFALSE;
      }
    }
    return FemtoCorrFunc1D::Check();
  }

  FemtoCorrFuncKt::FemtoCorrFuncKt(const FemtoCorrFuncKt& other) :
    FemtoCorrFunc1D(other), fKtNum((TH1D*) other.fKtNum->Clone()), fKtDen((TH1D*) other.fKtDen->Clone()) {}
}  // namespace Hal
