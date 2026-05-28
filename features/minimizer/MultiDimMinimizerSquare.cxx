/*
 * MultiDimMInimizerSquare.cxx
 *
 *  Created on: 16 wrz 2025
 *      Author: daniel
 */

#include "MultiDimMinimizerSquare.h"

#include <TMath.h>
#include <TMathBase.h>

#include "FitParam.h"
#include "MinimizerResult.h"
#include "MultiDimFile.h"
#include "MultiDimInterpolator.h"

#include <iostream>

namespace Hal {

  MinimizerResult MultiDimMinimizerSquare::Minimize() {
    PrintParameterSettings("Minimize");
    MinimizerResult res;
    fPoints.clear();
    for (auto& p : fParameters) {
      fPoints.push_back(p.GetIndexesArray());
    }
    std::vector<int> offset;
    Int_t freePars = 0;
    for (auto& p : fParameters) {
      offset.push_back(p.GetNPoints());
      if (!p.IsFixed()) ++freePars;
    }
    for (int i = 0; i < (int) fMultiOffsets.size(); i++) {
      for (int j = 0; j < (int) fPoints[i].size(); j++)
        fPoints[i][j] = fPoints[i][j] * fMultiOffsets[i];
    }
    fStateVector.resize(fDim);
    fStateVectorMin.resize(fDim);
    fStateVectorInt.resize(fDim);
    fErrLow.resize(fDim);
    fErrHigh.resize(fDim);
    for (int i = 0; i < (int) fParameters.size(); i++) {
      fErrHigh[i] = 0;
      fErrLow[i]  = fParameters[i].GetNPoints();
    }
    std::cout << "LOOP OVER PARAMETER" << std::endl;
    LoopOverParameter(0);
    Double_t step = TMath::ChisquareQuantile(0.65, freePars);
    fThreshold    = step + fChiMin;
    fThreshold    = 0.5;
    std::cout << "LOOP OVER ERROR" << std::endl;
    LoopOverParameterError(0);

    for (int i = 0; i < fDim; i++) {
      auto& pars    = fParameters[i];
      Double_t val  = pars.GetValueForBin(fStateVectorMin[i]);
      Double_t erLo = pars.GetValueForBin(fErrLow[i]);
      Double_t erHi = pars.GetValueForBin(fErrHigh[i]);
      pars.SetFittedValue(val);
      pars.SetAsymetricError(val - erLo, erHi - val);
    }
    return res;
  }

  void MultiDimMinimizerSquare::LoopOverParameter(Int_t n) {
    if (n == (int) fPoints.size()) {
      FindParam();
    } else {
      int count = 0;
      for (auto par : fPoints[n]) {
        fStateVector[n]    = par;
        fStateVectorInt[n] = count;
        ++count;
        LoopOverParameter(n + 1);
      }
    }
  }

  void MultiDimMinimizerSquare::LoopOverParameterError(Int_t n) {
    if (n == (int) fPoints.size()) {
      FindError();
    } else {
      int count = 0;
      for (auto par : fPoints[n]) {
        fStateVector[n]    = par;
        fStateVectorInt[n] = count;
        ++count;
        LoopOverParameterError(n + 1);
      }
    }
  }

  void MultiDimMinimizerSquare::FindParam() {
    Int_t entry = 0;
    for (int i = 0; i < fDim; i++) {
      entry += fStateVector[i];
    }
    fFileRaw->GetEntry(entry);

    Double_t val = (*fFileRaw->GetValues())[0];
    if (val < fChiMin) {
      fChiMin         = val;
      fStateVectorMin = fStateVectorInt;
    }
  }

  void MultiDimMinimizerSquare::FindError() {
    Int_t entry = 0;
    for (int i = 0; i < fDim; i++) {
      entry += fStateVector[i];
    }
    fFileRaw->GetEntry(entry);
    Double_t val = (*fFileRaw->GetValues())[0];
    if (val < fThreshold) {
      for (int i = 0; i < fDim; i++) {
        fErrLow[i]  = TMath::Min(fErrLow[i], fStateVectorInt[i]);
        fErrHigh[i] = TMath::Max(fErrHigh[i], fStateVectorInt[i]);
      }
    }
  }

  void MultiDimMinimizerSquare::Init() {
    MultiDimMinimizer::Init();
    fFileRaw = fFunc->GetMDimFIle();
  }

} /* namespace Hal */
