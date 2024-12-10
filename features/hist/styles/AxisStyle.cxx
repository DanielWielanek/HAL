/*
 * AxisStyle.cxx
 *
 *  Created on: 30 lip 2024
 *      Author: daniel
 */

#include "AxisStyle.h"

#include <RtypesCore.h>
#include <TAxis.h>
#include <TString.h>

#include "XMLNode.h"

namespace Hal {
  const unsigned short int AxisStyle::kTitleOffset    = 0;
  const unsigned short int AxisStyle::kLabelOffset    = 1;
  const unsigned short int AxisStyle::kTitleSize      = 2;
  const unsigned short int AxisStyle::kLabelSize      = 3;
  const unsigned short int AxisStyle::kTickLength     = 4;
  const unsigned short int AxisStyle::kNdivisions     = 5;
  const unsigned short int AxisStyle::kAxisColor      = 6;
  const unsigned short int AxisStyle::kLabelColor     = 7;
  const unsigned short int AxisStyle::kTitleColor     = 8;
  const unsigned short int AxisStyle::kTitleFont      = 9;
  const unsigned short int AxisStyle::kCenterTitle    = 10;
  const unsigned short int AxisStyle::kRangeMin       = 11;
  const unsigned short int AxisStyle::kRangeMax       = 12;
  const unsigned short int AxisStyle::kTitle          = 13;
  const unsigned short int AxisStyle::kTicksOpt       = 14;
  const unsigned short int AxisStyle::kMoreLog        = 15;
  const unsigned short int AxisStyle::kRotatedTitle   = 16;
  const unsigned short int AxisStyle::kFontStyleLabel = 17;
  const unsigned short int AxisStyle::kDecimal        = 18;
  const unsigned short int AxisStyle::kNoExp          = 19;

  void AxisStyle::SetTitleOffset(Float_t val) { SetF(kTitleOffset, val); }

  void AxisStyle::SetLabelOffset(Float_t val) { SetF(kLabelOffset, val); }

  void AxisStyle::SetTitleSize(Float_t val) { SetF(kTitleSize, val); }

  void AxisStyle::SetLabelSize(Float_t val) { SetF(kLabelSize, val); }

  void AxisStyle::SetTickLength(Float_t val) { SetF(kTickLength, val); }

  void AxisStyle::SetNdivisions(Int_t val, Bool_t optim) {
    val = TMath::Abs(val);
    if (!optim) val = -val;
    SetI(kNdivisions, val);
  }

  void AxisStyle::SetAxisColor(Int_t val) { SetI(kAxisColor, val); }

  void AxisStyle::SetLabelColor(Int_t val) { SetI(kLabelColor, val); }

  void AxisStyle::SetTitleColor(Int_t val) { SetI(kTitleColor, val); }

  void AxisStyle::SetTitleFont(Int_t val) { SetI(kTitleFont, val); }

  void AxisStyle::SetCenterTitle(Int_t val) { SetI(kCenterTitle, val); }

  void AxisStyle::SetLabelFont(Int_t font) { SetI(kFontStyleLabel, font); };

  void AxisStyle::SetRotateTitle(Bool_t rot) { SetI(kRotatedTitle, rot); };

  void AxisStyle::SetNoExponent(Bool_t rot) { SetI(kNoExp, rot); };

  void AxisStyle::SetDecimals(Bool_t opt) { SetI(kDecimal, opt); };

  Float_t AxisStyle::GetTitleOffset() const { return GetF(kTitleOffset); }

  Float_t AxisStyle::GetLabelOffset() const { return GetF(kLabelOffset); }

  Float_t AxisStyle::GetTitleSize() const { return GetF(kTitleSize); }

  Float_t AxisStyle::GetLabelSize() const { return GetF(kLabelSize); }

  Float_t AxisStyle::GetTickLength() const { return GetF(kTickLength); }

  Int_t AxisStyle::GetNDivisions() const { return TMath::Abs(GetI(kNdivisions)); }

  Int_t AxisStyle::GetAxisColor() const { return GetI(kAxisColor); }

  Int_t AxisStyle::GetLabelColor() const { return GetI(kLabelColor); }

  Int_t AxisStyle::GetTitleColor() const { return GetI(kTitleColor); }

  Int_t AxisStyle::GetTitleFont() const { return GetI(kTitleFont); }

  Int_t AxisStyle::GetCenterTitle() const { return GetI(kCenterTitle); }

