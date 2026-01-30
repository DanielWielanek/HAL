/*
 * AxisStyle.cxx
 *
 *  Created on: 30 lip 2024
 *      Author: daniel
 */

#include "AxisStyle.h"

#include <RtypesCore.h>
#include <TAxis.h>
#include <TMath.h>
#include <TString.h>

#include "XMLNode.h"

namespace Hal {

  void AxisStyle::SetTitleOffset(Float_t val) { SetF(EBitFlag::kTitleOffset, val); }

  void AxisStyle::SetLabelOffset(Float_t val) { SetF(EBitFlag::kLabelOffset, val); }

  void AxisStyle::SetTitleSize(Float_t val) { SetF(EBitFlag::kTitleSize, val); }

  void AxisStyle::SetLabelSize(Float_t val) { SetF(EBitFlag::kLabelSize, val); }

  void AxisStyle::SetTickLength(Float_t val) { SetF(EBitFlag::kTickLength, val); }

  void AxisStyle::SetNdivisions(Int_t val, Bool_t optim) {
    val = TMath::Abs(val);
    if (!optim) val = -val;
    SetI(EBitFlag::kNdivisions, val);
  }

  void AxisStyle::SetAxisColor(Int_t val) { SetI(EBitFlag::kAxisColor, val); }

  void AxisStyle::SetColor(Int_t col) {
    SetAxisColor(col);
    SetLabelColor(col);
    SetTitleColor(col);
  }

  void AxisStyle::SetLabelColor(Int_t val) { SetI(EBitFlag::kLabelColor, val); }

  void AxisStyle::SetTitleColor(Int_t val) { SetI(EBitFlag::kTitleColor, val); }

  void AxisStyle::SetTitleFont(Int_t val) { SetI(EBitFlag::kTitleFont, val); }

  void AxisStyle::SetCenterTitle(Int_t val) { SetI(EBitFlag::kCenterTitle, val); }

  void AxisStyle::SetLabelFont(Int_t font) { SetI(EBitFlag::kFontStyleLabel, font); };

  void AxisStyle::SetRotateTitle(Bool_t rot) { SetI(EBitFlag::kRotatedTitle, rot); };

  void AxisStyle::SetNoExponent(Bool_t rot) { SetI(EBitFlag::kNoExp, rot); };

  void AxisStyle::SetDecimals(Bool_t opt) { SetI(EBitFlag::kDecimal, opt); };

  Float_t AxisStyle::GetTitleOffset() const { return GetF(EBitFlag::kTitleOffset); }

  Float_t AxisStyle::GetLabelOffset() const { return GetF(EBitFlag::kLabelOffset); }

  Float_t AxisStyle::GetTitleSize() const { return GetF(EBitFlag::kTitleSize); }

  Float_t AxisStyle::GetLabelSize() const { return GetF(EBitFlag::kLabelSize); }

  Float_t AxisStyle::GetTickLength() const { return GetF(EBitFlag::kTickLength); }

  Int_t AxisStyle::GetNDivisions() const { return TMath::Abs(GetI(EBitFlag::kNdivisions)); }

  Int_t AxisStyle::GetAxisColor() const { return GetI(EBitFlag::kAxisColor); }

  Int_t AxisStyle::GetLabelColor() const { return GetI(EBitFlag::kLabelColor); }

  Int_t AxisStyle::GetTitleColor() const { return GetI(EBitFlag::kTitleColor); }

  Int_t AxisStyle::GetTitleFont() const { return GetI(EBitFlag::kTitleFont); }

  Int_t AxisStyle::GetCenterTitle() const { return GetI(EBitFlag::kCenterTitle); }

  Int_t AxisStyle::GetLabelFont() const { return GetI(EBitFlag::kFontStyleLabel); };

  Bool_t AxisStyle::GetRotatedTitle() const { return GetI(EBitFlag::kRotatedTitle); };

  Bool_t AxisStyle::GetDecimal() const { return GetI(EBitFlag::kDecimal); };

  Bool_t AxisStyle::GetMoreLogLabels() const { return GetI(EBitFlag::kMoreLog); };

  Bool_t AxisStyle::GetNoExponent() const { return GetI(EBitFlag::kNoExp); }

  Bool_t AxisStyle::NDivisionsOptimized() const {
    int div = GetI(EBitFlag::kNdivisions);
    if (div >= 0) return kTRUE;
    return kFALSE;
  }

