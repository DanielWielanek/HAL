/*
 * QAHistoSettings.cxx
 *
 *  Created on: 25 lis 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "QAHistoSettings.h"

#include "HalStdHist.h"

#include <iostream>

namespace Hal {
  TH1* QAHistoSettings::MakeHisto() const {
    switch (fSize) {
      case 1: return HalStd::MakeHisto1D(GetName(), GetFullTitle(), fBinsX); break;
      case 2: return (TH1*) HalStd::MakeHisto2D(GetName(), GetFullTitle(), fBinsX, fBinsY); break;
      case 3: return (TH1*) HalStd::MakeHisto3D(GetName(), GetFullTitle(), fBinsX, fBinsY, fBinsZ); break;
    }
    return nullptr;
  }

  TString QAHistoSettings::GetFullTitle() const {
    switch (fSize) {
      case 1: return Form("%s;%s", GetTitle(), fAxisNameX.Data()); break;
      case 2: return Form("%s;%s;%s", GetTitle(), fAxisNameX.Data(), fAxisNameY.Data()); break;
      case 3: return Form("%s;%s;%s;%s", GetTitle(), fAxisNameX.Data(), fAxisNameY.Data(), fAxisNameZ.Data()); break;
    }
    return GetName();
  }

  void QAHistoSettings::Print(Option_t* option) const {
    std::cout << "== QAHistoSettings ==" << std::endl;
    std::cout << "Ndim: " << fSize << std::endl;
    std::cout << "Custom: " << fCustom << std::endl;
    std::cout << "Flag: " << fFlag << std::endl;
    if (fSize > 0) {
      std::cout << "\tConf X " << std::endl;
      std::cout << "\t\tFill flag id: " << fFillFlagIdX << std::endl;
      std::cout << "\t\tFill flag name: " << fAxisNameX << std::endl;
      std::cout << "\t\t Axis conf " << Form("%4.0f %4.4f %4.4f", fBinsX.X(), fBinsX.Y(), fBinsX.Z()) << std::endl;
    }
    if (fSize > 1) {
      std::cout << "\tConf Y " << std::endl;
      std::cout << "\t\tFill flag id: " << fFillFlagIdY << std::endl;
      std::cout << "\t\tFill flag name: " << fAxisNameY << std::endl;
      std::cout << "\t\t Axis conf " << Form("%4.0f %4.4f %4.4f", fBinsY.X(), fBinsY.Y(), fBinsY.Z()) << std::endl;
    }
    if (fSize > 2) {
      std::cout << "\tConf X " << std::endl;
      std::cout << "\t\tFill flag id: " << fFillFlagIdZ << std::endl;
      std::cout << "\t\tFill flag name: " << fAxisNameZ << std::endl;
      std::cout << "\t\t Axis conf " << Form("%4.0f %4.4f %4.4f", fBinsZ.X(), fBinsZ.Y(), fBinsZ.Z()) << std::endl;
    }
  }
}  // namespace Hal
