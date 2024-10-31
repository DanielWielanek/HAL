/*
 * FluctuationMath.cxx
 *
 *  Created on: 31 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "Fluctuation1D.h"

#include "Cout.h"
#include "HtmlFile.h"
#include "HtmlTable.h"

#include <TCanvas.h>
#include <TLatex.h>
#include <TMath.h>
#include <TSystem.h>
#include <iostream>

namespace Hal {
  Fluctuation1D::Fluctuation1D() :
    fN(0),
    fNError(0),
    fOmega(0),
    fSSigma(0),
    fKappaSigma(0),
    fOmegaError(0),
    fSigmaError(0),
    fKappaError(0),
    fNSample(10),
    fHistogram(NULL) {
    fM[0] = fM[1] = fM[2] = fM[3] = fM[4] = fM[5] = fM[6] = 0;
    fMErr[0] = fMErr[1] = fMErr[2] = fMErr[3] = fMErr[4] = fMErr[5] = fMErr[6] = 0;
  }

  void Fluctuation1D::Calc(TH1D* h) {
    fN = fM[0] = fM[1] = fM[2] = fM[3] = fM[4] = fM[5] = fM[6] = 0.0;
    Double_t entries                                           = h->GetEntries();
    // fHisto->Scale(1.0/entries);
    for (int i = 1; i <= h->GetNbinsX(); i++) {
      Double_t pN = ((Double_t) h->GetBinContent(i));
      Double_t N  = (Double_t) h->GetBinCenter(i);
      fN += N * pN;
    }
    fN = fN / entries;
    for (int i = 1; i <= h->GetNbinsX(); i++) {
      Double_t pN = ((Double_t) h->GetBinContent(i));
      Double_t N  = (Double_t) h->GetBinCenter(i);
      for (int j = 1; j <= 6; j++) {
        fM[j] += TMath::Power(N - fN, j) * pN;
      }
    }
    for (int j = 1; j <= 6; j++) {
      fM[j] = fM[j] / entries;
    }
    fOmega      = GetCentralMoment(2) / fN;
    fSSigma     = GetCentralMoment(3) / GetCentralMoment(2);
    fKappaSigma = GetCentralMoment(4) / GetCentralMoment(2) - 3.0 * GetCentralMoment(2);
  }

  Double_t Fluctuation1D::GetCentralMoment(Int_t i) const {
    if (i < 1 || i > 6) {
      Cout::PrintInfo("wrong mu index", EInfo::kLowWarning);
      return 0;
    } else {
      return fM[i];
    }
  }

  Double_t Fluctuation1D::GetCentralMomentError(Int_t i) const {
    if (i < 1 || i > 6) {
      Cout::PrintInfo("wrong mu index", EInfo::kLowWarning);
      return 0;
    } else {
      return fMErr[i];
    }
  }

  void Fluctuation1D::CalcError(TH1D* h) {
    TH1D* hh                  = (TH1D*) h->Clone("temp_boostrap");
    Fluctuation1D* stat       = new Fluctuation1D();
    Double_t* mean            = new Double_t[fNSample];
    Double_t* omega           = new Double_t[fNSample];
    Double_t* sigma           = new Double_t[fNSample];
    Double_t* kappa           = new Double_t[fNSample];
    Double_t** central_moment = new Double_t*[5];
    for (int i = 0; i < 5; i++) {
      central_moment[i] = new Double_t[fNSample];
    }
    for (int i = 0; i < fNSample; i++) {
      hh->Reset();
      for (int j = 0; j < h->GetEntries(); j++) {
        hh->Fill(h->GetRandom());
      }
      stat->Calc(hh);
      omega[i] = stat->GetOmega();
      sigma[i] = stat->GetSSigma();
      kappa[i] = stat->GetKappaSigma();
      mean[i]  = stat->GetMean();
      for (int j = 2; j < 7; j++)
        central_moment[j - 2][i] = stat->GetCentralMoment(j);
    }
    fOmegaError = GetRMS(omega, fNSample);
    fSigmaError = GetRMS(sigma, fNSample);
    fKappaError = GetRMS(kappa, fNSample);
    fNError     = GetRMS(mean, fNSample);
    for (int i = 2; i < 7; i++) {
      fMErr[i] = GetRMS(central_moment[i - 2], fNSample);
    }
    for (int i = 0; i < 5; i++) {
      delete[] central_moment[i];
    }
    delete[] central_moment;
    delete[] omega;
    delete[] kappa;
    delete[] sigma;
    delete[] mean;
    delete stat;
    delete hh;
  }

  Fluctuation1D::Fluctuation1D(TH1D* h) : Fluctuation1D() {
    if (h->InheritsFrom("TH2")) {
      Cout::PrintInfo("Cannot  call Fluctuation1D(TH2D*)", EInfo::kLowWarning);
      return;
    }
    fHistogram = (TH1D*) h->Clone();
    Recalculate();
  }

  Fluctuation1D::Fluctuation1D(TH2D* h, TString opt) : Fluctuation1D() {
    if (opt.EqualTo("x") || opt.EqualTo("pos")) {
      fHistogram = h->ProjectionX(Form("%s_pos", h->GetName()));
    } else if (opt.EqualTo("y") || opt.EqualTo("neg")) {
      fHistogram = h->ProjectionY(Form("%s_neg", h->GetName()));
    } else if (opt.EqualTo("sum") || opt.EqualTo("+")) {
      fHistogram = CalcProfile(h, 1);
    } else {
      fHistogram = CalcProfile(h, 0);
    }
    Recalculate();
  }

  TString Fluctuation1D::HTMLExtract(Int_t no, TString dir) const {
    TString path = Form("%s/fluct_%i", dir.Data(), no);
    gSystem->MakeDirectory(path);
    TString filename = Form("%s/fluct.html", path.Data());
    HtmlFile file(filename, kFALSE);
    HtmlTable table("", "haltable", "");
    HtmlRow row1("", "dark_blue", "");
    row1.AddContent(HtmlCell("label"));
    row1.AddContent(HtmlCellCol("value", 2));
    table.AddContent(row1);
    HtmlRow row2("", "dark_blue", "");
    row2.AddContent(HtmlCell("Name"));
    row2.AddContent(HtmlCellCol(GetName(), 2));
    table.AddContent(row2);
    HtmlRow row3("", "dark_blue", "");
    row2.AddContent(HtmlCell("NSample"));
    row2.AddContent(HtmlCellCol(Form("%i", fNSample), 2));
    table.AddContent(row3);
    file.AddContent(table);
    HtmlTable table2("", "haltable", "");
    HtmlRow row4("", "dark_blue", "");
    row4.AddContent(HtmlCell("No."));
    row4.AddContent(HtmlCell("Name/Value"));
    table2.AddContent(row4);
    file.AddContent(table2);
    TCanvas* c1 = new TCanvas();
    fHistogram->DrawClone();
    TLatex latex;
    latex.SetNDC(kTRUE);
    latex.DrawLatex(0.5, 0.8, Form("#omega %4.3f #pm %4.3f", GetOmega(), GetOmegaError()));
    latex.DrawLatex(0.5, 0.75, Form("#kappa#sigma^{2} %4.3f #pm %4.3f", GetKappaSigma(), GetKappaSigmaError()));
    latex.DrawLatex(0.5, 0.7, Form("S#sigma %4.3f #pm %4.3f", GetSSigma(), GetSSigmaError()));
    c1->SaveAs(Form("%s//fluct.root", path.Data()));
    delete c1;
    TString file_name = Form("%s/fluct.root", path.Data());
    file.AddStringContent(HtmlCore::GetJsDiv(path, "fluct.root", "canvas;1"));
    file.Save();
    return HtmlCore::GetUrl(Form("fluct_%i/fluct.html", no), this->ClassName());
  }

  void Fluctuation1D::Add(const Object* pack) {
    if (pack->InheritsFrom("Hal::Fluctuation1D")) {
      Fluctuation1D* other = (Fluctuation1D*) pack;
      fHistogram->Add(other->fHistogram);
      Recalculate();
    } else {
      Cout::PrintInfo(Form("Cannot ddd %s to  %s", pack->ClassName(), this->ClassName()), EInfo::kLowWarning);
    }
  }

  Long64_t Fluctuation1D::Merge(TCollection* collection) {
    if (collection) {
      Fluctuation1D* pack = NULL;
      TIter iterator(collection);
      while ((pack = (Fluctuation1D*) iterator())) {
        fHistogram->Add(pack->fHistogram);
      }
      Recalculate();
    }
    return 1;
  }

  Double_t Fluctuation1D::GetRMS(Double_t* array, Int_t size) const {
    Double_t average = 0;
    Double_t n       = size;
    for (int i = 0; i < size; i++) {
      average += array[i];
    }
    average      = average / n;
    Double_t rms = 0;
    for (int i = 0; i < size; i++) {
      Double_t dx = array[i] - average;
      rms += dx * dx;
    }
    return TMath::Sqrt(rms / n);
  }

  TH1D* Fluctuation1D::CalcProfile(TH2D* h, Int_t opt) {
    Double_t min_pos = h->GetXaxis()->GetBinLowEdge(1);
    Double_t max_pos = h->GetXaxis()->GetBinUpEdge(h->GetXaxis()->GetNbins());
    Double_t min_neg = h->GetYaxis()->GetBinLowEdge(1);
    Double_t max_neg = h->GetYaxis()->GetBinUpEdge(h->GetYaxis()->GetNbins());
    Double_t min, max;
    if (opt == 0) {  // difference
      min = min_pos - max_neg;
      max = max_pos - min_neg;
    } else {  // sum
      min = min_pos + min_neg;
      max = max_pos + max_neg;
    }
    min = TMath::Floor(min) - 0.5;
    max = TMath::Ceil(max) + 0.5;

    TH1D* temp = new TH1D("profile", "profile", (max - min), min, max);
    for (int j = 1; j <= h->GetNbinsX(); j++) {
      Double_t y = h->GetYaxis()->GetBinCenter(j);
      if (opt == 0) y = -y;
      for (int i = 1; i <= h->GetNbinsY(); i++) {
        Double_t x = h->GetXaxis()->GetBinCenter(i);
        Double_t n = h->GetBinContent(i, j);
        temp->Fill(x + y, n);
      }
    }
    return temp;
  }

  void Fluctuation1D::Recalculate() {
    Calc(fHistogram);
    CalcError(fHistogram);
  }

  void Fluctuation1D::Browse(TBrowser* b) {
    gPad->Clear();
    TVirtualPad* c1 = gPad;
    Draw("all");
    gPad = c1;
    b->Add(fHistogram);
  }

  void Fluctuation1D::Draw(Option_t* opt) {
    TString option = opt;
    if (option == "all") {
      fHistogram->SetStats(kFALSE);
      fHistogram->Draw();
      TLatex latex;
      latex.SetNDC(kTRUE);
      latex.DrawLatex(0.5, 0.8, Form("#omega %4.3f #pm %4.3f", GetOmega(), GetOmegaError()));
      latex.DrawLatex(0.5, 0.75, Form("#kappa#sigma^{2} %4.3f #pm %4.3f", GetKappaSigma(), GetKappaSigmaError()));
      latex.DrawLatex(0.5, 0.7, Form("S#sigma %4.3f #pm %4.3f", GetSSigma(), GetSSigmaError()));
    } else {
      fHistogram->Draw();
    }
  }

  void Fluctuation1D::GetFrom2D(TH1D* /*h*/, TString /*opt*/) {}

  void Fluctuation1D::Print(Option_t* /*option*/) const {
    std::cout << "Fluctuation1D " << std::endl;
    std::cout << Form("Mean\t\t:%4.3f+/-%4.3f", GetMean(), GetMeanError()) << std::endl;
    std::cout << Form("KappaSigma\t\t:%4.3f+/-%4.3f", GetKappaSigma(), GetKappaSigmaError()) << std::endl;
    std::cout << Form("Omega\t\t:%4.3f+/-%4.3f", GetOmega(), GetOmegaError()) << std::endl;
    std::cout << Form("SSigma\t\t:%4.3f+/-%4.3f", GetSSigma(), GetSSigmaError()) << std::endl;
    for (int i = 2; i < 7; i++) {
      std::cout << Form("Central moment[%i]\t:%4.3f+/-%4.3f", i, GetCentralMoment(i), GetCentralMomentError(i)) << std::endl;
    }
    std::cout << "**********************" << std::endl;
  }

  Fluctuation1D::~Fluctuation1D() {
    if (fHistogram) delete fHistogram;
  }
}  // namespace Hal
