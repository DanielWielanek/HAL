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
  FitParam::FitParam() {}

  void FitParam::Init() {
    if (!IsDiscrete()) return;
    if (fNPoint == 0) {
      std::cout << " cannot have npoint = 0" << std::endl;
      exit(0);
    }
    fDParam = (fMapMax - fMapMin) / Double_t(fNPoint - 1);
    if (fNPoint == 1) fDParam = 0;
    if (!IsMapSet()) {
      if (fMin == fMax) {
        SetMapRange(fMin, fMax, 1);
      } else {
        SetMapRange(fMin, fMax, 100);
      }
    }
    fMin   = Hal::Std::Discretize(fNPoint - 1, fMapMin, fMapMax, fMin, '-');  // npoint -1 because we have n-1 areas than points
    fMax   = Hal::Std::Discretize(fNPoint - 1, fMapMin, fMapMax, fMax, '+');
    fStart = Hal::Std::Discretize(fNPoint - 1, fMapMin, fMapMax, fStart, '=');
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
    if (points <= 1) SetIsFixed(kTRUE);  // to not go into some infinite loops
    if (min > max) {
      Double_t temp = max;
      max           = min;
      min           = temp;
    }
    fMapMin   = min;
    fMapMax   = max;
    fNPoint   = points;
    fIsMapSet = kTRUE;
  }

  void FitParam::Print(Option_t* /*option*/) const {
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
    auto array = GetValuesArray();
    for (unsigned int i = 0; i < array.size(); i++) {
      std::cout << array[i] << " ";
    }
    std::cout << std::endl;
  }

  const std::vector<Double_t> FitParam::GetValuesArray() const {
    std::vector<Double_t> values;
    if (!IsDiscrete()) {
      values.push_back(fMin);
      return values;
    }
    if (IsFixed() || fDParam == 0) {
      values.push_back(fMin);
    } else {
      for (double x = fMin; x <= fMax; x += fDParam) {
        values.push_back(x);
      }
    }
    return values;
  }

}  // namespace Hal