  Int_t AxisStyle::GetLabelFont() const { return GetI(kFontStyleLabel); };

  Bool_t AxisStyle::GetRotatedTitle() const { return GetI(kRotatedTitle); };

  Bool_t AxisStyle::GetDecimal() const { return GetI(kDecimal); };

  Bool_t AxisStyle::GetMoreLogLabels() const { return GetI(kMoreLog); };

  Bool_t AxisStyle::GetNoExponent() const { return GetI(kNoExp); }

  Bool_t AxisStyle::NDivisionsOptimized() const {
    int div = GetI(kNdivisions);
    if (div >= 0) return kTRUE;
    return kFALSE;
  }

  void AxisStyle::Apply(TAxis& obj) const {
    if (Find(kTitleOffset)) obj.SetTitleOffset(GetF(kTitleOffset));
    if (Find(kLabelOffset)) obj.SetLabelOffset(GetF(kLabelOffset));
    if (Find(kTitleSize)) obj.SetTitleSize(GetF(kTitleSize));
    if (Find(kLabelSize)) obj.SetLabelSize(GetF(kLabelSize));
    if (Find(kTickLength)) obj.SetTickLength(GetF(kTickLength));
    if (Find(kNdivisions)) {
      int div = GetI(kNdivisions);
      if (div >= 0)
        obj.SetNdivisions(div, true);
      else
        obj.SetNdivisions(-div, false);
    }
    if (Find(kAxisColor)) obj.SetAxisColor(GetI(kAxisColor));
    if (Find(kLabelColor)) obj.SetLabelColor(GetI(kLabelColor));
    if (Find(kTitleColor)) obj.SetTitleColor(GetI(kTitleColor));
    if (Find(kTitleFont)) obj.SetTitleFont(GetI(kTitleFont));
    if (Find(kCenterTitle)) obj.CenterTitle(GetI(kCenterTitle));
    if (!fSkipTitle)
      if (Find(kTitle)) obj.SetTitle(fTitle);
    if (Find(kRangeMin) && Find(kRangeMin)) obj.SetRangeUser(GetF(kRangeMin), GetF(kRangeMax));
    if (Find(kTicksOpt)) obj.SetTicks(GetTicks());
    if (Find(kMoreLog)) obj.SetMoreLogLabels(GetI(kMoreLog));
    if (Find(kFontStyleLabel)) obj.SetLabelFont(GetI(kFontStyleLabel));
    if (Find(kRotatedTitle)) { obj.RotateTitle(GetI(kRotatedTitle)); }
    if (Find(kDecimal)) obj.SetDecimals(GetI(kDecimal));
    if (Find(kNoExp)) obj.SetNoExponent(GetI(kNoExp));
  };

  void AxisStyle::SetTitle(TString val) {
    SetI(kTitle, 0);
    fTitle = val;
  }

