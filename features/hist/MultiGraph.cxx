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
#include <TGraphErrors.h>
#include <TH1D.h>
#include <TMultiGraph.h>
#include <TString.h>
#include <TVirtualPad.h>
#include <iostream>

namespace Hal {

  MultiGraph::MultiGraph() : fN(0), fGraphErrors(nullptr), fMultiGraph(new TMultiGraph()), fXaxis(nullptr), fYaxis(nullptr) {
    fMin[0] = fMin[1] = 0;
    fMax[0] = fMax[1] = 0;
  }
  MultiGraph::MultiGraph(Int_t graphs, Option_t* opt) : MultiGraph() { MakeGraphs(graphs, opt); }

  void MultiGraph::MakeGraph(Option_t* opt) {
    TGraphErrors** temp = fGraphErrors;
    fGraphErrors        = new TGraphErrors*[fN + 1];
    for (int i = 0; i < fN; i++) {
      fGraphErrors[i] = temp[i];
    }
    delete[] temp;
    fGraphErrors[fN] = new TGraphErrors();
    fMultiGraph->Add(fGraphErrors[fN], opt);
    fN++;
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
    fGraphErrors[graph]->SetPointError(point, ex, ey);
  }

  TGraphErrors* MultiGraph::GetGraph(Int_t no) const { return fGraphErrors[no]; }

  void MultiGraph::Draw(Option_t* opt) {
    fDrawn         = kTRUE;
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
    if (no < 0) {
      for (int i = 0; i < fN; i++)
        fGraphErrors[i]->SetMarkerStyle(marker);
    } else if (no < fN) {
      fGraphErrors[no]->SetMarkerStyle(marker);
    } else {
      std::cout << __FILE__ << " " << __LINE__ << "Cannot SetMarkerStyle to graph " << no << std::endl;
    }
  }

  void MultiGraph::SetMarkerSize(Size_t size, Int_t no) {
    if (no < 0) {
      for (int i = 0; i < fN; i++)
        fGraphErrors[i]->SetMarkerSize(size);
    } else if (no < fN) {
      fGraphErrors[no]->SetMarkerSize(size);
    } else {
      std::cout << __FILE__ << " " << __LINE__ << "Cannot SetMarkerSize to graph " << no << std::endl;
    }
  }

  void MultiGraph::SetMarkerColor(Color_t color, Int_t no) {
    if (no < 0) {
      for (int i = 0; i < fN; i++)
        fGraphErrors[i]->SetMarkerColor(color);
    } else if (no < fN) {
      fGraphErrors[no]->SetMarkerColor(color);
    } else {
      std::cout << __FILE__ << " " << __LINE__ << "Cannot SetMarkerColor to graph " << no << std::endl;
    }
  }

  void MultiGraph::SetLineWidth(Width_t width, Int_t no) {
    if (no < 0) {
      for (int i = 0; i < fN; i++)
        fGraphErrors[i]->SetLineWidth(width);
    } else if (no < fN) {
      fGraphErrors[no]->SetLineWidth(width);
    } else {
      std::cout << __FILE__ << " " << __LINE__ << "Cannot SetLineWidth to graph " << no << std::endl;
    }
  }

  void MultiGraph::SetLineColor(Color_t color, Int_t no) {
    if (no < 0) {
      for (int i = 0; i < fN; i++)
        fGraphErrors[i]->SetLineColor(color);
    } else if (no < fN) {
      fGraphErrors[no]->SetLineColor(color);
    } else {
      std::cout << __FILE__ << " " << __LINE__ << "Cannot SetLineColor to graph " << no << std::endl;
    }
  }

  void MultiGraph::SetLineStyle(Style_t style, Int_t no) {
    if (no < 0) {
      for (int i = 0; i < fN; i++)
        fGraphErrors[i]->SetLineStyle(style);
    } else if (no < fN) {
      fGraphErrors[no]->SetLineStyle(style);
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
    if (fGraphErrors) {
      for (int i = 0; i < fN; i++) {
        delete fGraphErrors[i];
      }
    }
    delete[] fGraphErrors;
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

}  // namespace Hal
