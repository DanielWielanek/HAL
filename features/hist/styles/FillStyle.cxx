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

  void FillStyle::SetColor(Int_t val) { SetI(EBitFlag::kColor, val); }

  void FillStyle::SetStyle(Int_t val) { SetI(EBitFlag::kStyle, val); }

  Int_t FillStyle::GetColor() const { return GetI(EBitFlag::kColor); }

  Int_t FillStyle::GetStyle() const { return GetI(EBitFlag::kStyle); }

  void FillStyle::ExportToXML(XMLNode& node) const {

    if (Find(EBitFlag::kColor)) node.AddAttrib("Color", Form("%i", GetI(EBitFlag::kColor)));
    if (Find(EBitFlag::kStyle)) node.AddAttrib("Style", Form("%i", GetI(EBitFlag::kStyle)));
  }

  void FillStyle::ImportFromXML(const XMLNode& node) {

    if (auto& atr = node.GetAttrib("Color"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetColor(x);
    }
    if (auto& atr = node.GetAttrib("Style"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetStyle(x);
    }
  }

} /* namespace Hal */
