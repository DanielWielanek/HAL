/*
 * FillStyle.cxx
 *
 *  Created on: 30 lip 2024
 *      Author: daniel
 */

#include "FillStyle.h"

#include <TString.h>
#include <TStyle.h>

#include "StdString.h"
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

  void FillStyle::SetGlobal(TString opt) const {
    if (Hal::Std::FindParam(opt, "hist")) {
      if (Find(EBitFlag::kColor)) gStyle->SetHistFillColor(GetI(EBitFlag::kColor));
      if (Find(EBitFlag::kStyle)) gStyle->SetHistFillStyle(GetI(EBitFlag::kStyle));
    } else if (Hal::Std::FindParam(opt, "pad")) {
      if (Find(EBitFlag::kColor)) gStyle->SetFrameFillColor(GetI(EBitFlag::kColor));
      if (Find(EBitFlag::kStyle)) gStyle->SetFrameFillStyle(GetI(EBitFlag::kStyle));
    } else {
      if (Find(EBitFlag::kColor)) gStyle->SetFillColor(GetI(EBitFlag::kColor));
      if (Find(EBitFlag::kStyle)) gStyle->SetFillStyle(GetI(EBitFlag::kStyle));
    }
  }

} /* namespace Hal */
