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

#include "XMLNode.h"

namespace Hal {
  const unsigned short int PadStyle::kBottomMargin = 0;
  const unsigned short int PadStyle::kLeftMargin   = 1;
  const unsigned short int PadStyle::kTopMargin    = 2;
  const unsigned short int PadStyle::kRightMargin  = 3;
  const unsigned short int PadStyle::kLogx         = 4;
  const unsigned short int PadStyle::kLogy         = 5;
  const unsigned short int PadStyle::kLogz         = 6;
  const unsigned short int PadStyle::kGridx        = 7;
  const unsigned short int PadStyle::kGridy        = 8;
  const unsigned short int PadStyle::kGridz        = 9;

  PadStyle::PadStyle(Double_t x1, Double_t y1, Double_t x2, Double_t y2) {
    SetLeftMargin(x1);
    SetRightMargin(x2);
    SetBottomMargin(y1);
    SetTopMargin(y2);
  }

  void PadStyle::SetBottomMargin(Float_t val) { SetF(kBottomMargin, val); }

  void PadStyle::SetLeftMargin(Float_t val) { SetF(kLeftMargin, val); }

  void PadStyle::SetTopMargin(Float_t val) { SetF(kTopMargin, val); }

  void PadStyle::SetRightMargin(Float_t val) { SetF(kRightMargin, val); }

  void PadStyle::SetLogx(Int_t val) { SetI(kLogx, val); }

  void PadStyle::SetLogy(Int_t val) { SetI(kLogy, val); }

  void PadStyle::SetLogz(Int_t val) { SetI(kLogz, val); }

  void PadStyle::SetGridx(Int_t val) { SetI(kGridx, val); }

  void PadStyle::SetGridy(Int_t val) { SetI(kGridy, val); }

  void PadStyle::SetGridz(Int_t val) { SetI(kGridz, val); }

  Float_t PadStyle::GetBottomMargin() const { return GetF(kBottomMargin); }

  Float_t PadStyle::GetLeftMargin() const { return GetF(kLeftMargin); }

  Float_t PadStyle::GetTopMargin() const { return GetF(kTopMargin); }

  Float_t PadStyle::GetRightMargin() const { return GetF(kRightMargin); }

  Int_t PadStyle::GetLogx(Int_t val) const { return GetI(kLogx); }

  Int_t PadStyle::GetLogy(Int_t val) const { return GetI(kLogy); }

  Int_t PadStyle::GetLogz(Int_t val) const { return GetI(kLogz); }

  Int_t PadStyle::GetGridx(Int_t val) const { return GetI(kGridx); }

  Int_t PadStyle::GetGridy(Int_t val) const { return GetI(kGridy); }

  Int_t PadStyle::GetGridz(Int_t val) const { return GetI(kGridz); }

  PadStyle::PadStyle() {}

  void PadStyle::Apply(TVirtualPad* obj) {
    if (!obj) return;
    if (Find(kBottomMargin)) obj->SetBottomMargin(GetF(kBottomMargin));
    if (Find(kLeftMargin)) obj->SetLeftMargin(GetF(kLeftMargin));
    if (Find(kTopMargin)) obj->SetTopMargin(GetF(kTopMargin));
    if (Find(kRightMargin)) obj->SetRightMargin(GetF(kRightMargin));
    if (Find(kLogx)) obj->SetLogx(GetI(kLogx));
    if (Find(kLogy)) obj->SetLogy(GetI(kLogy));
    if (Find(kLogz)) obj->SetLogz(GetI(kLogz));
    if (Find(kGridx)) obj->SetGridx(GetI(kGridx));
    if (Find(kGridy)) obj->SetGridy(GetI(kGridy));
  }

  void PadStyle::ExportToXML(XMLNode* node) const {

    if (Find(kBottomMargin)) node->AddAttrib(new Hal::XMLAttrib("BottomMargin", Form("%4.4f", GetF(kBottomMargin))));
    if (Find(kLeftMargin)) node->AddAttrib(new Hal::XMLAttrib("LeftMargin", Form("%4.4f", GetF(kLeftMargin))));
    if (Find(kTopMargin)) node->AddAttrib(new Hal::XMLAttrib("TopMargin", Form("%4.4f", GetF(kTopMargin))));
    if (Find(kRightMargin)) node->AddAttrib(new Hal::XMLAttrib("RightMargin", Form("%4.4f", GetF(kRightMargin))));
    if (Find(kLogx)) node->AddAttrib(new Hal::XMLAttrib("Logx", Form("%i", GetI(kLogx))));
    if (Find(kLogy)) node->AddAttrib(new Hal::XMLAttrib("Logy", Form("%i", GetI(kLogy))));
    if (Find(kLogz)) node->AddAttrib(new Hal::XMLAttrib("Logz", Form("%i", GetI(kLogz))));
    if (Find(kGridx)) node->AddAttrib(new Hal::XMLAttrib("Gridx", Form("%i", GetI(kGridx))));
    if (Find(kGridy)) node->AddAttrib(new Hal::XMLAttrib("Gridy", Form("%i", GetI(kGridy))));
    if (Find(kGridz)) node->AddAttrib(new Hal::XMLAttrib("Gridz", Form("%i", GetI(kGridz))));
  }

  void PadStyle::ImportFromXML(XMLNode* node) {

    if (auto atr = node->GetAttrib("BottomMargin")) {
      float x = atr->GetValue().Atof();
      SetBottomMargin(x);
    }
    if (auto atr = node->GetAttrib("LeftMargin")) {
      float x = atr->GetValue().Atof();
      SetLeftMargin(x);
    }
    if (auto atr = node->GetAttrib("TopMargin")) {
      float x = atr->GetValue().Atof();
      SetTopMargin(x);
    }
    if (auto atr = node->GetAttrib("RightMargin")) {
      float x = atr->GetValue().Atof();
      SetRightMargin(x);
    }
    if (auto atr = node->GetAttrib("Logx")) {
      int x = atr->GetValue().Atoi();
      SetLogx(x);
    }
    if (auto atr = node->GetAttrib("Logy")) {
      int x = atr->GetValue().Atoi();
      SetLogy(x);
    }
    if (auto atr = node->GetAttrib("Logz")) {
      int x = atr->GetValue().Atoi();
      SetLogz(x);
    }
    if (auto atr = node->GetAttrib("Gridx")) {
      int x = atr->GetValue().Atoi();
      SetGridx(x);
    }
    if (auto atr = node->GetAttrib("Gridy")) {
      int x = atr->GetValue().Atoi();
      SetGridy(x);
    }
    if (auto atr = node->GetAttrib("Gridz")) {
      int x = atr->GetValue().Atoi();
      SetGridz(x);
    }
  }
} /* namespace Hal */
