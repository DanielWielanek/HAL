/*
 * PadStyle.cpp
 *
 *  Created on: 30 lip 2024
 *      Author: daniel
 */

#include "PadStyle.h"

#include <Rtypes.h>
#include <TString.h>
#include <TVirtualPad.h>

#include "StdString.h"
#include "XMLNode.h"

namespace Hal {

  PadStyle::PadStyle(Double_t x1, Double_t y1, Double_t x2, Double_t y2, TString style) : PadStyle(style) {
    SetLeftMargin(x1);
    SetRightMargin(x2);
    SetBottomMargin(y1);
    SetTopMargin(y2);
  }

  void PadStyle::SetBottomMargin(Float_t val) { SetF(EBitFlag::kBottomMargin, val); }

  void PadStyle::SetLeftMargin(Float_t val) { SetF(EBitFlag::kLeftMargin, val); }

  void PadStyle::SetTopMargin(Float_t val) { SetF(EBitFlag::kTopMargin, val); }

  void PadStyle::SetRightMargin(Float_t val) { SetF(EBitFlag::kRightMargin, val); }

  void PadStyle::SetLogx(Int_t val) { SetI(EBitFlag::kLogx, val); }

  void PadStyle::SetLogy(Int_t val) { SetI(EBitFlag::kLogy, val); }

  void PadStyle::SetLogz(Int_t val) { SetI(EBitFlag::kLogz, val); }

  void PadStyle::SetGridx(Int_t val) { SetI(EBitFlag::kGridx, val); }

  void PadStyle::SetGridy(Int_t val) { SetI(EBitFlag::kGridy, val); }

  void PadStyle::SetGridz(Int_t val) { SetI(EBitFlag::kGridz, val); }

  void PadStyle::SetTickx(Int_t val) { SetI(EBitFlag::kTickX, val); }

  void PadStyle::SetTicky(Int_t val) { SetI(EBitFlag::kTickY, val); }

  Float_t PadStyle::GetBottomMargin() const { return GetF(EBitFlag::kBottomMargin); }

  Float_t PadStyle::GetLeftMargin() const { return GetF(EBitFlag::kLeftMargin); }

  Float_t PadStyle::GetTopMargin() const { return GetF(EBitFlag::kTopMargin); }

  Float_t PadStyle::GetRightMargin() const { return GetF(EBitFlag::kRightMargin); }

  Int_t PadStyle::GetLogx(Int_t val) const { return GetI(EBitFlag::kLogx); }

  Int_t PadStyle::GetLogy(Int_t val) const { return GetI(EBitFlag::kLogy); }

  Int_t PadStyle::GetLogz(Int_t val) const { return GetI(EBitFlag::kLogz); }

  Int_t PadStyle::GetGridx(Int_t val) const { return GetI(EBitFlag::kGridx); }

  Int_t PadStyle::GetGridy(Int_t val) const { return GetI(EBitFlag::kGridy); }

  Int_t PadStyle::GetGridz(Int_t val) const { return GetI(EBitFlag::kGridz); }

  Int_t PadStyle::GetTickx(Int_t val) const { return GetI(EBitFlag::kTickX); }

  Int_t PadStyle::GetTicky(Int_t val) const { return GetI(EBitFlag::kTickY); }

  Int_t PadStyle::GetFillColor() const { return GetI(EBitFlag::kFillColor); }

  Int_t PadStyle::GetLineColor() const { return GetI(EBitFlag::kLineColor); }

  Int_t PadStyle::GetFrameLineColor() const { return GetI(EBitFlag::kFrameLineColor); }

  PadStyle::PadStyle(TString style) {
    if (Hal::Std::FindParam(style, "black")) {
      SetFillColor(kBlack);
      SetLineColor(kBlack);
      SetFrameLineColor(kBlack);
    }
  }

  void PadStyle::Apply(TVirtualPad* obj) {
    if (!obj) return;
    if (Find(EBitFlag::kBottomMargin)) obj->SetBottomMargin(GetF(EBitFlag::kBottomMargin));
    if (Find(EBitFlag::kLeftMargin)) obj->SetLeftMargin(GetF(EBitFlag::kLeftMargin));
    if (Find(EBitFlag::kTopMargin)) obj->SetTopMargin(GetF(EBitFlag::kTopMargin));
    if (Find(EBitFlag::kRightMargin)) obj->SetRightMargin(GetF(EBitFlag::kRightMargin));
    if (Find(EBitFlag::kLogx)) obj->SetLogx(GetI(EBitFlag::kLogx));
    if (Find(EBitFlag::kLogy)) obj->SetLogy(GetI(EBitFlag::kLogy));
    if (Find(EBitFlag::kLogz)) obj->SetLogz(GetI(EBitFlag::kLogz));
    if (Find(EBitFlag::kGridx)) obj->SetGridx(GetI(EBitFlag::kGridx));
    if (Find(EBitFlag::kGridy)) obj->SetGridy(GetI(EBitFlag::kGridy));
    if (Find(EBitFlag::kTickX)) obj->SetTickx(GetI(EBitFlag::kTickX));
    if (Find(EBitFlag::kTickY)) obj->SetTicky(GetI(EBitFlag::kTickY));
    if (Find(EBitFlag::kFillColor)) obj->SetFillColor(GetI(EBitFlag::kFillColor));
    if (Find(EBitFlag::kLineColor)) obj->SetLineColor(GetI(EBitFlag::kLineColor));
    if (Find(EBitFlag::kFrameLineColor)) obj->SetFrameLineColor(GetI(EBitFlag::kFrameLineColor));
  }

