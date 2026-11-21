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
#include "PaveStyle.h"
#include "Std.h"
#include "XMLNode.h"

#include <initializer_list>
#include <iostream>

#include <TAttLine.h>
#include <TAttMarker.h>
#include <TAxis.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TPave.h>
#include <TPaveText.h>
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

  //======================= STYLES =================================
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

  Hal::PaveStyle* Styles::GetPaveStyle(TString name) const {
    for (auto p : fStylePave) {
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
    instance.RegisterStyle(Hal::PaveStyle("apollo"));
    instance.RegisterStyle(Hal::PaveStyle("black"));
    return instance;
  }

  void Styles::RegisterStyle(const Hal::Style& style, TString newName) {
    auto histo_style  = dynamic_cast<const Hal::HistoStyle*>(&style);
    auto legend_style = dynamic_cast<const Hal::LegendStyle*>(&style);
    auto pad_style    = dynamic_cast<const Hal::PadStyle*>(&style);
    auto text_style   = dynamic_cast<const Hal::TextStyle*>(&style);
    auto pave_style   = dynamic_cast<const Hal::PaveStyle*>(&style);
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
    if (pave_style) {
      auto newStyle = new Hal::PaveStyle(*pave_style);
      if (newName.Length() > 0) newStyle->Rename(newName);
      if (!GetPaveStyle(pave_style->GetName())) fStylePave.push_back(newStyle);
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
    std::cout << "\tPave styles" << std::endl;
    for (auto p : fStylePave)
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
    for (auto p : fStylePave)
      delete p;
  }

  void Styles::ApplyLegend(TLegend* obj, TString name) const {
    if (auto st = GetLegendStyle(name); st) st->Apply(*(TLegend*) obj);
  }

  void Styles::ApplyText(TAttText* obj, TString name) const {
    if (auto st = GetTextStyle(name); st) st->ApplyDirect(*(TAttText*) obj);
  }

  void Styles::ApplyHisto(TObject* obj, TString name) const {
    if (auto st = GetHistoStyle(name); st) st->Apply(*(TObject*) obj);
  }

  void Styles::ApplyPad(TVirtualPad* obj, TString name) const {
    if (auto st = GetPadStyle(name); st) st->Apply((TVirtualPad*) obj);
  }

  void Styles::ApplyPave(TPave* obj, TString name) const {
    if (auto st = GetPaveStyle(name); st) {
      if (obj->InheritsFrom("TPaveText")) st->Apply(*obj);
    }
  }


  void Styles::SetGlobal(TString opt) const {
    if (auto paveStyle = GetPaveStyle(opt); paveStyle) {
      paveStyle->SetGlobal();
      paveStyle->SetGlobal("stat");
    }
    if (auto textStyle = GetTextStyle(opt); textStyle) { textStyle->SetGlobal(); }
    if (auto legStyle = GetLegendStyle(opt); legStyle) { legStyle->SetGlobal(); }
    if (auto padStyle = GetPadStyle(opt); padStyle) { padStyle->SetGlobal(); }
    if (auto histoStyle = GetHistoStyle(opt); histoStyle) { histoStyle->SetGlobal(); }
  }

  Bool_t Style::GetSimpleColor(Int_t& colz, TString col) const {
    if (Hal::Std::FindParam(col, "kBlack")) {
      colz = kBlack;
      return kTRUE;
    }
    if (Hal::Std::FindParam(col, "kRed")) {
      colz = kRed;
      return kTRUE;
    }
    if (Hal::Std::FindParam(col, "kGreen")) {
      colz = kGreen;
      return kTRUE;
    }
    if (Hal::Std::FindParam(col, "kYellow")) {
      colz = kYellow;
      return kTRUE;
    }
    if (Hal::Std::FindParam(col, "kWhite")) {
      colz = kWhite;
      return kTRUE;
    }
    if (Hal::Std::FindParam(col, "kOrange")) {
      colz = kOrange;
      return kTRUE;
    }
    if (Hal::Std::FindParam(col, "kPink")) {
      colz = kPink;
      return kTRUE;
    }
    if (Hal::Std::FindParam(col, "kMagenta")) {
      colz = kMagenta;
      return kTRUE;
    }
    if (Hal::Std::FindParam(col, "kViolet")) {
      colz = kViolet;
      return kTRUE;
    }
    if (Hal::Std::FindParam(col, "kBlue")) {
      colz = kBlue;
      return kTRUE;
    }
    if (Hal::Std::FindParam(col, "kAzure")) {
      colz = kAzure;
      return kTRUE;
    }
    if (Hal::Std::FindParam(col, "kCyan")) {
      colz = kCyan;
      return kTRUE;
    }
    if (Hal::Std::FindParam(col, "kTeal")) {
      colz = kTeal;
      return kTRUE;
    }
    if (Hal::Std::FindParam(col, "kGray")) {
      colz = kGray;
      return kTRUE;
    }
    return kFALSE;
  }

  void Styles::LoadPredefined(TString name) {
    RegisterStyle(Hal::HistoStyle(name));
    RegisterStyle(Hal::LegendStyle(name));
    RegisterStyle(Hal::PadStyle(name));
    RegisterStyle(Hal::TextStyle(name));
    RegisterStyle(Hal::PaveStyle(name));
  }

}  // namespace Hal
