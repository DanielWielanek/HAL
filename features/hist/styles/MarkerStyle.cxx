/*
 * MarkerStyle.cxx
 *
 *  Created on: 30 lip 2024
 *      Author: daniel
 */

#include "MarkerStyle.h"

#include <RtypesCore.h>
#include <TString.h>

#include "XMLNode.h"

namespace Hal {
  void MarkerStyle::SetColor(Int_t val) { SetI(EBitFlag::kColor, val); }

  void MarkerStyle::SetStyle(Int_t val) { SetI(EBitFlag::kStyle, val); }

  // void MarkerStyle::SetLineWidth(Int_t val) { SetI(EBitFlag::kLineWidth, val); }

  void MarkerStyle::SetSize(Double_t val) { SetF(EBitFlag::kSize, val); }

  Int_t MarkerStyle::GetColor() const { return GetI(EBitFlag::kColor); }

  Int_t MarkerStyle::GetStyle() const { return GetI(EBitFlag::kStyle); }

  // Int_t MarkerStyle::GetLineWidth() const { return GetI(EBitFlag::kLineWidth); }

  Double_t MarkerStyle::GetSize() const { return GetF(EBitFlag::kSize); }

  void MarkerStyle::ExportToXML(XMLNode& node) const {

    if (Find(EBitFlag::kColor)) node.AddAttrib("Color", Form("%i", GetI(EBitFlag::kColor)));
    if (Find(EBitFlag::kStyle)) node.AddAttrib("Style", Form("%i", GetI(EBitFlag::kStyle)));
    //  if (Find(EBitFlag::kLineWidth)) node.AddAttrib(new Hal::XMLAttrib("LineWidth", Form("%i", GetI(EBitFlag::kLineWidth))));
    if (Find(EBitFlag::kSize)) node.AddAttrib("Size", Form("%4.2f", GetF(EBitFlag::kSize)));
  }

  void MarkerStyle::ImportFromXML(const XMLNode& node) {

    if (auto& atr = node.GetAttrib("Color"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetColor(x);
    }
    if (auto& atr = node.GetAttrib("Style"); !atr.IsNull()) {
      int x = atr.GetValue().Atoi();
      SetStyle(x);
    }
    /*  if (auto atr = node.GetAttrib("LineWidth")) {
        int x = atr->GetValue().Atoi();
        SetLineWidth(x);
      }*/
    if (auto& atr = node.GetAttrib("Size"); !atr.IsNull()) {
      int x = atr.GetValue().Atof();
      SetSize(x);
    }
  }
} /* namespace Hal */
