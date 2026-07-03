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
#include <iostream>

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

  void Styles::RegisterStyle(const Hal::Style& style, TString newName) {
    auto histo_style  = dynamic_cast<const Hal::HistoStyle*>(&style);
    auto legend_style = dynamic_cast<const Hal::LegendStyle*>(&style);
    auto pad_style    = dynamic_cast<const Hal::PadStyle*>(&style);
    auto text_style   = dynamic_cast<const Hal::TextStyle*>(&style);
    if (histo_style) {
      auto newStyle = new Hal::HistoStyle(*histo_style);
      if (newName.Length() > 0) newStyle->Rename(newName);
      if (!GetHistoStyle(histo_style->GetName())) fStyleHisto.push_back(newStyle);
    }
    if (legend_style) {
      auto newStyle = new Hal::LegendStyle(*legend_style);
      if (newName.Length() > 0) newStyle->Rename(newName);
      if (!GetLegendStyle(legend_style->GetName())) fStyleLegend.push_back(newStyle);
    }
    if (pad_style) {
      auto newStyle = new Hal::PadStyle(*pad_style);
      if (newName.Length() > 0) newStyle->Rename(newName);
      if (!GetPadStyle(pad_style->GetName())) fStylePad.push_back(newStyle);
    }
    if (text_style) {
      auto newStyle = new Hal::TextStyle(*text_style);
      if (newName.Length() > 0) newStyle->Rename(newName);
      if (!GetTextStyle(text_style->GetName())) fStyleText.push_back(newStyle);
    }
  }

  void Styles::Print() const {
    std::cout << "Styles info" << std::endl;
    std::cout << "Histo styles" << std::endl;
    for (auto p : fStyleHisto)
      std::cout << "\t\t" << p->GetName() << std::endl;
    std::cout << "\tLegend styles" << std::endl;
    for (auto p : fStyleLegend)
      std::cout << "\t\t" << p->GetName() << std::endl;
    std::cout << "\tPad styles" << std::endl;
    for (auto p : fStylePad)
      std::cout << "\t\t" << p->GetName() << std::endl;
    std::cout << "\tText styles" << std::endl;
    for (auto p : fStyleText)
      std::cout << "\t\t" << p->GetName() << std::endl;
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

  void Styles::ApplyLegend(TLegend* obj, TString name) const {
    if (auto st = GetLegendStyle(name); st) st->Apply(*(TLegend*) obj);
  }

  void Styles::ApplyText(TAttText* obj, TString name) const {
    if (auto st = GetTextStyle(name); st) st->Apply(*(TAttText*) obj);
  }

  void Styles::ApplyHisto(TObject* obj, TString name) const {
    if (auto st = GetHistoStyle(name); st) st->Apply(*(TObject*) obj);
  }

  void Styles::ApplyPad(TVirtualPad* obj, TString name) const {
    if (auto st = GetPadStyle(name); st) st->Apply((TVirtualPad*) obj);
  }

}  // namespace Hal
