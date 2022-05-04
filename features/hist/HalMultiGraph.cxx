/*
 * HalMultiGraph.cxx
 *
 *  Created on: 5 mar 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "HalMultiGraph.h"

#include <TAttAxis.h>
#include <TAttLine.h>
#include <TAttMarker.h>
#include <TAxis.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TString.h>
#include <TVirtualPad.h>

HalMultiGraph::HalMultiGraph() :
  fN(0), fGraphErrors(nullptr), fMultiGraph(new TMultiGraph()), fXaxis(nullptr), fYaxis(nullptr) {
  fMin[0] = fMin[1] = 0;
  fMax[0] = fMax[1] = 0;
}

void HalMultiGraph::MakeGraph(Option_t* opt) {
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

void HalMultiGraph::SetXRange(Double_t min, Double_t max) {
  fMin[0] = min;
  fMax[0] = max;
}

void HalMultiGraph::SetYRange(Double_t min, Double_t max) {
  fMin[1] = min;
  fMax[1] = max;
}

void HalMultiGraph::SetPoint(Int_t graph, Int_t point, Double_t x, Double_t y) { fGraphErrors[graph]->SetPoint(point, x, y); }

void HalMultiGraph::SetPoint(Int_t graph, Int_t point, Double_t x, Double_t ex, Double_t y, Double_t ey) {
  fGraphErrors[graph]->SetPoint(point, x, y);
  fGraphErrors[graph]->SetPointError(point, ex, ey);
}

TGraphErrors* HalMultiGraph::GetGraph(Int_t no) const { return fGraphErrors[no]; }

void HalMultiGraph::Draw(Option_t* opt) {
  TString option = opt;
  if (option.Contains("A") == kFALSE) option = "A" + option;
  fMultiGraph->Draw(option);
  if (fXaxis) CopyAxis(fXaxis, fMultiGraph->GetXaxis());
  if (fYaxis) CopyAxis(fYaxis, fMultiGraph->GetYaxis());
  if (fMin[0] != fMax[0]) fMultiGraph->GetXaxis()->SetRangeUser(fMin[0], fMax[0]);
  if (fMin[1] != fMax[1]) fMultiGraph->GetYaxis()->SetRangeUser(fMin[1], fMax[1]);
  gPad->Update();
  gPad->RedrawAxis();
}

void HalMultiGraph::SetMarkerStyle(Style_t marker, Int_t no) {
  if (no < 0) {
    for (int i = 0; i < fN; i++)
      fGraphErrors[i]->SetMarkerStyle(marker);
  } else {
    fGraphErrors[no]->SetMarkerStyle(marker);
  }
}

void HalMultiGraph::SetMarkerSize(Size_t size, Int_t no) {
  if (no < 0) {
    for (int i = 0; i < fN; i++)
      fGraphErrors[i]->SetMarkerSize(size);
  } else {
    fGraphErrors[no]->SetMarkerSize(size);
  }
}

void HalMultiGraph::SetMarkerColor(Color_t color, Int_t no) {
  if (no < 0) {
    for (int i = 0; i < fN; i++)
      fGraphErrors[i]->SetMarkerColor(color);
  } else {
    fGraphErrors[no]->SetMarkerColor(color);
  }
}

void HalMultiGraph::SetLineWidth(Width_t width, Int_t no) {
  if (no < 0) {
    for (int i = 0; i < fN; i++)
      fGraphErrors[i]->SetLineWidth(width);
  } else {
    fGraphErrors[no]->SetLineWidth(width);
  }
}

void HalMultiGraph::SetLineColor(Color_t color, Int_t no) {
  if (no < 0) {
    for (int i = 0; i < fN; i++)
      fGraphErrors[i]->SetLineColor(color);
  } else {
    fGraphErrors[no]->SetLineColor(color);
  }
}

void HalMultiGraph::SetLineStyle(Style_t style, Int_t no) {
  if (no < 0) {
    for (int i = 0; i < fN; i++)
      fGraphErrors[i]->SetLineStyle(style);
  } else {
    fGraphErrors[no]->SetLineStyle(style);
  }
}

void HalMultiGraph::SetLineAttributes(Style_t style, Width_t width, Color_t color, Int_t no) {
  SetLineStyle(style, no);
  SetLineWidth(width, no);
  SetLineColor(color, no);
}

void HalMultiGraph::SetMarkerAttributes(Style_t style, Size_t size, Color_t color, Int_t no) {
  SetMarkerStyle(style, no);
  SetMarkerSize(size, no);
  SetMarkerColor(color, no);
}

void HalMultiGraph::MakeGraphs(Int_t no, Option_t* opt) {
  for (int i = 0; i < no; i++) {
    MakeGraph(opt);
  }
}

TMultiGraph* HalMultiGraph::GetMutliGraph() const { return fMultiGraph; }

HalMultiGraph::~HalMultiGraph() {
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

TAxis* HalMultiGraph::GetXaxis() {
  if (fXaxis) return fXaxis;
  fXaxis = new TAxis();
  return fXaxis;
}

void HalMultiGraph::CopyAxis(TAxis* from, TAxis* to) {
  TAxis default_axis = TAxis();
  if (default_axis.GetCenterTitle() != from->GetCenterTitle()) to->CenterTitle(from->GetCenterTitle());
  if (default_axis.GetNdivisions() != from->GetNdivisions()) to->SetNdivisions(from->GetNdivisions());
  if (default_axis.GetTitleColor() != from->GetTitleColor()) to->SetTitleColor(from->GetTitleColor());
  if (default_axis.GetTitleFont() != from->GetTitleFont()) to->SetTitleFont(from->GetTitleFont());
  if (default_axis.GetTitleOffset() != from->GetTitleOffset()) to->SetTitleOffset(from->GetTitleOffset());
  if (default_axis.GetTitleSize() != from->GetTitleSize()) to->SetTitleSize(from->GetTitleSize());
  to->SetTitle(from->GetTitle());
  if (default_axis.GetLabelColor() != from->GetLabelColor()) to->SetLabelColor(from->GetLabelColor());
  if (default_axis.GetLabelFont() != from->GetLabelFont()) to->SetLabelFont(from->GetLabelFont());
  if (default_axis.GetLabelOffset() != from->GetLabelOffset()) to->SetLabelOffset(from->GetLabelOffset());
  if (default_axis.GetLabelSize() != from->GetLabelSize()) to->SetLabelSize(from->GetLabelSize());
}

TAxis* HalMultiGraph::GetYaxis() {
  if (fYaxis) return fYaxis;
  fYaxis = new TAxis();
  return fYaxis;
}
