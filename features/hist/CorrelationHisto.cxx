/*
 * CorrHisto.cxx
 *
 *  Created on: 11 kwi 2024
 *      Author: daniel
 */

#include "CorrelationHisto.h"

#include <TAxis.h>
#include <TCanvas.h>
#include <TCollection.h>
#include <TH1.h>
#include <TH2.h>
#include <TMath.h>
#include <TMathBase.h>
#include <TNamed.h>
#include <TROOT.h>
#include <TSystem.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stddef.h>

#include "Cout.h"
#include "HtmlCore.h"
#include "HtmlFile.h"
#include "HtmlTable.h"
#include "Std.h"

namespace Hal {
  CorrelationHisto::CorrelationHisto(Int_t nParams, Int_t nTest) :
    fNParams(nParams), fNTest(TMath::Abs(nTest)), fAutoFill(kTRUE) {}

  void CorrelationHisto::ConfigParam(Int_t par, Int_t bins, Double_t min, Double_t max, TString name) {
    if (par >= fNParams) {
      fAxesConf.resize(par + 1);
      fNParams = par + 1;
    }
    fAxesConf.insert(fAxesConf.begin() + par, Hal::HistogramAxisConf(name, bins, min, max));
    fAutoFill = kFALSE;
    fNTest    = -1;
  }

  CorrelationHisto::CorrelationHisto(std::initializer_list<std::initializer_list<Double_t>> vals,
                                     std::initializer_list<TString> names) {
    auto ranges = Hal::Std::GetVector(vals);
    auto namess = Hal::Std::GetVector(names);
    if (ranges.size() > namess.size()) {
      for (int i = namess.size(); i < ranges.size(); i++) {
        namess.push_back(Form("Par_%i", i));
      }
    }
    fNParams = ranges.size();
    for (int i = 0; i < fNParams; i++) {
      auto vec = Hal::Std::GetVector(ranges[i]);
      fAxesConf.push_back(Hal::HistogramAxisConf(namess[i], vec[0], vec[1], vec[2]));
    }
    fNTest    = -1;
    fAutoFill = kFALSE;
  }

  void CorrelationHisto::Fill(std::vector<Double_t> params) {
    if (params.size() != fNParams) return;
    //    HalCoutDebug();
    if (fAutoFill) {
      //      HalCoutDebug();
      if (fTestValues.size() < fNTest) {

        fTestValues.push_back(params);
        return;
      } else {
        fAutoFill = kFALSE;
        AutoInitialize();
      }
    } else {
      if (fNTest == -1) {
        Initialize();
        fNTest = 0;
      }
    }
    //    HalCoutDebug();
    int count = 0;
    for (int i = 0; i < fNParams; i++) {
      double x = params[i];
      for (int j = i; j < fNParams; j++) {
        double y = params[j];
        fHistograms[count++]->Fill(x, y);
      }
    }
  }

  void CorrelationHisto::Initialize() {
    fNHistograms = (fNParams + 1) * fNParams / 2;
    fHistograms.resize(fNHistograms);
    int count = 0;
    for (int i = 0; i < fNParams; i++) {
      for (int j = i; j < fNParams; j++) {
        count++;
        TString title = Form("SB_%i", count);
        if (i == j) {
          fHistograms[count - 1] = new TH1D(title, title, fAxesConf[i].GetNBins(), fAxesConf[i].GetMin(), fAxesConf[i].GetMax());
          fHistograms[count - 1]->GetYaxis()->CenterTitle();
          fHistograms[count - 1]->GetYaxis()->SetTitle("N");
        } else {
          fHistograms[count - 1] = new TH2D(title,
                                            title,
                                            fAxesConf[i].GetNBins(),
                                            fAxesConf[i].GetMin(),
                                            fAxesConf[i].GetMax(),
                                            fAxesConf[j].GetNBins(),
                                            fAxesConf[j].GetMin(),
                                            fAxesConf[j].GetMax());
          fHistograms[count - 1]->GetYaxis()->CenterTitle();
          fHistograms[count - 1]->GetYaxis()->SetTitle(fAxesConf[j].GetName());
        }
        fHistograms[count - 1]->GetXaxis()->SetTitle(fAxesConf[i].GetName());
        fHistograms[count - 1]->GetXaxis()->CenterTitle();
      }
    }
  }

