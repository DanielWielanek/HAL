/*
 * HalCorrFitParam.cxx
 *
 *  Created on: 21 gru 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FitParam.h"

#include "Std.h"

#include <iostream>
namespace Hal {
  FitParam::FitParam() :
    fIsFixed(kFALSE),
    fIsDiscrete(kFALSE),
    fMin(0),
    fMax(0),
    fMapMin(0),
    fMapMax(0),
    fNPoint(0),
    fDParam(0),
    fStart(0),
    fFitted(0),
    fError(0),
    fName("") {}

  void FitParam::Init() {
    if (IsDiscrete()) {
      fValues.clear();
      if (fDParam == 0) fDParam = (fMapMax - fMapMin) / (fNPoint - 1.);
      if (fNPoint == 0) fNPoint = (fMapMax - fMapMin) / fDParam + 1;
      fMin   = Hal::Std::Discretize(fNPoint, fMapMin, fMapMax, fMin, '-');
      fMax   = Hal::Std::Discretize(fNPoint, fMapMin, fMapMax, fMax, '+');
      fStart = Hal::Std::Discretize(fNPoint, fMapMin, fMapMax, fStart, '=');
      for (double i = fMapMin; i <= fMapMax; i += fDParam) {
        fValues.push_back(i);
      }
      if (IsFixed()) { fMin = fMax; }
    } else {
      fDParam = 0.01;
      fValues.clear();
    }
  }

  FitParam::~FitParam() {}

  void FitParam::SetRange(Double_t min, Double_t max) {
    if (min > max) {
      Double_t temp = max;
      max           = min;
      min           = temp;
    }
    if (min == max) {
      SetIsFixed(kTRUE);
    } else {
      SetIsFixed(kFALSE);
    }
    fMin   = min;
    fMax   = max;
    fStart = 0.5 * (min + max);
  }

  void FitParam::SetMapRange(Double_t min, Double_t max, Int_t points) {
    if (points <= 1)
      SetIsFixed(kTRUE);
    else
      SetIsFixed(kFALSE);
    if (min > max) {
      Double_t temp = max;
      max           = min;
      min           = temp;
    }
    fMapMin = min;
    fMapMax = max;
    fNPoint = points;
  }

  void FitParam::Print(Option_t* option) const {
    std::cout << "PARAM " << GetParName() << std::endl;
    TString disc = "no", fix = "no";
    if (IsDiscrete()) disc = "yes";
    if (IsFixed()) fix = "yes";
    std::cout << "\tIsDiscrete: " << disc << "\tIsFixed: " << fix << std::endl;
    std::cout << "\tStart val " << Form("%4.4f ", GetStartVal()) << std::endl;
    std::cout << Form("\tLimits %4.4f - %4.4f ", GetMin(), GetMax()) << std::endl;
    std::cout << "\tMap settings" << std::endl;
    std::cout << Form("\tNPoints %i  NDx %4.4f", GetNPoints(), GetDParam()) << std::endl;
    std::cout << Form("\tMap Min: %4.4f   Map Max: %4.4f", GetMapMin(), GetMapMax()) << std::endl;
    std::cout << "\tValues ";
    for (int i = 0; i < GetValues().size(); i++) {
      std::cout << GetValue(i) << " ";
    }
    std::cout << std::endl;
  }
}  // namespace Hal
