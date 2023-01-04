/*
 * Style.cxx
 *
 *  Created on: 27 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Style.h"
#include "MultiGraph.h"
#include "Std.h"

#include <initializer_list>

#include <TAttLine.h>
#include <TAttMarker.h>
#include <TAxis.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TVirtualPad.h>

namespace Hal {

  Bool_t Style::Find(TString key, Int_t& val) const {
    auto search = fInts.find(key);
    if (search != fInts.end()) {
      val = search->second;
      return kTRUE;
    } else {
      return kFALSE;
    }
  }

  Bool_t Style::Find(TString key, Double_t& val) const {
    auto search = fDoubles.find(key);
    if (search != fDoubles.end()) {
      val = search->second;
      return kTRUE;
    } else {
      return kFALSE;
    }
  }

  void Style::Register(TString key, Int_t val) { fInts.insert({key, val}); }

  void Style::Register(TString key, Double_t val) { fDoubles.insert({key, val}); }


  // ===================== pad style ===============================================
  PadStyle::PadStyle() {}

  PadStyle::PadStyle(Double_t x1, Double_t y1, Double_t x2, Double_t y2) { SetMargins(x1, y1, x2, y2); }

  void PadStyle::Apply() {
    Double_t d;
    Int_t i;
    if (Find("bottom", d)) { gPad->SetBottomMargin(d); };
    if (Find("top", d)) { gPad->SetTopMargin(d); };
    if (Find("left", d)) { gPad->SetLeftMargin(d); };
    if (Find("right", d)) { gPad->SetRightMargin(d); };
    if (Find("logx", i)) { gPad->SetLogx(i); };
    if (Find("logy", i)) { gPad->SetLogy(i); };
    if (Find("logz", i)) { gPad->SetLogz(i); };
    if (Find("gridx", i)) { gPad->SetGridx(i); };
    if (Find("gridy", i)) { gPad->SetGridy(i); };
  }

  void PadStyle::SetMargins(Double_t x1, Double_t y1, Double_t x2, Double_t y2) {
    SetLeftMargin(x1);
    SetRightMargin(x2);
    SetBottomMargin(y1);
    SetTopMargin(y2);
  }

  void PadStyle::SetLog(Bool_t x, Bool_t y, Bool_t z) {
    SetLogx(x);
    SetLogy(y);
    SetLogz(z);
  }

  //===========================================================================

  HistoStyle::HistoStyle() {}

  void HistoStyle::SetMarkerProperties(Color_t c, Int_t marker, Size_t size) {
    Int_t col = c;
    Int_t mar = marker;
    Int_t siz = size;
    Register({"mCol", "mStyle", "mSize"}, {col, mar, siz});
  }

  void HistoStyle::SetLineProperties(Color_t c, Style_t style, Double_t width) {
    Register("lCol", Int_t(c));
    if (style >= 0) Register("lStyle", Int_t(style));
    if (width >= 0) Register("lWidth", Double_t(width));
  }

  void HistoStyle::SetColor(Color_t c) { Register({"mCol", "lCol"}, {Int_t(c), Int_t(c)}); }

  void HistoStyle::SetAxis(Double_t titleSize, Double_t titleOffset, Double_t labelSize, Double_t labelOffset, Char_t x) {
    TString labels[4] = {"TitSiz", "TitOff", "LabSiz", "LabOff"};
    TString str       = x;
    for (int i = 0; i < 4; i++) {
      labels[i] = str + labels[i];
    }
    if (titleSize >= 0) Register(labels[0], titleSize);
    if (titleOffset >= 0) Register(labels[1], titleOffset);
    if (labelSize >= 0) Register(labels[2], labelSize);
    if (labelOffset >= 0) Register(labels[3], labelOffset);
  }

  void HistoStyle::SetRangeUser(Double_t min, Double_t max, Char_t ax) {
    Register(Form("%cRangeMin", ax), min);
    Register(Form("%cRangeMax", ax), max);
  }

  void HistoStyle::SetMin(Double_t val) { Register("min", val); }

  void HistoStyle::SetMax(Double_t val) { Register("max", val); }

  void HistoStyle::SetMinMax(Double_t minVal, Double_t maxVal) {
    SetMin(minVal);
    SetMax(maxVal);
  }

  void HistoStyle::SetTitles(TString histo, TString xAxis, TString yAxis, TString zAxis) {
    fTitle      = histo;
    fAxTitle[0] = xAxis;
    fAxTitle[1] = yAxis;
    fAxTitle[2] = zAxis;
  }

  void Style::Register(std::initializer_list<TString> keys, std::initializer_list<Int_t> vals) {
    if (keys.size() != vals.size()) return;
    auto keyVec = Hal::Std::GetVector(keys);
    auto valVec = Hal::Std::GetVector(vals);
    for (unsigned int i = 0; i < keyVec.size(); i++) {
      Register(keyVec[i], valVec[i]);
    }
  }

  void Style::Register(std::initializer_list<TString> keys, std::initializer_list<Double_t> vals) {
    if (keys.size() != vals.size()) return;
    auto keyVec = Hal::Std::GetVector(keys);
    auto valVec = Hal::Std::GetVector(vals);
    for (unsigned int i = 0; i < keyVec.size(); i++) {
      Register(keyVec[i], valVec[i]);
    }
  }

  void HistoStyle::ApplyMGraph(MultiGraph& g, Int_t no) {
    Int_t i;
    Double_t d, e;

    if (fTitle.Length()) g.GetGraph(0)->SetTitle(fTitle);

    auto applyAxis = [&](int n) {
      if (Find("mCol", i)) g.SetMarkerColor(i, n);
      if (Find("mStyle", i)) g.SetMarkerStyle(i, n);
      if (Find("mSize", i)) g.SetMarkerSize(i, n);
      if (Find("lCol", i)) g.SetLineColor(i, n);
      if (Find("lStyle", i)) g.SetLineStyle(i, n);
      if (Find("lWidth", d)) g.SetLineWidth(d, n);
    };

    if (no < -1) {
      for (int j = 0; j < g.GetNGraphs(); j++) {
        applyAxis(j);
      }
    } else {
      applyAxis(no);
    }


    //   if (Find("min", d)) h.SetMinimum(d);
    //   if (Find("max", d)) h.SetMaximum(d);
    TString lab[3]    = {'x', 'y', 'z'};
    TString labels[4] = {"TitSiz", "TitOff", "LabSiz", "LabOff"};
    for (int iAx = 0; iAx < 2; iAx++) {

      TString newLabs[4];
      for (int j = 0; j < 4; j++) {
        newLabs[j] = lab[iAx] + labels[j];
      }
      TAxis* ax = nullptr;
      switch (iAx) {
        case 0: ax = g.GetXaxis(); break;
        case 1: ax = g.GetYaxis(); break;
        default: break;
      }
      if (!ax) continue;
      if (fAxTitle[iAx].Length()) ax->SetTitle(fAxTitle[iAx]);
      if (Find(Form("%sTitSiz", lab[iAx].Data()), d)) ax->SetTitleSize(d);
      if (Find(Form("%sTitOff", lab[iAx].Data()), d)) ax->SetTitleOffset(d);
      if (Find(Form("%sLabSiz", lab[iAx].Data()), d)) ax->SetLabelSize(d);
      if (Find(Form("%sLabOf", lab[iAx].Data()), d)) ax->SetLabelOffset(d);
      if (Find(Form("%sRangeMin", lab[iAx].Data()), d) && Find(Form("%sRangeMax", lab[iAx].Data()), e)) {
        if (iAx == 0) {
          g.SetXRange(d, e);
        } else {
          g.SetYRange(d, e);
        }
      }
    }
  }

  void HistoStyle::ApplyTGraph(TGraph& gr) {
    Int_t i;
    Double_t d, e;
    if (fTitle.Length()) gr.SetTitle(fTitle);
    if (Find("mCol", i)) gr.SetMarkerColor(i);
    if (Find("mStyle", i)) gr.SetMarkerStyle(i);
    if (Find("mSize", i)) gr.SetMarkerSize(i);
    if (Find("lCol", i)) gr.SetLineColor(i);
    if (Find("lStyle", i)) gr.SetLineStyle(i);
    if (Find("lWidth", d)) gr.SetLineWidth(d);
    if (Find("min", d)) gr.SetMinimum(d);
    if (Find("max", d)) gr.SetMaximum(d);
    TString lab[3]    = {'x', 'y', 'z'};
    TString labels[4] = {"TitSiz", "TitOff", "LabSiz", "LabOff"};
    for (int iAx = 0; iAx < 2; iAx++) {

      TString newLabs[4];
      for (int j = 0; j < 4; j++) {
        newLabs[j] = lab[iAx] + labels[j];
      }
      TAxis* ax = nullptr;
      switch (iAx) {
        case 0: ax = gr.GetXaxis(); break;
        case 1: ax = gr.GetYaxis(); break;
        default: break;
      }
      if (!ax) continue;
      if (fAxTitle[iAx].Length()) ax->SetTitle(fAxTitle[iAx]);
      if (Find(Form("%sTitSiz", lab[iAx].Data()), d)) ax->SetTitleSize(d);
      if (Find(Form("%sTitOff", lab[iAx].Data()), d)) ax->SetTitleOffset(d);
      if (Find(Form("%sLabSiz", lab[iAx].Data()), d)) ax->SetLabelSize(d);
      if (Find(Form("%sLabOf", lab[iAx].Data()), d)) ax->SetLabelOffset(d);
      if (Find(Form("%sRangeMin", lab[iAx].Data()), d) && Find(Form("%sRangeMax", lab[iAx].Data()), e)) ax->SetRangeUser(d, e);
    }
  }

  void HistoStyle::Apply(TObject& h, Int_t no) {
    if (h.InheritsFrom("TH1")) { ApplyTH(dynamic_cast<TH1&>(h)); }
    if (h.InheritsFrom("Hal::MultiGraph")) { ApplyMGraph(dynamic_cast<Hal::MultiGraph&>(h), no); }
    if (h.InheritsFrom("TGraph")) { ApplyTGraph(dynamic_cast<TGraph&>(h)); }
  }

  void HistoStyle::ApplyTH(TH1& h) {
    Int_t i;
    Double_t d, e;
    if (fTitle.Length()) h.SetTitle(fTitle);
    if (Find("mCol", i)) h.SetMarkerColor(i);
    if (Find("mStyle", i)) h.SetMarkerStyle(i);
    if (Find("mSize", i)) h.SetMarkerSize(i);
    if (Find("lCol", i)) h.SetLineColor(i);
    if (Find("lStyle", i)) h.SetLineStyle(i);
    if (Find("lWidth", d)) h.SetLineWidth(d);
    if (Find("min", d)) h.SetMinimum(d);
    if (Find("max", d)) h.SetMaximum(d);
    TString lab[3]    = {'x', 'y', 'z'};
    TString labels[4] = {"TitSiz", "TitOff", "LabSiz", "LabOff"};
    for (int iAx = 0; iAx < 3; iAx++) {

      TString newLabs[4];
      for (int j = 0; j < 4; j++) {
        newLabs[j] = lab[iAx] + labels[j];
      }
      TAxis* ax = nullptr;
      switch (iAx) {
        case 0: ax = h.GetXaxis(); break;
        case 1: ax = h.GetYaxis(); break;
        case 2: ax = h.GetZaxis(); break;
        default: break;
      }
      if (!ax) continue;
      if (fAxTitle[iAx].Length()) ax->SetTitle(fAxTitle[iAx]);
      if (Find(Form("%sTitSiz", lab[iAx].Data()), d)) ax->SetTitleSize(d);
      if (Find(Form("%sTitOff", lab[iAx].Data()), d)) ax->SetTitleOffset(d);
      if (Find(Form("%sLabSiz", lab[iAx].Data()), d)) ax->SetLabelSize(d);
      if (Find(Form("%sLabOf", lab[iAx].Data()), d)) ax->SetLabelOffset(d);
      if (Find(Form("%sRangeMin", lab[iAx].Data()), d) && Find(Form("%sRangeMax", lab[iAx].Data()), e)) ax->SetRangeUser(d, e);
    }
  }

  void HistoStyle::SetMarkerColor(Color_t col) { Register("mCol", col); }

  void HistoStyle::SetMarkerStyle(Int_t marker) { Register("mStyle", marker); }

  void HistoStyle::SetMarkerSize(Size_t size) { Register("mSize", size); }

  void HistoStyle::SetLineWidth(Double_t width) { Register("lWidth", width); }

  void HistoStyle::SetLineColor(Color_t c) { Register("lCol", c); }

  void HistoStyle::SetLineStyle(Style_t s) { Register("lStyle", s); }

} /* namespace Hal */