  void CorrelationHisto::AutoInitialize() {
    int count = 0;
    for (int par = 0; par < fNParams; par++) {
      double mini = 1E+9, maxi = 1E-9, av, sum = 0;
      double n = fNTest;
      for (unsigned int ent = 0; ent < fNTest; ent++) {
        double j = fTestValues[ent][par];
        sum += j;
        mini = TMath::Min(mini, j);
        maxi = TMath::Max(maxi, j);
      }
      av         = sum / n;
      double rms = 0;
      for (unsigned int ent = 0; ent < fNTest; ent++) {
        double j = fTestValues[ent][par];
        rms += (av - j) * (av - j);
      }
      rms  = TMath::Sqrt(rms / n);
      mini = TMath::Min(mini, av - rms * 4.0);
      maxi = TMath::Max(maxi, av + rms * 4.0);
      fAxesConf.push_back(Hal::HistogramAxisConf(Form("%i", count++), 200, mini, maxi));
    }

    Initialize();
    for (auto i : fTestValues) {
      FillUnchecked(i);
    }
    fTestValues.clear();
  }

  void CorrelationHisto::FillUnchecked(std::vector<Double_t> params) {
    int count = 0;
    for (int i = 0; i < fNParams; i++) {
      double x = params[i];
      for (int j = i; j < fNParams; j++) {
        double y = params[j];
        if (i != j)
          fHistograms[count++]->Fill(x, y);
        else
          fHistograms[count++]->Fill(x);
      }
    }
  }

  void CorrelationHisto::Draw(Option_t* opt) {
    TString option   = opt;
    Bool_t logx      = Hal::Std::FindParam(option, "logx", kTRUE);
    Bool_t logy      = Hal::Std::FindParam(option, "logy", kTRUE);
    Bool_t logz      = Hal::Std::FindParam(option, "logz", kTRUE);
    Bool_t gridx     = Hal::Std::FindParam(option, "gridx", kTRUE);
    Bool_t gridy     = Hal::Std::FindParam(option, "gridy", kTRUE);
    TString otherOpt = option;
    auto pads        = Hal::Std::GetGridPad(fNParams, fNParams, 0.124, 0.124);
    int count        = 0;
    for (int i = 0; i < fNParams; i++) {
      for (int j = i; j < fNParams; j++) {
        pads[i][j]->cd();
        if (otherOpt.Length() == 0)
          fHistograms[count++]->Draw("col");
        else
          fHistograms[count++]->Draw(otherOpt);
        if (logx) gPad->SetLogx();
        if (i != j) {
          if (logy) gPad->SetLogy();
          if (logz) gPad->SetLogz();
        } else {
          if (logz) gPad->SetLogy();
        }
        if (gridx) gPad->SetGridx();
        if (gridy) gPad->SetGridy();
      }
    }
  }

  CorrelationHisto::~CorrelationHisto() {
    if (fHistograms.size()) {
      for (int i = 0; i < fNHistograms; i++) {
        delete fHistograms[i];
      }
    }
  }

  Bool_t CorrelationHisto::CanBeTableElement() const { return kFALSE; }

  Long64_t CorrelationHisto::Merge(TCollection* collection) {
    if (collection) {
      CorrelationHisto* pack = NULL;
      TIter iterator(collection);
      while ((pack = (CorrelationHisto*) iterator())) {
        Add(pack);
      }
    }
    return 1;
  }

  TString CorrelationHisto::GetExtractType() const { return "Page"; }