  void AxisStyle::ExportToXML(XMLNode* node) const {
    if (Find(kTitleOffset)) node->AddAttrib(new Hal::XMLAttrib("TitleOffset", Form("%4.4f", GetF(kTitleOffset))));
    if (Find(kLabelOffset)) node->AddAttrib(new Hal::XMLAttrib("LabelOffset", Form("%4.4f", GetF(kLabelOffset))));
    if (Find(kTitleSize)) node->AddAttrib(new Hal::XMLAttrib("TitleSize", Form("%4.4f", GetF(kTitleSize))));
    if (Find(kLabelSize)) node->AddAttrib(new Hal::XMLAttrib("LabelSize", Form("%4.4f", GetF(kLabelSize))));
    if (Find(kTickLength)) node->AddAttrib(new Hal::XMLAttrib("TickLength", Form("%4.4f", GetF(kTickLength))));
    if (Find(kNdivisions)) node->AddAttrib(new Hal::XMLAttrib("Ndivisions", Form("%i", GetI(kNdivisions))));
    if (Find(kAxisColor)) node->AddAttrib(new Hal::XMLAttrib("AxisColor", Form("%i", GetI(kAxisColor))));
    if (Find(kLabelColor)) node->AddAttrib(new Hal::XMLAttrib("LabelColor", Form("%i", GetI(kLabelColor))));
    if (Find(kTitleColor)) node->AddAttrib(new Hal::XMLAttrib("TitleColor", Form("%i", GetI(kTitleColor))));
    if (Find(kTitleFont)) node->AddAttrib(new Hal::XMLAttrib("TitleFont", Form("%i", GetI(kTitleFont))));
    if (Find(kCenterTitle)) node->AddAttrib(new Hal::XMLAttrib("CenterTitle", Form("%i", GetI(kCenterTitle))));
    if (!fSkipTitle)
      if (Find(kTitle)) node->AddAttrib(new Hal::XMLAttrib("Title", fTitle));
    if (Find(kRangeMin)) node->AddAttrib(new Hal::XMLAttrib("RangeMin", Form("%4.4f", GetF(kRangeMin))));
    if (Find(kRangeMax)) node->AddAttrib(new Hal::XMLAttrib("RangeMax", Form("%4.4f", GetF(kRangeMax))));
    if (Find(kTicksOpt)) node->AddAttrib(new Hal::XMLAttrib("TicksOpt", GetTicks()));
    if (Find(kMoreLog)) node->AddAttrib(new Hal::XMLAttrib("MoreLog", Form("%i", (int) GetI(kMoreLog))));
    if (Find(kRotatedTitle)) node->AddAttrib(new Hal::XMLAttrib("RotatedTitle", Form("%i", (int) GetI(kRotatedTitle))));
    if (Find(kFontStyleLabel)) node->AddAttrib(new Hal::XMLAttrib("LabelFont", Form("%i", (int) GetI(kFontStyleLabel))));
    if (Find(kDecimal)) node->AddAttrib(new Hal::XMLAttrib("Decimal", Form("%i", (int) GetI(kDecimal))));
    if (Find(kNoExp)) node->AddAttrib(new Hal::XMLAttrib("NoExp", Form("%i", (int) GetI(kNoExp))));
  }

  void AxisStyle::SetRangeUser(Float_t min, Float_t max) {
    SetF(kRangeMin, min);
    SetF(kRangeMax, max);
  }

  void AxisStyle::ImportFromXML(XMLNode* node) {

    if (auto atr = node->GetAttrib("TitleOffset")) {
      float x = atr->GetValue().Atof();
      SetTitleOffset(x);
    }
    if (auto atr = node->GetAttrib("LabelOffset")) {
      float x = atr->GetValue().Atof();
      SetLabelOffset(x);
    }
    if (auto atr = node->GetAttrib("TitleSize")) {
      float x = atr->GetValue().Atof();
      SetTitleSize(x);
    }
    if (auto atr = node->GetAttrib("LabelSize")) {
      float x = atr->GetValue().Atof();
      SetLabelSize(x);
    }
    if (auto atr = node->GetAttrib("TickLength")) {
      float x = atr->GetValue().Atof();
      SetTickLength(x);
    }
    if (auto atr = node->GetAttrib("Ndivisions")) {
      int x = atr->GetValue().Atoi();
      if (x >= 0)
        SetNdivisions(x, true);
      else
        SetNdivisions(x, false);
    }
    if (auto atr = node->GetAttrib("AxisColor")) {
      int x = atr->GetValue().Atoi();
      SetAxisColor(x);
    }
    if (auto atr = node->GetAttrib("LabelColor")) {
      int x = atr->GetValue().Atoi();
      SetLabelColor(x);
    }
    if (auto atr = node->GetAttrib("TitleColor")) {
      int x = atr->GetValue().Atoi();
      SetTitleColor(x);
    }
    if (auto atr = node->GetAttrib("TitleFont")) {
      int x = atr->GetValue().Atoi();
      SetTitleFont(x);
    }
    if (auto atr = node->GetAttrib("CenterTitle")) {
      int x = atr->GetValue().Atoi();
      SetCenterTitle(x);
    }

    if (node->GetAttrib("RangeMin") && node->GetAttrib("RangeMin")) {
      auto atr1  = node->GetAttrib("RangeMin");
      auto atr2  = node->GetAttrib("RangeMax");
      float mini = atr1->GetValue().Atof();
      float maxi = atr2->GetValue().Atof();
      SetRangeUser(mini, maxi);
    }

    if (auto atr = node->GetAttrib("CenterTitle")) {
      int x = atr->GetValue().Atoi();
      SetCenterTitle(x);
    }
    if (auto atr = node->GetAttrib("Title")) {
      TString x = atr->GetValue();
      if (!fSkipTitle) SetTitle(x);
    }
    if (auto atr = node->GetAttrib("TicksOpt")) {
      TString x = atr->GetValue();
      SetTicks(x);
    }

    if (auto atr = node->GetAttrib("MoreLog")) {
      int x = atr->GetValue().Atoi();
      SetMoreLogLabels(x);
    }

    if (auto atr = node->GetAttrib("RotatedTitle")) {
      int x = atr->GetValue().Atoi();
      SetRotateTitle(x);
    }

    if (auto atr = node->GetAttrib("LabelFont")) {
      int x = atr->GetValue().Atoi();
      SetLabelFont(x);
    }

    if (auto atr = node->GetAttrib("Decimal")) {
      int x = atr->GetValue().Atoi();
      SetDecimals(x);
    }

    if (auto atr = node->GetAttrib("NoExp")) {
      int x = atr->GetValue().Atoi();
      SetNoExponent(x);
    }
  }

