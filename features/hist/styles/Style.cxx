/*
 * Style.cxx
 *
 *  Created on: 27 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Style.h"
#include "Cout.h"
#include "DividedHisto.h"
#include "HistoStyle.h"
#include "LegendStyle.h"
#include "MultiGraph.h"
#include "PadStyle.h"
#include "Std.h"
#include "XMLNode.h"

#include <initializer_list>

#include <TAttLine.h>
#include <TAttMarker.h>
#include <TAxis.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TVirtualPad.h>

namespace Hal {
  void Style::SetFloatInner(Int_t bit, Float_t val) {
    if (bit < 0 || bit > 63) return;
    fValues[bit].sFval = val;
    SETBIT(fFlags, bit);
  }

  void Style::SetIntInner(Int_t bit, Int_t val) {
    if (bit < 0 || bit > 63) return;
    fValues[bit].sIval = val;
    SETBIT(fFlags, bit);
  }

  void Style::LoadFromXML(TString filename, TString nodeName) {
    Hal::XMLFile file(filename);
    auto node = file.GetRootNode().GetChild(nodeName);
    if (node) ImportFromXML(node);
  }

  Hal::LegendStyle* Styles::GetLegendStyle(TString name) const {
    for (auto p : fStyleLegend) {
      if (p->GetName() == name) return p;
    }
    return nullptr;
  }

  Hal::HistoStyle* Styles::GetHistoStyle(TString name) const {
    for (auto p : fStyleHisto) {
      if (p->GetName() == name) return p;
    }
    return nullptr;
  }


  Hal::PadStyle* Styles::GetPadStyle(TString name) const {
    for (auto p : fStylePad) {
      if (p->GetName() == name) return p;
    }
    return nullptr;
  }

  Hal::TextStyle* Styles::GetTextStyle(TString name) const {
    for (auto p : fStyleText) {
      if (p->GetName() == name) return p;
    }
    return nullptr;
  }

  Styles& Styles::Instance() {
    static Styles instance;
    instance.RegisterStyle(Hal::HistoStyle("black"));
    instance.RegisterStyle(Hal::HistoStyle("apollo"));
    instance.RegisterStyle(Hal::LegendStyle("black"));
    instance.RegisterStyle(Hal::LegendStyle("apollo"));
    instance.RegisterStyle(Hal::PadStyle("black"));
    instance.RegisterStyle(Hal::TextStyle("apollo"));
    instance.RegisterStyle(Hal::TextStyle("black"));
    return instance;
  }

  void Styles::RegisterStyle(const Hal::Style& style) {
    auto histo_style  = dynamic_cast<const Hal::HistoStyle*>(&style);
    auto legend_style = dynamic_cast<const Hal::LegendStyle*>(&style);
    auto pad_style    = dynamic_cast<const Hal::PadStyle*>(&style);
    auto text_style   = dynamic_cast<const Hal::TextStyle*>(&style);
    if (histo_style) {
      if (!GetHistoStyle(histo_style->GetName())) fStyleHisto.push_back(new Hal::HistoStyle(*histo_style));
    }
    if (legend_style) {
      if (!GetLegendStyle(legend_style->GetName())) fStyleLegend.push_back(new Hal::LegendStyle(*legend_style));
    }
    if (pad_style) {
      if (!GetPadStyle(pad_style->GetName())) fStylePad.push_back(new Hal::PadStyle(*pad_style));
    }
    if (text_style) {
      if (!GetTextStyle(text_style->GetName())) fStyleText.push_back(new Hal::TextStyle(*text_style));
    }
  }

  Styles::~Styles() {
    for (auto p : fStyleHisto)
      delete p;
    for (auto p : fStyleLegend)
      delete p;
    for (auto p : fStylePad)
      delete p;
    for (auto p : fStyleText)
      delete p;
  }

}  // namespace Hal
