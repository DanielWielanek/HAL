/*
 * LineStyle.cxx
 *
 *  Created on: 30 lip 2024
 *      Author: daniel
 */

#include "LineStyle.h"

#include <RtypesCore.h>
#include <TString.h>

#include "XMLNode.h"

namespace Hal {
  const unsigned short int LineStyle::kColor = 0;
  const unsigned short int LineStyle::kWidth = 1;
  const unsigned short int LineStyle::kStyle = 2;

  void LineStyle::SetColor(Int_t val) { SetI(kColor, val); }

  void LineStyle::SetWidth(Int_t val) { SetI(kWidth, val); }

  void LineStyle::SetStyle(Int_t val) { SetI(kStyle, val); }

  Int_t LineStyle::GetColor() const { return GetI(kColor); }

  Int_t LineStyle::GetWidth() const { return GetI(kWidth); }

  Int_t LineStyle::GetStyle() const { return GetI(kStyle); }

  void LineStyle::ExportToXML(XMLNode* node) const {

    if (Find(kColor)) node->AddAttrib(new Hal::XMLAttrib("Color", Form("%i", GetI(kColor))));
    if (Find(kWidth)) node->AddAttrib(new Hal::XMLAttrib("Width", Form("%i", GetI(kWidth))));
    if (Find(kStyle)) node->AddAttrib(new Hal::XMLAttrib("Style", Form("%i", GetI(kStyle))));
  }

  void LineStyle::ImportFromXML(XMLNode* node) {

    if (auto atr = node->GetAttrib("Color")) {
      int x = atr->GetValue().Atoi();
      SetColor(x);
    }
    if (auto atr = node->GetAttrib("Width")) {
      int x = atr->GetValue().Atoi();
      SetWidth(x);
    }
    if (auto atr = node->GetAttrib("Style")) {
      int x = atr->GetValue().Atoi();
      SetStyle(x);
    }
  }
} /* namespace Hal */