  TString CorrelationHisto::HTMLExtract(Int_t counter, TString dir) const {
    /// build canvas
    TString path = Form("%s/correlations_%i", dir.Data(), counter);
    gSystem->MakeDirectory(path);
    Bool_t batch = gROOT->IsBatch();
    gROOT->SetBatch(kTRUE);
    if (fNParams < 5) {
      TCanvas* c1 = new TCanvas("canvas", "canvas", 0, 0, 1000, 1500);
      auto pads   = Hal::Std::GetGridPad(fNParams, fNParams, 0.124, 0.124);
      int count   = 0;
      for (int i = 0; i < fNParams; i++) {
        for (int j = i; j < fNParams; j++) {
          pads[i][j]->cd();
          fHistograms[count++]->Draw("col");
        }
      }
      c1->SaveAs(Form("%s/correlations_%i/correlations.root", dir.Data(), counter));
    } else {
      int count = 0;
      for (int i = 0; i < fNParams; i++) {
        for (int j = i; j < fNParams; j++) {
          TCanvas* c1 = new TCanvas("canvas", "canvas", 0, 0, 1000, 1500);
          fHistograms[count++]->Draw("col");
          c1->SaveAs(Form("%s/correlations_%i/correlations_%i_%i.root", dir.Data(), counter, i, j));
        }
      }
    }
    // create text file
    gROOT->SetBatch(batch);


    /// build HTML
    path = Form("%s/correlations_%i", dir.Data(), counter);
    gSystem->MakeDirectory(path);
    TString filename = Form("%s/correlations.html", path.Data());
    HtmlFile file(filename, kFALSE);

    if (fNParams < 5) {
      TString file_name = Form("%s/correlations.root", path.Data());
      std::ifstream f(file_name.Data());
      if (f.good()) { file.AddStringContent(HtmlCore::GetJsDiv(path, "correlations.root", "canvas;1")); }
      file.Save();
    } else {
      HtmlTable table("", "haltable", "");
      HtmlRow legend("", "dark_blue", "");
      std::vector<HtmlRow> rows;
      legend.AddContent(HtmlCell());
      for (int i = 0; i < fNParams; i++) {
        HtmlCell cell;
        cell.SetStringContent(fAxesConf[i].GetTitle());
        legend.AddContent(cell);
      }
      table.AddContent(legend);
      for (int iy = 0; iy < fNParams; iy++) {
        HtmlRow row("", "light_blue", "");
        HtmlCell cell1;
        cell1.SetStringContent(fAxesConf[iy].GetTitle());
        row.AddContent(cell1);
        for (int ix = 0; ix < fNParams; ix++) {
          if (ix <= iy) {
            HtmlCell cell;
            TString divText = HtmlCore::GetJsDiv(
              path, Form("correlations_%i_%i.root", ix, iy), "canvas;1", "colz", Form("x%i_%i_%i", counter, ix, iy));
            divText.ReplaceAll("style=\"width:90%;height:900px\"", "style=\"width:300px;height:300px\"");
            cell.AddStringContent(divText);
            row.AddContent(cell);
          } else {
            HtmlCell cell;
            row.AddContent(cell);
          }
        }
        table.AddContent(row);
        rows.push_back(row);
      }
      file.AddContent(table);
      file.Save();
    }
    return HtmlCore::GetUrl(Form("correlations_%i/correlations.html", counter), this->ClassName());
  }

  TH1* CorrelationHisto::GetCorrelationHisto(Int_t parA, Int_t parB) const {
    int count = 0;
    for (int i = 0; i < fNParams; i++) {
      for (int j = i; j < fNParams; j++) {
        count++;
        if (parA == i && parB == j) return fHistograms[count - 1];
      }
    }
    Hal::Cout::PrintInfo(Form("CorrelationHisto::GetCorrelationHisto histogram for params %i vs %i not found", parA, parB),
                         EInfo::kWarning);
    return nullptr;
  }

  void CorrelationHisto::Add(const Hal::Object* pack) {
    auto copy = dynamic_cast<const CorrelationHisto*>(pack);
    if (!copy) return;
    if (copy->fNHistograms != fNHistograms) return;
    for (int i = 0; i < fNHistograms; i++) {
      fHistograms[i]->Add(copy->fHistograms[i]);
    }
  }

}  // namespace Hal
