/*
 * PaveStyle.cxx
 *
 *  Created on: 19 lis 2026
 *      Author: daniel
 */

#include "PaveStyle.h"

#include <TPave.h>
#include <TPaveText.h>
#include <TString.h>
#include <TStyle.h>

#include <iostream>

#include "Std.h"
#include "Style.h"
#include "TextStyle.h"
#include "XMLNode.h"

namespace Hal {

  PaveStyle::PaveStyle(TString style) : Style(style) {
    if (Hal::Std::FindParam(style, Std::PredefinedStyle::Black(), kTRUE)) {
      fFill.SetColor(kBlack);
      fText.SetColor(kWhite);
      fLine.SetColor(kWhite);
      SetShadowColor(kWhite);
    }
    if (Hal::Std::FindParam(style, Std::PredefinedStyle::Default(), kTRUE)) {
      fFill.SetColor(kWhite);
      fText.SetColor(kBlack);
      fLine.SetColor(kBlack);
      SetShadowColor(kBlack);
    }
    if (Hal::Std::FindParam(style, Std::PredefinedStyle::Apollo(), kTRUE)) { fText.SetFont(82); }
  }

  void PaveStyle::SetBorderSize(Int_t size) { SetI(EBitFlag::kBorderSize, size); }

  void PaveStyle::SetCornerRadis(Double_t size) { SetF(EBitFlag::kCornerRadius, size); }

  void PaveStyle::SetShadowColor(Int_t color) { SetI(EBitFlag::kShadowColor, color); }

  Int_t PaveStyle::GetBorderSize() const { return GetI(EBitFlag::kBorderSize); }

  Int_t PaveStyle::GetShadowColor() const { return GetI(EBitFlag::kShadowColor); }

  Double_t PaveStyle::GetCornerRadius() const { return GetF(EBitFlag::kCornerRadius); }

  void PaveStyle::ExportToXML(XMLNode& node) const {
    if (Find(EBitFlag::kBorderSize)) node.AddAttrib("BorderSize", Form("%d", GetBorderSize()));
    if (Find(EBitFlag::kShadowColor)) node.AddAttrib("ShadowColor", Form("%d", GetShadowColor()));
    if (Find(EBitFlag::kCornerRadius)) node.AddAttrib("CornerRadius", Form("%4.4f", GetCornerRadius()));
    XMLNode fill("Fill");
    XMLNode text("Text");
    XMLNode line("Line");
    fFill.ExportToXML(fill);
    fText.ExportToXML(text);
    fLine.ExportToXML(line);
    node.AddChild(fill);
    node.AddChild(text);
    node.AddChild(line);
  }

  void PaveStyle::ImportFromXML(const XMLNode& node) {
    if (auto& atr = node.GetAttrib("BorderSize"); !atr.IsNull()) {
      TString x = atr.GetValue();
      SetBorderSize(x.Atoi());
    }
    if (auto& atr = node.GetAttrib("ShadowColor"); !atr.IsNull()) {
      TString x = atr.GetValue();
      SetShadowColor(x.Atoi());
    }
    if (auto& atr = node.GetAttrib("CornerRadius"); !atr.IsNull()) {
      TString x = atr.GetValue();
      SetCornerRadis(x.Atoi());
    }

    if (auto xFill = node.GetChild("Fill")) { fFill.ImportFromXML(xFill); }
    if (auto xFill = node.GetChild("Line")) { fLine.ImportFromXML(xFill); }
    if (auto xFill = node.GetChild("Text")) { fText.ImportFromXML(xFill); }
  }

  void PaveStyle::Apply(TPave& leg) const {
    if (Find(EBitFlag::kBorderSize)) leg.SetBorderSize(GetF(EBitFlag::kBorderSize));
    if (Find(EBitFlag::kShadowColor)) leg.SetShadowColor(GetF(EBitFlag::kShadowColor));
    if (Find(EBitFlag::kCornerRadius)) leg.SetCornerRadius(GetF(EBitFlag::kCornerRadius));
    fFill.ApplyDirect(leg);
    fLine.ApplyDirect(leg);
    if (leg.InheritsFrom("TPaveText")) { fText.ApplyDirect((TPaveText&) leg); }
  }

  void PaveStyle::SetGlobal(TString opt) const {
    if (Hal::Std::FindParam(opt, "stat")) {
      if (Find(EBitFlag::kBorderSize)) gStyle->SetStatBorderSize(GetF(EBitFlag::kBorderSize));
      if (fText.Find(Hal::TextStyle::EBitFlag::kColor)) gStyle->SetStatTextColor(fText.GetColor());
      if (fText.Find(Hal::TextStyle::EBitFlag::kFont)) gStyle->SetStatFont(fText.GetFont());
      if (fText.Find(Hal::TextStyle::EBitFlag::kSize)) gStyle->SetStatFontSize(fText.GetSize());
      if (fFill.Find(Hal::FillStyle::EBitFlag::kColor)) gStyle->SetStatColor(fFill.GetColor());
    } else {
      if (fText.Find(Hal::TextStyle::EBitFlag::kColor)) gStyle->SetTextColor(fText.GetColor());
      if (fText.Find(Hal::TextStyle::EBitFlag::kFont)) gStyle->SetTextFont(fText.GetFont());
      if (fText.Find(Hal::TextStyle::EBitFlag::kSize)) gStyle->SetTextSize(fText.GetSize());
      if (fFill.Find(Hal::FillStyle::EBitFlag::kColor)) gStyle->SetFillColor(fFill.GetColor());
      if (fLine.Find(Hal::LineStyle::EBitFlag::kColor)) gStyle->SetLineColor(fLine.GetColor());
      if (fLine.Find(Hal::LineStyle::EBitFlag::kStyle)) gStyle->SetLineStyle(fLine.GetStyle());
      if (fLine.Find(Hal::LineStyle::EBitFlag::kWidth)) gStyle->SetLineWidth(fLine.GetWidth());
    }
  }
} /* namespace Hal */
