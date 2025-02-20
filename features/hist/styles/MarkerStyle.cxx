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
  const unsigned short int MarkerStyle::kColor     = 0;
  const unsigned short int MarkerStyle::kStyle     = 1;
  const unsigned short int MarkerStyle::kLineWidth = 2;
  const unsigned short int MarkerStyle::kSize      = 3;
  void MarkerStyle::SetColor(Int_t val) { SetI(kColor, val); }

  void MarkerStyle::SetStyle(Int_t val) { SetI(kStyle, val); }

  // void MarkerStyle::SetLineWidth(Int_t val) { SetI(kLineWidth, val); }

  void MarkerStyle::SetSize(Int_t val) { SetI(kSize, val); }

  Int_t MarkerStyle::GetColor() const { return GetI(kColor); }

  Int_t MarkerStyle::GetStyle() const { return GetI(kStyle); }

  // Int_t MarkerStyle::GetLineWidth() const { return GetI(kLineWidth); }

  Int_t MarkerStyle::GetSize() const { return GetI(kSize); }

  void MarkerStyle::ExportToXML(XMLNode* node) const {

    if (Find(kColor)) node->AddAttrib(new Hal::XMLAttrib("Color", Form("%i", GetI(kColor))));
    if (Find(kStyle)) node->AddAttrib(new Hal::XMLAttrib("Style", Form("%i", GetI(kStyle))));
    //  if (Find(kLineWidth)) node->AddAttrib(new Hal::XMLAttrib("LineWidth", Form("%i", GetI(kLineWidth))));
    if (Find(kSize)) node->AddAttrib(new Hal::XMLAttrib("Size", Form("%i", GetI(kSize))));
  }

  void MarkerStyle::ImportFromXML(XMLNode* node) {

    if (auto atr = node->GetAttrib("Color")) {
      int x = atr->GetValue().Atoi();
      SetColor(x);
    }
    if (auto atr = node->GetAttrib("Style")) {
      int x = atr->GetValue().Atoi();
      SetStyle(x);
    }
    /*  if (auto atr = node->GetAttrib("LineWidth")) {
        int x = atr->GetValue().Atoi();
        SetLineWidth(x);
      }*/
    if (auto atr = node->GetAttrib("Size")) {
      int x = atr->GetValue().Atoi();
      SetSize(x);
    }
  }
} /* namespace Hal */
