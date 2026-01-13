/*
 * TextStyle.cxx
 *
 *  Created on: 13 sty 2026
 *      Author: daniel
 */

#include "TextStyle.h"

#include "XMLNode.h"

namespace Hal {
  const unsigned short int TextStyle::kColor = 0;
  const unsigned short int TextStyle::kAngle = 1;
  const unsigned short int TextStyle::kFont  = 2;
  const unsigned short int TextStyle::kSize  = 3;
  const unsigned short int TextStyle::kAling = 4;

  TextStyle::TextStyle() {}

  void TextStyle::SetAlign(Int_t align) { SetI(kAling, align); }

  void TextStyle::SetColor(Int_t color) { SetI(kColor, color); }

  void TextStyle::SetAngle(Float_t angle) { SetF(kAngle, angle); }

  void TextStyle::SetSize(Float_t size) { SetF(kSize, size); }

  void TextStyle::SetFont(Float_t font) { SetF(kFont, font); }

  Int_t TextStyle::GetAlign() const { return GetI(kAling); }

  Int_t TextStyle::GetColor() const { return GetI(kColor); }

  Float_t TextStyle::GetAngle() const { return GetF(kAngle); }

  Float_t TextStyle::GetSize() const { return GetF(kSize); }

  Float_t TextStyle::GetFont() const { return GetF(kFont); }

  void TextStyle::Apply(TAttText& obj) const {
    if (Find(kAling)) obj.SetTextAlign(GetI(kAling));
    if (Find(kColor)) obj.SetTextColor(GetI(kColor));
    if (Find(kAngle)) obj.SetTextAngle(GetF(kAngle));
    if (Find(kSize)) obj.SetTextSize(GetF(kSize));
    if (Find(kFont)) obj.SetTextFont(GetF(kFont));
  }

  void TextStyle::ExportToXML(XMLNode* node) const {
    if (Find(kAling)) node->AddAttrib(new Hal::XMLAttrib("Align", Form("%i", GetI(kAling))));
    if (Find(kColor)) node->AddAttrib(new Hal::XMLAttrib("Color", Form("%i", GetI(kColor))));
    if (Find(kAngle)) node->AddAttrib(new Hal::XMLAttrib("Angle", Form("%4.4f", GetF(kAngle))));
    if (Find(kSize)) node->AddAttrib(new Hal::XMLAttrib("Size", Form("%4.4f", GetF(kSize))));
    if (Find(kFont)) node->AddAttrib(new Hal::XMLAttrib("Font", Form("%4.4f", GetF(kFont))));
  }

  void TextStyle::ImportFromXML(XMLNode* node) {
    if (auto child = node->GetAttrib("Size")) SetSize(child->GetValue().Atof());
    if (auto child = node->GetAttrib("Font")) SetFont(child->GetValue().Atof());
    if (auto child = node->GetAttrib("Angle")) SetAngle(child->GetValue().Atof());
    if (auto child = node->GetAttrib("Color")) { SetColor(child->GetValue().Atoi()); }
    if (auto child = node->GetAttrib("Align")) { SetAlign(child->GetValue().Atoi()); }
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
