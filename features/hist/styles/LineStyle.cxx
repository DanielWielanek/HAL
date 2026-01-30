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

  void LineStyle::SetColor(Int_t val) { SetI(EBitFlag::kColor, val); }

  void LineStyle::SetWidth(Int_t val) { SetI(EBitFlag::kWidth, val); }

  void LineStyle::SetStyle(Int_t val) { SetI(EBitFlag::kStyle, val); }

  Int_t LineStyle::GetColor() const { return GetI(EBitFlag::kColor); }

  Int_t LineStyle::GetWidth() const { return GetI(EBitFlag::kWidth); }

  Int_t LineStyle::GetStyle() const { return GetI(EBitFlag::kStyle); }

  void LineStyle::ExportToXML(XMLNode& node) const {

    if (Find(EBitFlag::kColor)) node.AddAttrib("Color", Form("%i", GetI(EBitFlag::kColor)));
    if (Find(EBitFlag::kWidth)) node.AddAttrib("Width", Form("%i", GetI(EBitFlag::kWidth)));
    if (Find(EBitFlag::kStyle)) node.AddAttrib("Style", Form("%i", GetI(EBitFlag::kStyle)));
  }

  void LineStyle::ImportFromXML(const XMLNode& node) {

    if (auto& atr = node.GetAttrib("Color"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetColor(x);
    }
    if (auto& atr = node.GetAttrib("Width"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetWidth(x);
    }
    if (auto& atr = node.GetAttrib("Style"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetStyle(x);
    }
  }
} /* namespace Hal */
