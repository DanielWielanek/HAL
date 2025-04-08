/*
 * CorrFitMapGroupConfig2.cxx
 *
 *  Created on: 27 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitMapGroupConfig.h"

#include "Cout.h"
#include "FemtoPair.h"

namespace Hal {
  CorrFitMapGroupConfig::CorrFitMapGroupConfig() : fBins(100), fMin(0), fMax(100) {
    fStep = (fMax - fMin) / Double_t(fBins);
    fStep = 1.0 / fStep;
    SetName("CorrFitMapGroup");
  }

  void CorrFitMapGroupConfig::SetAxis(Int_t bins, Double_t min, Double_t max) {
    fBins = bins;
    if (fMin < 0) {
      fMin = 0;
      if (fBins % 2 == 1) { Cout::PrintInfo("CorrFitMapGroupConfig::SetAxis odd number of CF bins detected!", EInfo::kError); }
      fBins = fBins / 2;
    }
    fMin  = min;
    fMax  = max;
    fStep = (fMax - fMin) / Double_t(fBins);
    fStep = 1.0 / fStep;
  }


  Int_t CorrFitMapGroupConfig::GetBin(const FemtoPair* pair) const {
    Double_t val = TMath::Abs(pair->GetT());
    if (GroupByLong()) {  // long
      val = TMath::Abs(pair->GetZ());
    }
    Int_t bin = (val - fMin) * fStep;
    if (bin >= fBins) return -1;
    return bin;
  }

  void CorrFitMapGroupConfig::Add(const Object* pack) {
    auto* data = dynamic_cast<const CorrFitMapGroupConfig*>(pack);
    if (!data) {
      Hal::Cout::PrintInfo("Cannot add CorrFitMapGroupConfig", EInfo::kError);
      return;
    }
    auto printError = []() { Hal::Cout::PrintInfo("Cannot add CorrFitMapGroupConfig", EInfo::kError); };

    if (fMode != data->fMode) { printError(); }
    if (fBins != data->fBins) { printError(); };
    if (fMin != data->fMin) { printError(); }
  }

  std::pair<Int_t, Int_t> CorrFitMapGroupConfig::GetBranchesByValue(Double_t min, Double_t max) const {
    if (min == max && fMin != fMax) { return GetBranchesByValue(fMin, fMax); }
    Int_t lowBin  = (fMin - min) * fStep;
    Int_t highBin = (fMax - min) * fStep;
    std::pair<Int_t, Int_t> result;
    result.first  = lowBin;
    result.second = highBin - 1;
    return result;
  }

} /* namespace Hal */
