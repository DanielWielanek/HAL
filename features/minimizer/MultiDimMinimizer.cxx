/*
 * MultiDimMinimizer.cxx
 *
 *  Created on: 16 wrz 2025
 *      Author: daniel
 */

#include "MultiDimMinimizer.h"

#include <Rtypes.h>
#include <TAxis.h>
#include <TH1.h>
#include <TH2.h>
#include <iostream>

#include "ChiSqMap2D.h"
#include "Cout.h"
#include "MultiDimDataManager.h"
#include "MultiDimFile.h"
#include "MultiDimInterpolator.h"
#include "Std.h"

namespace Hal {

  MultiDimMinimizer::MultiDimMinimizer() {
    // TODO Auto-generated constructor stub
  }

  void MultiDimMinimizer::OpenFile(TString file) {
    if (fInit) {
      Hal::Cout::PrintInfo("Cannot open two files in MultiDimMinuit", EInfo::kError);
      return;
    }
    fFunc = new Hal::MultiDimInterpolator();
    fFunc->OpenFile(file);
    fMultiOffsets = fFunc->GetMDimFIle()->GetMultiOffsets();
    Init();
  }

  void MultiDimMinimizer::SetParLimits(Int_t par, Double_t min, Double_t max) {
    if (par < 0 || par >= (int) fParameters.size()) return;
    fParameters[par].SetMin(min);
    fParameters[par].SetMax(max);
    fParameters[par].SetStartVal(0.5 * (min + max));
    if (min != max)
      fParameters[par].SetIsFixed(kFALSE);
    else
      fParameters[par].SetIsFixed(kTRUE);
    fParameters[par].Init();
  }

  void MultiDimMinimizer::FixParameter(Int_t par, Double_t val) {
    if (par < 0 || par >= (int) fParameters.size()) return;
    fParameters[par].SetMin(val);
    fParameters[par].SetMax(val);
    fParameters[par].SetStartVal(val);
    fParameters[par].SetIsFixed(true);
    fParameters[par].Init();
  }

  TString MultiDimMinimizer::GetParameterName(Int_t par) const {
    if (par < 0 || par >= (int) fParameters.size()) return "";
    return fParameters[par].GetParName();
  }

  void MultiDimMinimizer::SetStartParam(Int_t par, Double_t start) {
    if (par < 0 || par >= (int) fParameters.size()) return;
    fParameters[par].SetStartVal(start);
  }

  void MultiDimMinimizer::PrintParameterSettings(TString message) {
    Cout::PrintInfo(message, EInfo::kLowWarning);
    std::cout << Cout::GetColor(kBlue);
    Cout::Database({"ParName", "MinMap", "MaxMap", "Points", "Step", "Min", "Max"});
    std::cout << Cout::GetDisableColor();
    for (unsigned int i = 0; i < fParameters.size(); i++) {
      if (fParameters[i].IsFixed()) { std::cout << Cout::GetColor(kOrange); }
      Cout::Database({fParameters[i].GetParName().Data(),
                      Form("%4.4f", fParameters[i].GetMapMin()),
                      Form("%4.4f", fParameters[i].GetMapMax()),
                      Form("%d", fParameters[i].GetNPoints()),
                      Form("%4.4f", fParameters[i].GetStepSize()),
                      Form("%4.4f", fParameters[i].GetMin()),
                      Form("%4.4f", fParameters[i].GetMax())});
      if (fParameters[i].IsFixed()) std::cout << Cout::GetDisableColor();
    }
  }

  MultiDimMinimizer::~MultiDimMinimizer() {
    if (fFunc) delete fFunc;
  }

  void MultiDimMinimizer::Init() {
    fParameters = fFunc->GetConfig()->GetParams();
    fDim        = fParameters.size();
    for (auto& x : fParameters) {
      x.SetStartVal(0.5 * (x.GetMapMax() + x.GetMapMin()));
      Double_t mapLow  = x.GetMapMin();
      Double_t mapHigh = x.GetMapMax();
      if (mapLow == mapHigh) {
        x.SetMin(mapLow);
        x.SetMax(mapHigh);
        x.SetIsFixed(true);
      } else {
        x.SetMin(mapLow);
        x.SetMax(mapHigh);
      }
      x.Init();
    }
    PrintParameterSettings("AFTER MINIMIZER INIT ");
    fInit = true;
  }

