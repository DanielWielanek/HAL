/*
 * FillStyle.cxx
 *
 *  Created on: 30 lip 2024
 *      Author: daniel
 */

#include "FillStyle.h"

#include <TString.h>

#include "XMLNode.h"

namespace Hal {
  const unsigned short int FillStyle::kColor = 0;
  const unsigned short int FillStyle::kStyle = 1;

  void FillStyle::SetColor(Int_t val) { SetI(kColor, val); }

  void FillStyle::SetStyle(Int_t val) { SetI(kStyle, val); }

  Int_t FillStyle::GetColor() const { return GetI(kColor); }

  Int_t FillStyle::GetStyle() const { return GetI(kStyle); }

  void FillStyle::ExportToXML(XMLNode* node) const {

    if (Find(kColor)) node->AddAttrib(new Hal::XMLAttrib("Color", Form("%i", GetI(kColor))));
    if (Find(kStyle)) node->AddAttrib(new Hal::XMLAttrib("Style", Form("%i", GetI(kStyle))));
  }

  void FillStyle::ImportFromXML(XMLNode* node) {

    if (auto atr = node->GetAttrib("Color")) {
      int x = atr->GetValue().Atoi();
      SetColor(x);
    }
    if (auto atr = node->GetAttrib("Style")) {
      int x = atr->GetValue().Atoi();
      SetStyle(x);
    }
  }

} /* namespace Hal */
