/*
 * Painter.cxx
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "Painter.h"

#include "Style.h"

#include <TCanvas.h>
namespace Hal {
  Painter::Painter() {
    if (gPad == nullptr) {
      fCanvas = new TCanvas();
      fOwnPad = kTRUE;
    }
    fMainPad = gPad;
    if (fMainPad->InheritsFrom("TCanvas")) { fCanvas = fMainPad; }
  }
  Painter::~Painter() {
    if (fOwnPad) { delete fCanvas; }
  }

  Painter::Painter(const Painter& other) {
    if (other.fCanvas) {
      fCanvas  = new TCanvas();
      fOwnPad  = kTRUE;
      fMainPad = gPad;
    }
    if (other.fPadStyle) this->fPadStyle = new Hal::PadStyle(*other.fPadStyle);
    if (other.fHistoStyle) this->fHistoStyle = new Hal::HistoStyle(*other.fHistoStyle);
  }

  Painter& Painter::operator=(const Painter& other) {
    if (&other == this) return *this;
    if (other.fCanvas) {
      fCanvas  = new TCanvas();
      fOwnPad  = kTRUE;
      fMainPad = gPad;
    }
    if (other.fPadStyle) {
      if (fPadStyle) delete fPadStyle;
      this->fPadStyle = new Hal::PadStyle(*other.fPadStyle);
    }
    if (other.fHistoStyle) {
      if (fHistoStyle) delete fHistoStyle;
      this->fHistoStyle = new Hal::HistoStyle(*other.fHistoStyle);
    }
    return *this;
  }

  void Painter::SetStyle(const Hal::Style& style) {
    const Hal::HistoStyle* styleHisto = dynamic_cast<Hal::HistoStyle*>(&style);
    const Hal::PadStyle* stylePad     = dynamic_cast<Hal::PadStyle*>(&style);
    SetHistoStyle(*styleHisto);
    SetPadStyle(*stylePad);
  }

  void Painter::SetHistoStyle(const Hal::HistoStyle& style) {}

  void Painter::SetPadStyle(const Hal::PadStyle& style) {}

  void Painter::SetStyle(const Hal::Style& style) {}

  std::vector<TH1*> Painter::GetListHistograms(Int_t subpad) const {
    std::vector<TH1*> list;
    auto vPad = fSubPads[subpad];
    if (vPad == nullptr) return list;
    TList* children = vPad->GetListOfPrimitives();
    for (int i = 0; i < children->GetEntries(); i++) {
      auto obj = dynamic_cast<TH1*>(children->At(i));
      if (obj) { list.push_back(obj); }
    }
    return list;
  }

  void Painter::Repaint(TString option) {
    for (int i = 0; i < fSubPads.size(); i++) {
      auto lista = GetListHistograms(i);
      if (lista.size() == 0) continue;
      fSubPads[i]->cd();
      for (auto i : lista) {
        i->Draw("SAME");
      }
    }
  }

} /* namespace Hal */
