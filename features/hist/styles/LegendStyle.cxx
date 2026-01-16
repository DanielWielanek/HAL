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

  void LegendStyle::Apply(TLegend& leg) const {
    fFill.Apply(leg);
    fLine.Apply(leg);
    fText.Apply(leg);
  }

  void LegendStyle::ExportToXML(XMLNode& node) const {
    XMLNode fillStyle("FillStyle");
    fFill.ExportToXML(fillStyle);
    XMLNode lineStyle("LineStyle");
    fLine.ExportToXML(fillStyle);
    XMLNode textStyle("TextStyle");
    node.AddChild(fillStyle);
    node.AddChild(lineStyle);
    node.AddChild(textStyle);
  }

  void LegendStyle::Import(const TLegend& x) {
    TLegend dummy;
    fText.Import(x);
    fLine.Import(x);
    fFill.Import(x);
  }

  void LegendStyle::ImportFromXML(const XMLNode& node) {
    if (auto child = node.GetChild("FillStyle"); !child.IsNull()) { fFill.ImportFromXML(child); }
    if (auto child = node.GetChild("LineStyle"); !child.IsNull()) { fLine.ImportFromXML(child); }
    if (auto child = node.GetChild("TextStyle"); !child.IsNull()) { fText.ImportFromXML(child); }
  }

  LegendStyle::LegendStyle(TString style) {
    if (style.Length() == 0) return;
    if (Hal::Std::FindParam(style, "apollo", kTRUE)) { fText.SetFont(82); }
    if (Hal::Std::FindParam(style, "05", kTRUE)) { fText.SetSize(0.05); }
    if (Hal::Std::FindParam(style, "06", kTRUE)) { fText.SetSize(0.06); }
    if (Hal::Std::FindParam(style, "black", kTRUE)) {
      GetFillStyle().SetColor(kBlack);
      GetTextStyle().SetColor(kWhite);
    }
  }

} /* namespace Hal */