  void AxisStyle::Apply(TAxis& obj) const {
    if (Find(EBitFlag::kTitleOffset)) obj.SetTitleOffset(GetF(EBitFlag::kTitleOffset));
    if (Find(EBitFlag::kLabelOffset)) obj.SetLabelOffset(GetF(EBitFlag::kLabelOffset));
    if (Find(EBitFlag::kTitleSize)) obj.SetTitleSize(GetF(EBitFlag::kTitleSize));
    if (Find(EBitFlag::kLabelSize)) obj.SetLabelSize(GetF(EBitFlag::kLabelSize));
    if (Find(EBitFlag::kTickLength)) obj.SetTickLength(GetF(EBitFlag::kTickLength));
    if (Find(EBitFlag::kNdivisions)) {
      int div = GetI(EBitFlag::kNdivisions);
      if (div >= 0)
        obj.SetNdivisions(div, true);
      else
        obj.SetNdivisions(-div, false);
    }
    if (Find(EBitFlag::kAxisColor)) obj.SetAxisColor(GetI(EBitFlag::kAxisColor));
    if (Find(EBitFlag::kLabelColor)) obj.SetLabelColor(GetI(EBitFlag::kLabelColor));
    if (Find(EBitFlag::kTitleColor)) obj.SetTitleColor(GetI(EBitFlag::kTitleColor));
    if (Find(EBitFlag::kTitleFont)) obj.SetTitleFont(GetI(EBitFlag::kTitleFont));
    if (Find(EBitFlag::kCenterTitle)) obj.CenterTitle(GetI(EBitFlag::kCenterTitle));
    if (!fSkipTitle)
      if (Find(EBitFlag::kTitle)) obj.SetTitle(fTitle);
    if (Find(EBitFlag::kRangeMin) && Find(EBitFlag::kRangeMin))
      obj.SetRangeUser(GetF(EBitFlag::kRangeMin), GetF(EBitFlag::kRangeMax));
    if (Find(EBitFlag::kTicksOpt)) obj.SetTicks(GetTicks());
    if (Find(EBitFlag::kMoreLog)) obj.SetMoreLogLabels(GetI(EBitFlag::kMoreLog));
    if (Find(EBitFlag::kFontStyleLabel)) obj.SetLabelFont(GetI(EBitFlag::kFontStyleLabel));
    if (Find(EBitFlag::kRotatedTitle)) { obj.RotateTitle(GetI(EBitFlag::kRotatedTitle)); }
    if (Find(EBitFlag::kDecimal)) obj.SetDecimals(GetI(EBitFlag::kDecimal));
    if (Find(EBitFlag::kNoExp)) obj.SetNoExponent(GetI(EBitFlag::kNoExp));
  };

  void AxisStyle::SetTitle(TString val) {
    SetI(EBitFlag::kTitle, 0);
    fTitle = val;
  }

  void AxisStyle::ExportToXML(XMLNode& node) const {
    if (Find(EBitFlag::kTitleOffset)) node.AddAttrib("TitleOffset", Form("%4.4f", GetF(EBitFlag::kTitleOffset)));
    if (Find(EBitFlag::kLabelOffset)) node.AddAttrib("LabelOffset", Form("%4.4f", GetF(EBitFlag::kLabelOffset)));
    if (Find(EBitFlag::kTitleSize)) node.AddAttrib("TitleSize", Form("%4.4f", GetF(EBitFlag::kTitleSize)));
    if (Find(EBitFlag::kLabelSize)) node.AddAttrib("LabelSize", Form("%4.4f", GetF(EBitFlag::kLabelSize)));
    if (Find(EBitFlag::kTickLength)) node.AddAttrib("TickLength", Form("%4.4f", GetF(EBitFlag::kTickLength)));
    if (Find(EBitFlag::kNdivisions)) node.AddAttrib("Ndivisions", Form("%i", GetI(EBitFlag::kNdivisions)));
    if (Find(EBitFlag::kAxisColor)) node.AddAttrib("AxisColor", Form("%i", GetI(EBitFlag::kAxisColor)));
    if (Find(EBitFlag::kLabelColor)) node.AddAttrib("LabelColor", Form("%i", GetI(EBitFlag::kLabelColor)));
    if (Find(EBitFlag::kTitleColor)) node.AddAttrib("TitleColor", Form("%i", GetI(EBitFlag::kTitleColor)));
    if (Find(EBitFlag::kTitleFont)) node.AddAttrib("TitleFont", Form("%i", GetI(EBitFlag::kTitleFont)));
    if (Find(EBitFlag::kCenterTitle)) node.AddAttrib("CenterTitle", Form("%i", GetI(EBitFlag::kCenterTitle)));
    if (!fSkipTitle)
      if (Find(EBitFlag::kTitle)) node.AddAttrib("Title", fTitle);
    if (Find(EBitFlag::kRangeMin)) node.AddAttrib("RangeMin", Form("%4.4f", GetF(EBitFlag::kRangeMin)));
    if (Find(EBitFlag::kRangeMax)) node.AddAttrib("RangeMax", Form("%4.4f", GetF(EBitFlag::kRangeMax)));
    if (Find(EBitFlag::kTicksOpt)) node.AddAttrib("TicksOpt", GetTicks());
    if (Find(EBitFlag::kMoreLog)) node.AddAttrib("MoreLog", Form("%i", (int) GetI(EBitFlag::kMoreLog)));
    if (Find(EBitFlag::kRotatedTitle)) node.AddAttrib("RotatedTitle", Form("%i", (int) GetI(EBitFlag::kRotatedTitle)));
    if (Find(EBitFlag::kFontStyleLabel)) node.AddAttrib("LabelFont", Form("%i", (int) GetI(EBitFlag::kFontStyleLabel)));
    if (Find(EBitFlag::kDecimal)) node.AddAttrib("Decimal", Form("%i", (int) GetI(EBitFlag::kDecimal)));
    if (Find(EBitFlag::kNoExp)) node.AddAttrib("NoExp", Form("%i", (int) GetI(EBitFlag::kNoExp)));
  }

