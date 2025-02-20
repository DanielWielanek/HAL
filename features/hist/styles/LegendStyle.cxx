/*
 * LegendStyle.cxx
 *
 *  Created on: 20 gru 2024
 *      Author: daniel
 */

#include "LegendStyle.h"

#include <TAttText.h>
#include <TLegend.h>
#include <TString.h>

#include "Std.h"
#include "Style.h"
#include "XMLNode.h"

namespace Hal {
  const unsigned short int LegendStyle::kFontSize  = 0;
  const unsigned short int LegendStyle::kFontStyle = 1;
  LegendStyle LegendStyle::GetStyle(TString opt) {
    LegendStyle style;
    if (Hal::Std::FindParam(opt, "apollo", kTRUE)) { style.SetFont(82); }
    if (Hal::Std::FindParam(opt, "05", kTRUE)) { style.SetFontSize(0.05); }
    if (Hal::Std::FindParam(opt, "06", kTRUE)) { style.SetFontSize(0.06); }
    return style;
  }

  void LegendStyle::SetFontSize(Float_t size) { SetF(kFontSize, size); }

  void LegendStyle::SetFont(Int_t style) { SetI(kFontStyle, style); }

  Float_t LegendStyle::GetFontSize() const { return GetF(kFontSize); }

  Int_t LegendStyle::GetFont() const { return GetI(kFontStyle); }

  void LegendStyle::Apply(TLegend& leg) const {
    if (Find(kFontSize)) leg.SetTextSize(GetFontSize());
    if (Find(kFontStyle)) leg.SetTextFont(GetFont());
    fFill.Apply(leg);
    fLine.Apply(leg);
  }

  void LegendStyle::ExportToXML(XMLNode* node) const {
    if (Find(kFontSize)) node->AddAttrib(new Hal::XMLAttrib("FontSize", Form("%4.4f", GetF(kFontSize))));
    if (Find(kFontStyle)) node->AddAttrib(new Hal::XMLAttrib("FontStyle", Form("%i", GetI(kFontStyle))));
    XMLNode* fillStyle = new XMLNode("FillStyle");
    fFill.ExportToXML(fillStyle);
    XMLNode* lineStyle = new XMLNode("LineStyle");
    fLine.ExportToXML(fillStyle);
    node->AddChild(fillStyle);
    node->AddChild(lineStyle);
  }

  void LegendStyle::Import(const TLegend& x) {
    TLegend dummy;
    if (dummy.GetTextFont() != x.GetTextFont()) SetFont(x.GetTextFont());
    if (dummy.GetTextSize() != x.GetTextSize()) SetFontSize(x.GetTextSize());
    fLine.Import(x);
    fFill.Import(x);
  }

  void LegendStyle::ImportFromXML(XMLNode* node) {
    if (auto child = node->GetAttrib("FontSize")) SetFontSize(child->GetValue().Atof());
    if (auto child = node->GetAttrib("FontStyle")) SetFont(child->GetValue().Atoi());
    if (auto child = node->GetChild("FillStyle")) { fFill.ImportFromXML(child); }
    if (auto child = node->GetChild("LineStyle")) { fLine.ImportFromXML(child); }
  }
} /* namespace Hal */