  AxisStyle::AxisStyle(Bool_t skipTitle) : fSkipTitle(skipTitle) {}

  AxisStyle::AxisStyle(Double_t titleSize, Double_t labelSize, Double_t titleOffset, Double_t labelOffset) {
    SetTitleSize(titleSize);
    SetLabelSize(labelSize);
    SetTitleOffset(titleOffset);
    SetLabelOffset(labelOffset);
  }


  void AxisStyle::SetMoreLogLabels(Bool_t val) { SetI(kMoreLog, val); }

  void AxisStyle::Import(const TAxis& x) {
    TAxis dummy;
    Apply(dummy);
    if (dummy.GetTitleOffset() != x.GetTitleOffset()) SetTitleOffset(x.GetTitleOffset());
    if (dummy.GetLabelOffset() != x.GetLabelOffset()) SetLabelOffset(x.GetLabelOffset());
    if (!fSkipTitle)
      if (dummy.GetTitleSize() != x.GetTitleSize()) SetTitleSize(x.GetTitleSize());
    if (dummy.GetLabelSize() != x.GetLabelSize()) SetLabelSize(x.GetLabelSize());
    if (dummy.GetTickLength() != x.GetTickLength()) SetTickLength(x.GetTickLength());
    if (dummy.GetNdivisions() != x.GetNdivisions()) SetNdivisions(x.GetNdivisions(), false);
    if (dummy.GetAxisColor() != x.GetAxisColor()) SetAxisColor(x.GetAxisColor());
    if (dummy.GetLabelColor() != x.GetLabelColor()) SetLabelColor(x.GetLabelColor());
    if (dummy.GetTitleColor() != x.GetTitleColor()) SetTitleColor(x.GetTitleColor());
    if (dummy.GetTitleFont() != x.GetTitleFont()) SetTitleFont(x.GetTitleFont());
    if (dummy.GetCenterTitle() != x.GetCenterTitle()) SetCenterTitle(x.GetCenterTitle());
    if (dummy.GetTitle() != x.GetTitle()) SetTitle(x.GetTitle());
    TString topt1 = dummy.GetTicks();
    TString topt2 = x.GetTicks();
    if (topt1 != topt2) SetTicks(x.GetTicks());
    if (dummy.GetMoreLogLabels() != x.GetMoreLogLabels()) SetMoreLogLabels(x.GetMoreLogLabels());
    if (dummy.GetRotateTitle() != x.GetRotateTitle()) SetRotateTitle(x.GetRotateTitle());
    if (dummy.GetLabelFont() != x.GetLabelFont()) SetLabelFont(x.GetLabelFont());
    if (dummy.GetDecimals() != x.GetDecimals()) SetDecimals(x.GetDecimals());
    if (dummy.GetNoExponent() != x.GetNoExponent()) SetNoExponent(x.GetNoExponent());
  }

  void AxisStyle::SetTicks(TString opt) {
    if (opt == "+") SetI(kTicksOpt, 1);
    if (opt == "-") SetI(kTicksOpt, 2);
    if (opt == "") SetI(kTicksOpt, 0);
    if (opt == "+-") SetI(kTicksOpt, 3);
  }

  TString AxisStyle::GetTicks() const {
    if (!Find(kTicksOpt)) return "";
    Int_t val = GetI(kTicksOpt);
    switch (val) {
      case 1: return "+"; break;
      case 2: return "-"; break;
      case 3: return "+-"; break;
      default: break;
    }
    return "";
  }

} /* namespace Hal */