  void AxisStyle::SetRangeUser(Float_t min, Float_t max) {
    SetF(EBitFlag::kRangeMin, min);
    SetF(EBitFlag::kRangeMax, max);
  }

  void AxisStyle::ImportFromXML(const XMLNode& node) {

    if (auto& atr = node.GetAttrib("TitleOffset"); !atr.IsNull()) {
      float x = atr.GetValue().Atof();
      SetTitleOffset(x);
    }
    if (auto& atr = node.GetAttrib("LabelOffset"); !atr.IsNull()) {
      float x = atr.GetValue().Atof();
      SetLabelOffset(x);
    }
    if (auto& atr = node.GetAttrib("TitleSize"); !atr.IsNull()) {
      float x = atr.GetValue().Atof();
      SetTitleSize(x);
    }
    if (auto& atr = node.GetAttrib("LabelSize"); !atr.IsNull()) {
      float x = atr.GetValue().Atof();
      SetLabelSize(x);
    }
    if (auto& atr = node.GetAttrib("TickLength"); !atr.IsNull()) {
      float x = atr.GetValue().Atof();
      SetTickLength(x);
    }
    if (auto& atr = node.GetAttrib("Ndivisions"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      if (x >= 0)
        SetNdivisions(x, true);
      else
        SetNdivisions(x, false);
    }
    if (auto& atr = node.GetAttrib("AxisColor"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetAxisColor(x);
    }
    if (auto& atr = node.GetAttrib("LabelColor"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetLabelColor(x);
    }
    if (auto& atr = node.GetAttrib("TitleColor"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetTitleColor(x);
    }
    if (auto& atr = node.GetAttrib("TitleFont"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetTitleFont(x);
    }
    if (auto& atr = node.GetAttrib("CenterTitle"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetCenterTitle(x);
    }

    if (!node.GetAttrib("RangeMin").IsNull() && !node.GetAttrib("RangeMin").IsNull()) {
      auto atr1  = node.GetAttrib("RangeMin");
      auto atr2  = node.GetAttrib("RangeMax");
      float mini = atr1.GetValue().Atof();
      float maxi = atr2.GetValue().Atof();
      SetRangeUser(mini, maxi);
    }

    if (auto& atr = node.GetAttrib("CenterTitle"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetCenterTitle(x);
    }
    if (auto& atr = node.GetAttrib("Title"); !atr.IsNull()) {
      TString x = atr.GetValue();
      if (!fSkipTitle) SetTitle(x);
    }
    if (auto& atr = node.GetAttrib("TicksOpt"); !atr.IsNull()) {
      TString x = atr.GetValue();
      SetTicks(x);
    }

    if (auto& atr = node.GetAttrib("MoreLog"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetMoreLogLabels(x);
    }

    if (auto& atr = node.GetAttrib("RotatedTitle"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetRotateTitle(x);
    }

    if (auto& atr = node.GetAttrib("LabelFont"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetLabelFont(x);
    }

    if (auto& atr = node.GetAttrib("Decimal"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetDecimals(x);
    }

    if (auto& atr = node.GetAttrib("NoExp"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
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


  void AxisStyle::SetMoreLogLabels(Bool_t val) { SetI(EBitFlag::kMoreLog, val); }

  void AxisStyle::Import(const TAxis& x) {
    TAxis dummy;
    Apply(dummy);
    if (dummy.GetTitleOffset() != x.GetTitleOffset()) SetTitleOffset(x.GetTitleOffset());
    if (dummy.GetLabelOffset() != x.GetLabelOffset()) SetLabelOffset(x.GetLabelOffset());
    if (!fSkipTitle) {
      TString dummyTitle  = dummy.GetTitle();
      TString sourceTitle = x.GetTitle();
      if (dummyTitle != sourceTitle) SetTitle(sourceTitle);
    }
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
    if (opt == "+") SetI(EBitFlag::kTicksOpt, 1);
    if (opt == "-") SetI(EBitFlag::kTicksOpt, 2);
    if (opt == "") SetI(EBitFlag::kTicksOpt, 0);
    if (opt == "+-") SetI(EBitFlag::kTicksOpt, 3);
  }

  TString AxisStyle::GetTicks() const {
    if (!Find(EBitFlag::kTicksOpt)) return "";
    Int_t val = GetI(EBitFlag::kTicksOpt);
    switch (val) {
      case 1: return "+"; break;
      case 2: return "-"; break;
      case 3: return "+-"; break;
      default: break;
    }
    return "";
  }

} /* namespace Hal */
