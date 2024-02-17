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

} /* namespace Hal */
