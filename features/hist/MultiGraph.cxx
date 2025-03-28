/*
 * HalMultiGraph.cxx
 *
 *  Created on: 5 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "MultiGraph.h"

#include "Cout.h"
#include "StdHist.h"

#include <TAttAxis.h>
#include <TAttLine.h>
#include <TAttMarker.h>
#include <TAxis.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TH1D.h>
#include <TMultiGraph.h>
#include <TString.h>
#include <TVirtualPad.h>
#include <iostream>

namespace Hal {

  MultiGraph::MultiGraph() : fMultiGraph(new TMultiGraph()), fXaxis(nullptr), fYaxis(nullptr) {
    fMin[0] = fMin[1] = 0;
    fMax[0] = fMax[1] = 0;
  }
  MultiGraph::MultiGraph(Int_t graphs, Option_t* opt) : MultiGraph() { MakeGraphs(graphs, opt); }

  void MultiGraph::MakeGraph(Option_t* opt) {
    auto graph = new TGraphAsymmErrors();
    fGraphErrors.push_back(graph);
    fAddingOptions.push_back(opt);
  }

  void MultiGraph::SetXRange(Double_t min, Double_t max) {
    fMin[0] = min;
    fMax[0] = max;
  }

  void MultiGraph::SetYRange(Double_t min, Double_t max) {
    fMin[1] = min;
    fMax[1] = max;
  }

  void MultiGraph::SetPoint(Int_t graph, Int_t point, Double_t x, Double_t y) { fGraphErrors[graph]->SetPoint(point, x, y); }

  void MultiGraph::SetPoint(Int_t graph, Int_t point, Double_t x, Double_t ex, Double_t y, Double_t ey) {
    fGraphErrors[graph]->SetPoint(point, x, y);
    fGraphErrors[graph]->SetPointError(point, ex, ex, ey, ey);
  }

  TGraphAsymmErrors* MultiGraph::GetGraph(Int_t no) const { return fGraphErrors[no]; }

  void MultiGraph::Draw(Option_t* opt) {
    fDrawn          = kTRUE;
    int old_entries = 0;
    if (fMultiGraph->GetListOfGraphs()) old_entries = fMultiGraph->GetListOfGraphs()->GetEntries();

    for (int i = old_entries; i < fGraphErrors.size(); i++) {
      fMultiGraph->Add(fGraphErrors[i], fAddingOptions[i]);
    }
    TString option = opt;
    if (option.Contains("A") == kFALSE) option = "A" + option;
    fMultiGraph->Draw(option);
    if (fXaxis) Hal::Std::CopyAxisProp(fXaxis, fMultiGraph->GetXaxis());
    if (fYaxis) Hal::Std::CopyAxisProp(fYaxis, fMultiGraph->GetYaxis());
    if (fMin[0] != fMax[0]) {
      fMultiGraph->GetXaxis()->SetRangeUser(fMin[0], fMax[0]);
      fMultiGraph->GetXaxis()->SetLimits(fMin[0], fMax[0]);  // why?
    }
    if (fMin[1] != fMax[1]) fMultiGraph->GetYaxis()->SetRangeUser(fMin[1], fMax[1]);
    gPad->Update();
    gPad->RedrawAxis();
  }

  void MultiGraph::SetMarkerStyle(Style_t marker, Int_t no) {
    auto vecs = GetListForOpt(no);
    for (auto gr : vecs) {
      gr->SetMarkerStyle(marker);
    }
  }

  void MultiGraph::SetMarkerSize(Size_t size, Int_t no) {
    auto vecs = GetListForOpt(no);
    for (auto gr : vecs) {
      gr->SetMarkerSize(size);
    }
  }

  void MultiGraph::SetMarkerColor(Color_t color, Int_t no) {
    auto vecs = GetListForOpt(no);
    for (auto gr : vecs) {
      gr->SetMarkerColor(color);
    }
  }

  void MultiGraph::SetLineWidth(Width_t width, Int_t no) {
    auto vecs = GetListForOpt(no);
    for (auto gr : vecs) {
      gr->SetLineWidth(width);
    }
  }

  void MultiGraph::SetLineColor(Color_t color, Int_t no) {
    auto vecs = GetListForOpt(no);
    for (auto gr : vecs) {
      gr->SetLineColor(color);
    }
  }

  void MultiGraph::SetLineStyle(Style_t style, Int_t no) {
    auto vecs = GetListForOpt(no);
    for (auto gr : vecs) {
      gr->SetLineStyle(style);
    }
  }

  void MultiGraph::SetFillStyle(Int_t style, Int_t no) {
    auto vecs = GetListForOpt(no);
    for (auto gr : vecs) {
      gr->SetFillStyle(style);
    }
  }

  void MultiGraph::SetFillColor(Color_t color, Int_t no) {
    auto vecs = GetListForOpt(no);
    for (auto gr : vecs) {
      gr->SetFillColor(color);
    }
  }

  void MultiGraph::SetLineAttributes(Style_t style, Width_t width, Color_t color, Int_t no) {
    SetLineStyle(style, no);
    SetLineWidth(width, no);
    SetLineColor(color, no);
  }

  void MultiGraph::SetMarkerAttributes(Style_t style, Size_t size, Color_t color, Int_t no) {
    SetMarkerStyle(style, no);
    SetMarkerSize(size, no);
    SetMarkerColor(color, no);
  }

  void MultiGraph::MakeGraphs(Int_t no, Option_t* opt) {
    for (int i = 0; i < no; i++) {
      MakeGraph(opt);
    }
  }

  TMultiGraph* MultiGraph::GetMutliGraph() const { return fMultiGraph; }

  MultiGraph::~MultiGraph() {
    delete fMultiGraph;
    for (auto gr : fGraphErrors)
      delete gr;
    if (fXaxis) delete fXaxis;
    if (fYaxis) delete fYaxis;
  }

  TAxis* MultiGraph::GetXaxis() {
    if (fXaxis) return fXaxis;
    fXaxis = new TAxis();
    return fXaxis;
  }

  TAxis* MultiGraph::GetYaxis() {
    if (fYaxis) return fYaxis;
    fYaxis = new TAxis();
    return fYaxis;
  }

  void MultiGraph::AddHistogram(const TH1& h, Int_t no) {
    if (no == -1) {
      MakeGraph();
      no = GetNGraphs() - 1;
    }
    auto axis = h.GetXaxis();
    for (int i = 0; i <= axis->GetNbins(); i++) {
      SetPoint(no, i, axis->GetBinCenter(i), 0, h.GetBinContent(i), h.GetBinError(i));
    }
  }

  void MultiGraph::SetPoint(Int_t graph,
                            Int_t point,
                            Double_t x,
                            Double_t exLow,
                            Double_t exHigh,
                            Double_t y,
                            Double_t eyLow,
                            Double_t eyHigh) {
    fGraphErrors[graph]->SetPoint(point, x, y);
    fGraphErrors[graph]->SetPointError(point, exLow, exHigh, eyLow, eyHigh);
  }

  void MultiGraph::MakeStatSys(TString optStat, TString optSys) {
    for (int i = 0; i < fGraphErrors.size(); i++) {
      if (i % 2 == 0) {  // sys
        if (optSys.Length() != 0)
          fAddingOptions[i] = optSys;
        else
          fAddingOptions[i] = "5";
      } else {  // stat
        if (optStat.Length() != 0) fAddingOptions[i] = optStat;
      }
    }
  }

  void MultiGraph::SetPointStatSys(Int_t pairGraph,
                                   Int_t point,
                                   Double_t x,
                                   std::vector<Double_t> errXStat,
                                   std::vector<Double_t> errXsys,
                                   Double_t y,
                                   std::vector<Double_t> errYStat,
                                   std::vector<Double_t> errYsys) {
    if (fGraphErrors.size() < pairGraph * 2) return;
    auto grSys  = fGraphErrors[pairGraph * 2];
    auto grStat = fGraphErrors[pairGraph * 2 + 1];
    Double_t xLowStat, xLowSys, xHighStat, xHighSys;
    Double_t yLowStat, yLowSys, yHighStat, yHighSys;
    auto check = [](std::vector<Double_t> vec, Double_t& lo, Double_t& hi) {
      switch (vec.size()) {
        case 0: lo = hi = 0; break;
        case 1: lo = hi = vec[0]; break;
        default:
          lo = vec[0];
          hi = vec[1];
          break;
      }
    };
    check(errXStat, xLowStat, xHighStat);
    check(errXsys, xLowSys, xHighSys);
    check(errYStat, yLowStat, yHighStat);
    check(errYsys, yLowSys, yHighSys);
    grSys->SetPoint(point, x, y);
    grStat->SetPoint(point, x, y);
    grSys->SetPointError(point, xLowSys, xHighSys, yLowSys, yLowSys);
    grStat->SetPointError(point, xLowStat, xHighStat, yLowStat, yLowStat);
  }

  void MultiGraph::RecalculateXLogErrors(Double_t ref, Double_t width, Int_t gr) {
    auto graphs          = GetListForOpt(gr);
    const Double_t delta = TMath::Log(ref + width) - TMath::Log(ref);
    for (auto graph : graphs) {
      for (int i = 0; i < graph->GetN(); i++) {
        Double_t z     = graph->GetPointX(i);
        Double_t basic = TMath::Log10(z);
        Double_t up    = TMath::Power(10.0, basic + delta);
        Double_t down  = TMath::Power(10.0, basic - delta);
        graph->SetPointEXlow(i, z - down);
        graph->SetPointEXhigh(i, up - z);
      }
    }
  }

  void MultiGraph::RecalculateYLogErrors(Double_t ref, Double_t width, Int_t gr) {
    auto graphs          = GetListForOpt(gr);
    const Double_t delta = TMath::Log(ref + width) - TMath::Log(ref);
    for (auto graph : graphs) {
      for (int i = 0; i < graph->GetN(); i++) {
        Double_t z     = graph->GetPointY(i);
        Double_t basic = TMath::Log10(z);
        Double_t up    = TMath::Power(10.0, basic + delta);
        Double_t down  = TMath::Power(10.0, basic - delta);
        graph->SetPointEYlow(i, z - down);
        graph->SetPointEYhigh(i, up - z);
      }
    }
  }

  std::vector<TGraphAsymmErrors*> MultiGraph::GetListForOpt(Int_t no) const {
    std::vector<TGraphAsymmErrors*> callVector;
    switch (no) {
      case -3: {
        for (int i = 0; i < GetNGraphs(); i++)
          if (i % 2 == 1) callVector.push_back(fGraphErrors[i]);
      } break;
      case -2: {
        for (int i = 0; i < GetNGraphs(); i++)
          if (i % 2 == 0) callVector.push_back(fGraphErrors[i]);
      } break;
      case -1: {
        for (auto gr : fGraphErrors) {
          callVector.push_back(gr);
        }
      } break;
      default: {
        if (no < 0) return callVector;
        if (no < fGraphErrors.size()) callVector.push_back(fGraphErrors[no]);
      } break;
    }
    return callVector;
  }

  void MultiGraph::SetStatSysStyles(TString opt) {
    int n                 = GetNGraphs();
    Bool_t copyLineColor  = Hal::Std::FindParam(opt, "line");
    Bool_t copyFillColor  = Hal::Std::FindParam(opt, "fill");
    Bool_t clearFillStyle = Hal::Std::FindParam(opt, "clear");
    Int_t fillStyle       = -1;
    auto bracket          = Hal::Std::FindBrackets(opt);
    if (bracket.size()) {
      auto bra  = bracket[0];
      fillStyle = bra.Atoi();
    }
    for (int i = 0; i < n; i += 2) {
      auto sys  = fGraphErrors[i];
      auto stat = fGraphErrors[i + 1];
      if (fillStyle > 0) { sys->SetFillStyle(fillStyle); }
      if (copyLineColor) sys->SetLineColor(stat->GetMarkerColor());
      if (copyFillColor) sys->SetFillColor(stat->GetMarkerColor());
      if (clearFillStyle) {
        sys->SetFillStyle(-1);
      }
    }
  }

}  // namespace Hal
