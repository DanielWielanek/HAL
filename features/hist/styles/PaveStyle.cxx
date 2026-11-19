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

#include "StdString.h"
#include "Style.h"
#include "TextStyle.h"
#include "XMLNode.h"

namespace Hal {

  PaveStyle::PaveStyle(TString tString) {}

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
  }

  void PaveStyle::Apply(TPaveText& text) const {
    if (Find(EBitFlag::kBorderSize)) text.SetBorderSize(GetF(EBitFlag::kBorderSize));
    if (Find(EBitFlag::kShadowColor)) text.SetShadowColor(GetF(EBitFlag::kShadowColor));
    if (Find(EBitFlag::kCornerRadius)) text.SetCornerRadius(GetF(EBitFlag::kCornerRadius));
    fFill.ApplyDirect(text);
    fText.ApplyDirect(text);
    fLine.ApplyDirect(text);
  }

  void PaveStyle::SetGlobal(TString opt) const {
    if (Hal::Std::FindParam(opt, "stat")) {
      if (Find(EBitFlag::kBorderSize)) gStyle->SetStatBorderSize(GetF(EBitFlag::kBorderSize));
      if (fText.Find(Hal::TextStyle::EBitFlag::kColor)) gStyle->SetStatTextColor(fText.GetColor());
      if (fText.Find(Hal::TextStyle::EBitFlag::kFont)) gStyle->SetStatFont(fText.GetFont());
      if (fText.Find(Hal::TextStyle::EBitFlag::kSize)) gStyle->SetStatFontSize(fText.GetSize());
    } else {
    }
  }
} /* namespace Hal */