  std::vector<std::vector<Hal::ChiSqMap2D*>> MultiDimMinimizer::GetChiMap() const {
    std::vector<std::vector<Hal::ChiSqMap2D*>> result;
    std::vector<double> required, minimal;
    std::vector<int> requiredInt, minimalInt;
    required.resize(fDim);
    minimal.resize(fDim);
    minimalInt.resize(fDim);
    requiredInt.resize(fDim);
    for (int i = 0; i < fDim; i++) {
      minimal[i]    = fParameters[i].GetFittedValue();
      minimalInt[i] = (fParameters[i].GetFittedValue() - fParameters[i].GetMapMin()) * fParameters[i].GetOverStepSize();
    }

    for (int i = 1; i < fDim; i++) {
      std::vector<Hal::ChiSqMap2D*> row;
      auto paramA    = fParameters[i];
      Double_t xlow  = paramA.GetMapMin() - paramA.GetStepSize() * 0.5;
      Double_t xhigh = paramA.GetMapMax() + paramA.GetStepSize() * 0.5;
      Int_t xbins    = paramA.GetNPoints();

      for (int j = 0; j < i; j++) {
        required       = minimal;
        requiredInt    = minimalInt;
        auto paramB    = fParameters[j];
        Double_t ylow  = paramB.GetMapMin() - paramB.GetStepSize() * 0.5;
        Double_t yhigh = paramB.GetMapMax() + paramB.GetStepSize() * 0.5;
        Int_t ybins    = paramB.GetNPoints();
        auto chimap    = new Hal::ChiSqMap2D("chi2", xbins, xlow, xhigh, ybins, ylow, yhigh);
        auto xaxis     = chimap->GetHist()->GetXaxis();
        auto yaxis     = chimap->GetHist()->GetYaxis();
        xaxis->CenterTitle();
        yaxis->CenterTitle();

        xaxis->SetTitle(paramA.GetParName());
        yaxis->SetTitle(paramB.GetParName());

        for (int a = 1; a <= xaxis->GetNbins(); a++) {
          required[i]    = xaxis->GetBinCenter(a);
          requiredInt[i] = (required[i] - fParameters[i].GetMapMin()) * fParameters[i].GetOverStepSize();
          for (int b = 1; b <= yaxis->GetNbins(); b++) {
            required[j]    = yaxis->GetBinCenter(b);
            requiredInt[j] = (required[j] - fParameters[j].GetMapMin()) * fParameters[j].GetOverStepSize();
            Int_t entry    = 0;
            for (int c = 0; c < fDim; c++) {
              entry += fMultiOffsets[c] * requiredInt[c];
            }
            double chimin2;  //= fFunc->Extrapolate(required);
            fFunc->GetMDimFIle()->GetEntry(entry);
            chimin2   = (*fFunc->GetMDimFIle()->GetValues())[0];
            auto vals = *fFunc->GetMDimFIle()->GetValues();
            /*     std::cout << "CHIMAP " << i << " " << j << std::endl;
                 for (int c = 0; c < fDim; c++) {
                   std::cout << required[c] << " ";
                 }
                 std::cout << std::endl;
                 for (int c = 0; c < fDim; c++) {
                   std::cout << vals[c + 1] << " ";
                 }
                 std::cout << std::endl;
                 std::cout << std::endl;
                 for (int c = 0; c < fDim; c++) {
                   std::cout << requiredInt[c] << " ";
                 }
                 std::cout << "=" << entry << " " << requiredInt[j] << std::endl;*/
            chimap->GetHist()->SetBinContent(a, b, chimin2);
          }
        }
        chimap->SetXFit(paramA.GetFittedValue(), paramA.GetErrorLow(), paramA.GetErrorHigh());
        chimap->SetYFit(paramB.GetFittedValue(), paramB.GetErrorLow(), paramB.GetErrorHigh());
        row.push_back(chimap);
      }
      for (int j = i; j < fDim; j++)
        row.push_back(nullptr);
      result.push_back(row);
    }
    for (auto x : fMultiOffsets) {
      std::cout << x << " ";
    }
    std::cout << std::endl;
    return result;
  }

} /* namespace Hal */
