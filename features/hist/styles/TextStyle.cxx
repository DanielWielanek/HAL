/*
 * TextStyle.cxx
 *
 *  Created on: 13 sty 2026
 *      Author: daniel
 */

#include "TextStyle.h"

#include <TStyle.h>

#include "StdString.h"
#include "XMLNode.h"

namespace Hal {

  TextStyle::TextStyle(TString style) {
    if (style.Contains("black")) { SetColor(kWhite); }
    if (style.Contains("apollo")) { SetFont(82); }
    if (style.Contains("08")) { SetSize(0.08); }
    if (style.Contains("07")) { SetSize(0.07); }
    if (style.Contains("06")) { SetSize(0.06); }
    if (style.Contains("05")) { SetSize(0.05); }
  }

  void TextStyle::SetAlign(Int_t align) { SetI(EBitFlag::kAlign, align); }

  void TextStyle::SetColor(Int_t color) { SetI(EBitFlag::kColor, color); }

  void TextStyle::SetAngle(Float_t angle) { SetF(EBitFlag::kAngle, angle); }

  void TextStyle::SetSize(Float_t size) { SetF(EBitFlag::kSize, size); }

  void TextStyle::SetFont(Float_t font) { SetF(EBitFlag::kFont, font); }

  Int_t TextStyle::GetAlign() const { return GetI(EBitFlag::kAlign); }

  Int_t TextStyle::GetColor() const { return GetI(EBitFlag::kColor); }

  Float_t TextStyle::GetAngle() const { return GetF(EBitFlag::kAngle); }

  Float_t TextStyle::GetSize() const { return GetF(EBitFlag::kSize); }

  Float_t TextStyle::GetFont() const { return GetF(EBitFlag::kFont); }

  void TextStyle::ApplyDirect(TAttText& obj) const {
    if (Find(EBitFlag::kAlign)) obj.SetTextAlign(GetI(EBitFlag::kAlign));
    if (Find(EBitFlag::kColor)) obj.SetTextColor(GetI(EBitFlag::kColor));
    if (Find(EBitFlag::kAngle)) obj.SetTextAngle(GetF(EBitFlag::kAngle));
    if (Find(EBitFlag::kSize)) obj.SetTextSize(GetF(EBitFlag::kSize));
    if (Find(EBitFlag::kFont)) obj.SetTextFont(GetF(EBitFlag::kFont));
  }

  void TextStyle::ExportToXML(XMLNode& node) const {
    if (Find(EBitFlag::kAlign)) node.AddAttrib("Align", Form("%i", GetI(EBitFlag::kAlign)));
    if (Find(EBitFlag::kColor)) node.AddAttrib("Color", Form("%i", GetI(EBitFlag::kColor)));
    if (Find(EBitFlag::kAngle)) node.AddAttrib("Angle", Form("%4.4f", GetF(EBitFlag::kAngle)));
    if (Find(EBitFlag::kSize)) node.AddAttrib("Size", Form("%4.4f", GetF(EBitFlag::kSize)));
    if (Find(EBitFlag::kFont)) node.AddAttrib("Font", Form("%4.4f", GetF(EBitFlag::kFont)));
  }

  void TextStyle::ImportFromXML(const XMLNode& node) {
    if (auto child = node.GetAttrib("Size"); !child.IsNull()) SetSize(child.GetValue().Atof());
    if (auto child = node.GetAttrib("Font"); !child.IsNull()) SetFont(child.GetValue().Atof());
    if (auto child = node.GetAttrib("Angle"); !child.IsNull()) SetAngle(child.GetValue().Atof());
    if (auto child = node.GetAttrib("Color"); !child.IsNull()) { SetColor(child.GetValue().Atoi()); }
    if (auto child = node.GetAttrib("Align"); !child.IsNull()) { SetAlign(child.GetValue().Atoi()); }
  }

  void TextStyle::Import(const TAttText& text) {
    TAttText def;
    def.SetTextColor(kBlack);
    if (def.GetTextSize() != text.GetTextSize()) SetSize(def.GetTextSize());
    if (def.GetTextFont() != text.GetTextFont()) SetFont(def.GetTextFont());
    if (def.GetTextAngle() != text.GetTextAngle()) SetAngle(def.GetTextAngle());
    if (def.GetTextColor() != text.GetTextColor()) SetColor(def.GetTextColor());
    if (def.GetTextAlign() != text.GetTextAlign()) SetAlign(def.GetTextAlign());
  }

  TextStyle::~TextStyle() {
    // TODO Auto-generated destructor stub
  }

  void TextStyle::SetGlobal(TString opt) const {
    if (opt == "") {
      if (Find(EBitFlag::kAlign)) gStyle->SetTextAlign(GetI(EBitFlag::kAlign));
      if (Find(EBitFlag::kColor)) gStyle->SetTextColor(GetI(EBitFlag::kColor));
      if (Find(EBitFlag::kAngle)) gStyle->SetTextAngle(GetF(EBitFlag::kAngle));
      if (Find(EBitFlag::kSize)) gStyle->SetTextSize(GetF(EBitFlag::kSize));
      if (Find(EBitFlag::kFont)) gStyle->SetTextFont(GetF(EBitFlag::kFont));
    } else if (Hal::Std::FindParam(opt, "leg")) {
      if (Find(EBitFlag::kFont)) gStyle->SetLegendFont(GetF(EBitFlag::kFont));
      if (Find(EBitFlag::kSize)) gStyle->SetLegendTextSize(GetF(EBitFlag::kSize));
    } else if (Hal::Std::FindParam(opt, "stat")) {
      if (Find(EBitFlag::kFont)) gStyle->SetStatFont(GetF(EBitFlag::kFont));
      if (Find(EBitFlag::kSize)) gStyle->SetStatFontSize(GetF(EBitFlag::kSize));
      if (Find(EBitFlag::kColor)) gStyle->SetStatTextColor(GetI(EBitFlag::kColor));
    }
  }

} /* namespace Hal */
