/*
 * FastAxisCalc.cxx
 *
 *  Created on: 11 paź 2024
 *      Author: daniel
 */

#include "FastAxisCalc.h"

#include <iostream>

#include <TAxis.h>

namespace Hal {
  FastAxisCalc::FastAxisCalc(const TAxis& x) {
    const int steps = x.GetNbins();
    fMin            = x.GetBinLowEdge(1);
    fMax            = x.GetBinUpEdge(x.GetNbins());
    fStep           = (fMax - fMin) / ((Double_t) steps);
    fStep           = 1.0 / fStep;
    fMinBin         = 0;
    fMaxBin         = steps + 1;
    fBins           = x.GetNbins();
  }

  void FastAxisCalc::Recalc(const TAxis& x) {
    const int steps = x.GetNbins();
    fMin            = x.GetBinLowEdge(1);
    fMax            = x.GetBinUpEdge(x.GetNbins());
    fStep           = (fMax - fMin) / ((Double_t) steps);
    fStep           = 1.0 / fStep;
    fMinBin         = 0;
    fMaxBin         = steps + 1;
    fBins           = x.GetNbins();
  }

  void FastAxisCalc::RoundToMinusOne() { fMinBin = fMaxBin = -1; }

  void FastAxisCalc::Print(Option_t* option) const {
    std::cout << ClassName() << std::endl;
    std::cout << Form("1/step %4.4f min = %4.4f max = %4.4f", fStep, fMin, fMax) << std::endl;
  }

}  // namespace Hal
