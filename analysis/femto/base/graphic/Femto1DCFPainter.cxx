/*
 * Femto1DCFPainter.cxx
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "Femto1DCFPainter.h"
#include "FemtoDrawOptions.h"

#include "Femto1DCF.h"
#include "Style.h"

#include <TH1.h>
#include <TString.h>
#include <TVirtualPad.h>

namespace Hal {

  Femto1DCFPainter::Femto1DCFPainter(Hal::Femto1DCF& cf, FemtoDrawOptions opt) : FemtoPainter(opt) {
    fNum   = (TH1*) cf.GetNum()->Clone();
    fDen   = (TH1*) cf.GetDen()->Clone();
    fRatio = (TH1*) cf.GetHist(false)->Clone();
  }

  void Femto1DCFPainter::Paint() {
    TString opt;
    if (fDrawOpts.DrawSame()) opt = "SAME";
    TH1* draw = nullptr;
    if (fDrawOpts.DrawNum()) {
      draw = fNum;
    } else if (fDrawOpts.DrawDen()) {
      draw = fDen;
    } else {
      draw = fRatio;
    }
    auto vpad = gPad;
    fMainPad->cd();
    if (fHistoStyle) { fHistoStyle->Apply(*draw); }
    if (fPadStyle) { fPadStyle->Apply(); }
    gPad = vpad;
  }

  void Femto1DCFPainter::Rescale(Double_t newScale) {
    FemtoPainter::Rescale(newScale);
    fRatio->Scale(fNewScale);
  }

  Femto1DCFPainter::~Femto1DCFPainter() {
    if (fNum) delete fNum;
    if (fDen) delete fDen;
    if (fRatio) delete fRatio;
  }

} /* namespace Hal */
