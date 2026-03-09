/*
 * TextStyle.cxx
 *
 *  Created on: 13 sty 2026
 *      Author: daniel
 */

#include "TextStyle.h"

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

  void TextStyle::SetAlign(Int_t align) { SetI(BitFlag::kAlign, align); }

  void TextStyle::SetColor(Int_t color) { SetI(BitFlag::kColor, color); }

  void TextStyle::SetAngle(Float_t angle) { SetF(BitFlag::kAngle, angle); }

  void TextStyle::SetSize(Float_t size) { SetF(BitFlag::kSize, size); }

  void TextStyle::SetFont(Float_t font) { SetF(BitFlag::kFont, font); }

  Int_t TextStyle::GetAlign() const { return GetI(BitFlag::kAlign); }

  Int_t TextStyle::GetColor() const { return GetI(BitFlag::kColor); }

  Float_t TextStyle::GetAngle() const { return GetF(BitFlag::kAngle); }

  Float_t TextStyle::GetSize() const { return GetF(BitFlag::kSize); }

  Float_t TextStyle::GetFont() const { return GetF(BitFlag::kFont); }

  void TextStyle::Apply(TAttText& obj) const {
    if (Find(BitFlag::kAlign)) obj.SetTextAlign(GetI(BitFlag::kAlign));
    if (Find(BitFlag::kColor)) obj.SetTextColor(GetI(BitFlag::kColor));
    if (Find(BitFlag::kAngle)) obj.SetTextAngle(GetF(BitFlag::kAngle));
    if (Find(BitFlag::kSize)) obj.SetTextSize(GetF(BitFlag::kSize));
    if (Find(BitFlag::kFont)) obj.SetTextFont(GetF(BitFlag::kFont));
  }

  void TextStyle::ExportToXML(XMLNode& node) const {
    if (Find(BitFlag::kAlign)) node.AddAttrib("Align", Form("%i", GetI(BitFlag::kAlign)));
    if (Find(BitFlag::kColor)) node.AddAttrib("Color", Form("%i", GetI(BitFlag::kColor)));
    if (Find(BitFlag::kAngle)) node.AddAttrib("Angle", Form("%4.4f", GetF(BitFlag::kAngle)));
    if (Find(BitFlag::kSize)) node.AddAttrib("Size", Form("%4.4f", GetF(BitFlag::kSize)));
    if (Find(BitFlag::kFont)) node.AddAttrib("Font", Form("%4.4f", GetF(BitFlag::kFont)));
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
    if (def.GetTextSize() != GetSize()) SetSize(def.GetTextSize());
    if (def.GetTextFont() != GetFont()) SetFont(def.GetTextFont());
    if (def.GetTextAngle() != GetAngle()) SetAngle(def.GetTextAngle());
    if (def.GetTextColor() != GetColor()) SetColor(def.GetTextColor());
    if (def.GetTextAlign() != GetAlign()) SetAlign(def.GetTextAlign());
  }

  TextStyle::~TextStyle() {
    // TODO Auto-generated destructor stub
  }


} /* namespace Hal */
