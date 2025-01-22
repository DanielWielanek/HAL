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
  }

  Int_t MultiDimDataManager::GetIndexInt(std::vector<int> paramsId) const {
    if (paramsId.size() != fParams.size()) return -1;
    Int_t step = 1;
    Int_t pos  = 0;
    for (int i = fParams.size() - 1; i >= 0; i--) {
      pos += paramsId[i] * step;
      step *= fParams[i].GetNPoints();
    }
    return pos;
  }

  Int_t MultiDimDataManager::GetIndexAny(std::vector<double> paramsVal) const {
    if (paramsVal.size() != fParams.size()) return -1;
    Int_t step = 1;
    Int_t pos  = 0;
    for (int i = fParams.size() - 1; i >= 0; i--) {
      Double_t par          = paramsVal[i];
      Double_t dif          = par - fParams[i].GetMapMin();
      Double_t overStepSize = fParams[i].GetOverStepSize();
      Int_t bin             = dif * overStepSize;
      pos += bin * step;
      step *= fParams[i].GetNPoints();
    }
    return pos;
  }

  std::vector<Double_t> MultiDimDataManager::GetValues(Int_t entry) const {
    std::vector<Double_t> res(fParams.size());
    for (int i = 0; i < fParams.size(); i++) {
      res[i] = entry % fParams[i].GetNPoints();
      res[i] = fParams[i].GetMapMin() + res[i] * fParams[i].GetStepSize();
      entry /= fParams[i].GetNPoints();
    }
    return res;
  }

  std::vector<Int_t> MultiDimDataManager::GetIndexes(Int_t entry) const {
    std::vector<Int_t> res(fParams.size());
    for (int i = 0; i < fParams.size(); i++) {
      res[i] = entry % fParams[i].GetNPoints();
      entry /= fParams[i].GetNPoints();
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

} /* namespace Hal */
