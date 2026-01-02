/*
 * MinimzerResult.cxx
 *
 *  Created on: 16 wrz 2025
 *      Author: daniel
 */

#include "MinimizerResult.h"

namespace Hal {

  FittedParam MinimizerResult::GetParam(Int_t index) const { return fParams[index]; }

  FittedParam MinimizerResult::GetParam(TString name) const {
    for (auto par : fParams) {
      if (par.GetFitParam().GetParName() == name) return par;
    }
    return fParams[0];
  }
} /* namespace Hal */
