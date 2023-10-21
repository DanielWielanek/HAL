/*
 * StdError.cxx
 *
 *  Created on: 21 paź 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "ErrorCalc.h"

#include "Cout.h"

#include <iostream>
namespace Hal {

  Double_t ErrorCalc::GetStatError() const { return fStatisticalUncert; }

  Double_t ErrorCalc::BarlowTest(Int_t prec) const {
    Double_t totalErr = 0;
    Hal::Cout::Database({"Name", "Value"});
    TString flag = Form("%%4.%if", prec);
    auto quadDif = [&](Double_t b) { return TMath::Abs(fStatisticalUncert * fStatisticalUncert - b * b); };
    auto measDif = [&](Double_t b) { return TMath::Abs(fMeasurement - b); };
    auto getForm = [&](Double_t val) { return TString(Form(flag, val)); };
    for (auto i : fValues) {
      auto uncerts       = i.second;  // array of all uncerts
      Double_t uncertSum = 0;
      Double_t m         = 0;
      for (auto j : uncerts) {  // all measurements
        Double_t xn        = j.first;
        Double_t en        = j.second;
        Double_t deltaSysj = TMath::Power(measDif(xn), 2) - quadDif(en);
        if (deltaSysj < 0) {
          deltaSysj = 0;  // does not contribute to uncert
        } else {
          uncertSum += deltaSysj * deltaSysj;
          m++;
        }
      }
      if (m == 0) m = 1;
      Double_t sigmaSysi = TMath::Sqrt(uncertSum / m);
      if (sigmaSysi > 0) {
        std::cout << Hal::Cout::GetColor(kGreen);
      } else {
        std::cout << Hal::Cout::GetColor(kRed);
      }

      totalErr += sigmaSysi * sigmaSysi;
      Hal::Cout::Database({i.first, getForm(sigmaSysi)});
    }

    totalErr = TMath::Sqrt(totalErr);
    std::cout << Hal::Cout::GetColor(kBlue);
    Hal::Cout::Database({"Total", getForm(totalErr)});
    std::cout << Hal::Cout::GetDisableColor();
    return totalErr;
  }

  ErrorCalc::ErrorCalc(TString name, Double_t val, Double_t err) {
    SetName(name);
    fMeasurement       = val;
    fStatisticalUncert = err;
  }

  std::vector<std::pair<Double_t, Double_t>> ErrorCalc::GetAllUncerts(TString name) const {
    std::vector<std::pair<Double_t, Double_t>> vals;
    for (auto i : fValues) {
      if (i.first == name) { return i.second; }
    }
    return vals;
  }

  void ErrorCalc::AddSysError(TString name, Double_t value, Double_t statUncert) {
    if (statUncert == -1) {
      statUncert = TMath::Abs(fMeasurement - value);
    } else if (statUncert < 0) {
      statUncert = fStatisticalUncert;
    }
    std::pair<Double_t, Double_t> p(value, statUncert);
    for (auto& i : fValues) {
      if (i.first == name) {
        i.second.push_back(p);
        return;
      }
    }
    std::vector<std::pair<Double_t, Double_t>> vec;
    vec.push_back(p);
    std::pair<TString, std::vector<std::pair<Double_t, Double_t>>> pair(name, vec);
    fValues.push_back(pair);
  }

  Double_t ErrorCalc::TotalSys(Int_t prec) const {
    Double_t total = 0;
    TString flag   = Form("%%4.%if", prec);
    auto getForm   = [&](Double_t val) { return TString(Form(flag, val)); };
    Hal::Cout::Database({"Name", "Value"});
    for (auto i : fValues) {
      auto errors     = GetAllUncerts(i.first);
      Double_t maxErr = 0;
      for (auto j : errors) {
        maxErr = TMath::Max(maxErr, TMath::Abs(fMeasurement - j.first));
      }
      total += maxErr * maxErr;
      Hal::Cout::Database({i.first, getForm(maxErr)});
    }
    std::cout << Hal::Cout::GetColor(kBlue);
    Hal::Cout::Database({"Total", getForm(TMath::Sqrt(total))});
    std::cout << Hal::Cout::GetDisableColor();
    return TMath::Sqrt(total);
  }

} /* namespace Hal */