  void PadStyle::ExportToXML(XMLNode& node) const {

    if (Find(EBitFlag::kBottomMargin)) node.AddAttrib("BottomMargin", Form("%4.4f", GetF(EBitFlag::kBottomMargin)));
    if (Find(EBitFlag::kLeftMargin)) node.AddAttrib("LeftMargin", Form("%4.4f", GetF(EBitFlag::kLeftMargin)));
    if (Find(EBitFlag::kTopMargin)) node.AddAttrib("TopMargin", Form("%4.4f", GetF(EBitFlag::kTopMargin)));
    if (Find(EBitFlag::kRightMargin)) node.AddAttrib("RightMargin", Form("%4.4f", GetF(EBitFlag::kRightMargin)));
    if (Find(EBitFlag::kLogx)) node.AddAttrib("Logx", Form("%i", GetI(EBitFlag::kLogx)));
    if (Find(EBitFlag::kLogy)) node.AddAttrib("Logy", Form("%i", GetI(EBitFlag::kLogy)));
    if (Find(EBitFlag::kLogz)) node.AddAttrib("Logz", Form("%i", GetI(EBitFlag::kLogz)));
    if (Find(EBitFlag::kGridx)) node.AddAttrib("Gridx", Form("%i", GetI(EBitFlag::kGridx)));
    if (Find(EBitFlag::kGridy)) node.AddAttrib("Gridy", Form("%i", GetI(EBitFlag::kGridy)));
    if (Find(EBitFlag::kGridz)) node.AddAttrib("Gridz", Form("%i", GetI(EBitFlag::kGridz)));
    if (Find(EBitFlag::kTickX)) node.AddAttrib("Tickx", Form("%i", GetI(EBitFlag::kTickX)));
    if (Find(EBitFlag::kTickY)) node.AddAttrib("Ticky", Form("%i", GetI(EBitFlag::kTickY)));
    if (Find(EBitFlag::kFillColor)) node.AddAttrib("FillColor", Form("%i", GetI(EBitFlag::kFillColor)));
    if (Find(EBitFlag::kLineColor)) node.AddAttrib("LineColor", Form("%i", GetI(EBitFlag::kLineColor)));
    if (Find(EBitFlag::kFrameLineColor)) node.AddAttrib("FrameLineColor", Form("%i", GetI(EBitFlag::kFrameLineColor)));
  }

  void PadStyle::ImportFromXML(const XMLNode& node) {

    if (auto& atr = node.GetAttrib("BottomMargin"); !atr.IsNull()) {
      float x = atr.GetValue().Atof();
      SetBottomMargin(x);
    }
    if (auto& atr = node.GetAttrib("LeftMargin"); !atr.IsNull()) {
      float x = atr.GetValue().Atof();
      SetLeftMargin(x);
    }
    if (auto& atr = node.GetAttrib("TopMargin"); !atr.IsNull()) {
      float x = atr.GetValue().Atof();
      SetTopMargin(x);
    }
    if (auto& atr = node.GetAttrib("RightMargin"); !atr.IsNull()) {
      float x = atr.GetValue().Atof();
      SetRightMargin(x);
    }
    if (auto& atr = node.GetAttrib("Logx"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetLogx(x);
    }
    if (auto& atr = node.GetAttrib("Logy"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetLogy(x);
    }
    if (auto& atr = node.GetAttrib("Logz"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetLogz(x);
    }
    if (auto& atr = node.GetAttrib("Gridx"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetGridx(x);
    }
    if (auto& atr = node.GetAttrib("Gridy"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetGridy(x);
    }
    if (auto& atr = node.GetAttrib("Gridz"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetGridz(x);
    }
    if (auto& atr = node.GetAttrib("Tickx"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetTickx(x);
    }
    if (auto& atr = node.GetAttrib("Ticky"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetTicky(x);
    }
    if (auto& atr = node.GetAttrib("FillColor"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetFillColor(x);
    }
    if (auto& atr = node.GetAttrib("LineColor"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetLineColor(x);
    }
    if (auto& atr = node.GetAttrib("FrameLineColor"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetFrameLineColor(x);
    }
  }

  void PadStyle::Import(TVirtualPad& pad) {
    SetLeftMargin(pad.GetLeftMargin());
    SetRightMargin(pad.GetRightMargin());
    SetBottomMargin(pad.GetBottomMargin());
    SetTopMargin(pad.GetTopMargin());
    SetGridx(pad.GetGridx());
    SetGridy(pad.GetGridy());
    SetLogx(pad.GetLogx());
    SetLogy(pad.GetLogy());
    SetLogz(pad.GetLogz());
    SetTickx(pad.GetTickx());
    SetTicky(pad.GetTicky());
    SetFillColor(pad.GetFillColor());
    SetLineColor(pad.GetFrameLineColor());
    SetFrameLineColor(pad.GetFrameLineColor());
  }

  void PadStyle::SetFillColor(Int_t col) { SetI(EBitFlag::kFillColor, col); }

  void PadStyle::SetLineColor(Int_t col) { SetI(EBitFlag::kLineColor, col); }

  void PadStyle::SetFrameLineColor(Int_t col) { SetI(EBitFlag::kFrameLineColor, col); }

} /* namespace Hal */
