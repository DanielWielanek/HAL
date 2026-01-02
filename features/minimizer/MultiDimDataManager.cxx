/*
 * MultiDimDataManager.cxx
 *
 *  Created on: 16 sty 2025
 *      Author: daniel
 */

#include "MultiDimDataManager.h"

#include "Cout.h"

#include <iostream>

namespace Hal {

  void MultiDimDataManager::AddParameter(TString name, Double_t low, Double_t high, Double_t step) {
    for (auto i : fParams) {
      if (i.GetParName() == name) {
        Hal::Cout::PrintInfo(Form("MultidimDataManager cannot add paramter %s twice", name.Data()), EInfo::kError);
        return;
      }
    }
    FitParam param;
    param.SetParName(name);
    param.SetIsDiscrete(kTRUE);
    param.SetMapRangeByStep(low, high, step);
    fParams.push_back(param);
  }

  void MultiDimDataManager::AddParameter(FitParam& param) {
    for (auto i : fParams) {
      if (i.GetParName() == param.GetParName()) {
        Hal::Cout::PrintInfo(Form("MultidimDataManager cannot add paramter %s twice", param.GetParName().Data()), EInfo::kError);
        return;
      }
    }
    fParams.push_back(param);
  }

  void MultiDimDataManager::Init() {
    for (auto& i : fParams)
      i.Init();
    fMultiFactors = GetMultiFactors();
  }

  Int_t MultiDimDataManager::GetIndexInt(std::vector<int> paramsId) const {
    if (paramsId.size() != fParams.size()) return -1;
    Int_t step   = 1;
    Int_t pos    = 0;
    auto factors = GetMultiFactors();
    for (int i = 0; i < fParams.size(); i++) {
      pos += fMultiFactors[i] * paramsId[i];
    }
    /*   for (int i = fParams.size() - 1; i >= 0; i--) {
         pos += paramsId[i] * step;
         step *= fParams[i].GetNPoints();
       }*/
    return pos;
  }

  Int_t MultiDimDataManager::GetIndexAny(std::vector<double> paramsVal) const {
    if (paramsVal.size() != fParams.size()) return -1;
    Int_t step = 1;
    Int_t pos  = 0;
    for (int i = 0; i < fParams.size(); i++) {
      Double_t dif          = paramsVal[i] - fParams[i].GetMapMin();
      Double_t overStepSize = fParams[i].GetOverStepSize();
      Int_t bin             = TMath::Nint(dif * overStepSize);
      pos += bin * fMultiFactors[i];
    }
    return pos;
  }

  std::vector<Double_t> MultiDimDataManager::GetValues(Int_t entry) const {
    std::vector<Double_t> res(fParams.size());
    std::cout << " GET ENTRY " << entry << std::endl;
    for (int i = 0; i < fParams.size(); i++) {
      int step = TMath::Nint(entry / fMultiFactors[i]);
      entry -= fMultiFactors[i] * step;
      res[i] = fParams[i].GetMapMin() + double(step) * fParams[i].GetStepSize();
    }
    return res;
  }

  std::vector<Int_t> MultiDimDataManager::GetIndexes(Int_t entry) const {
    std::vector<Int_t> res(fParams.size());
    for (int i = 0; i < fParams.size(); i++) {
      res[i] = TMath::Nint(entry / fMultiFactors[i]);
      entry -= res[i] * fParams[i].GetNPoints();
    }
    return res;
  }

  void MultiDimDataManager::Print(Option_t* option) const {
    std::cout << "MultiDimDataManager " << this->GetName() << std::endl;
    Hal::Cout::Database({"ParName", "MapMin", "MapMax", "Npoints", "Step"});
    for (auto i : fParams) {
      Hal::Cout::Database({i.GetParName(),
                           Form("%4.4f", i.GetMapMin()),
                           Form("%4.4f", i.GetMapMax()),
                           Form("%d", i.GetNPoints()),
                           Form("%4.4f", i.GetStepSize())});
    }
  }

  std::vector<Int_t> MultiDimDataManager::GetMultiFactors() const {
    const int params = fParams.size();
    std::vector<Int_t> temp(params), res(params);
    for (unsigned int i = 0; i < params; i++) {
      temp[i] = fParams[i].GetNPoints();
    }
    res[res.size() - 1] = 1;
    if (params > 1)
      for (int i = params - 2; i >= 0; i--) {
        res[i] = res[i + 1] * temp[i + 1];
      }
    return res;
  }

} /* namespace Hal */
