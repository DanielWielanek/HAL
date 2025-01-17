/*
 * MultiDimExtrapolator.cxx
 *
 *  Created on: 16 sty 2025
 *      Author: daniel
 */

#include "MultiDimExtrapolator.h"

#include "CorrelationHisto.h"
#include "Cout.h"

#include <TFile.h>
#include <TNtuple.h>
#include <TString.h>

#include <iostream>

namespace Hal {

  void MultiDimExtrapolator::LoadFromTFile(TString file) {
    fFile        = new TFile(file);
    fTree        = (TTree*) fFile->Get("data");
    fDataManager = (MultiDimDataManager*) fFile->Get("info");
    fTree->SetBranchAddress("vector", &fValues);
    fTree->GetEntry(0);
    if (fValues->size() != 0) {
      if (fValues->size() - 1 != fDataManager->GetSize()) {
        Hal::Cout::PrintInfo(Form("Incompatible data %lu expected %i", fValues->size() - 1, fDataManager->GetSize()),
                             EInfo::kError);
      }
    }
  }

  Hal::CorrelationHisto* MultiDimExtrapolator::GetCorrHisto() const {
    Hal::CorrelationHisto* res = new Hal::CorrelationHisto(fDataManager->GetSize());
    auto params                = fDataManager->GetParams();
    for (int i = 0; i < params.size(); i++) {
      res->ConfigParam(i,
                       params[i].GetNPoints(),
                       params[i].GetMapMin() - 0.5 * params[i].GetDParam(),
                       params[i].GetMapMax() + 0.5 * params[i].GetDParam(),
                       params[i].GetParName());
    }
    res->Init();
    std::vector<Double_t> values(fValues->size() - 1);
    for (int i = 0; i < fTree->GetEntries(); i++) {
      fTree->GetEntry(i);
      for (int j = 0; j < values.size(); j++) {
        values[j] = (*fValues)[j + 1];
      }
      res->FillUnchecked(values, (*fValues)[0]);
    }
    return res;
  }

  Double_t MultiDimExtrapolator::Extrapolate(std::vector<Double_t>& x) const {
    const int dimensions = fDataManager->GetSize();
    // if (dimensions == 0 || size.size() != min_values.size() || size.size() != max_values.size() || size.size() != x.size()) {
    //   throw std::invalid_argument("Dimension mismatch in input parameters.");
    //}

    // Znajdź dolne/górne indeksy i wagi dla każdego wymiaru
    std::vector<int> lower(dimensions), upper(dimensions);
    std::vector<double> weight(dimensions);
    for (int i = 0; i < dimensions; i++) {
      auto param        = fDataManager->GetParam(i);
      double scaled_pos = (x[i] - param.GetMapMin()) * param.GetOverDParam();
      lower[i]          = std::floor(scaled_pos);
      upper[i]          = std::min(lower[i] + 1, param.GetNPoints() - 1);
      weight[i]         = scaled_pos - lower[i];
    }

    // Iteruj przez wszystkie wierzchołki hipersześcianu
    double result    = 0.0;
    int num_vertices = 1 << dimensions;  // 2^dimensions
    for (int vertex = 0; vertex < num_vertices; vertex++) {
      double vertex_weight = 1.0;
      int index            = 0;
      int stride           = 1;

      for (int i = dimensions - 1; i >= 0; i--) {
        bool is_upper = (vertex & (1 << i)) != 0;  // Sprawdź, czy i-ty bit jest 1
        vertex_weight *= is_upper ? weight[i] : (1.0 - weight[i]);
        index += (is_upper ? upper[i] : lower[i]) * stride;
        stride *= fDataManager->GetParam(i).GetNPoints();
      }

      fTree->GetEntry(index);
      result += vertex_weight * (*fValues)[0];
    }

    return result;
  }

  MultiDimExtrapolator::~MultiDimExtrapolator() {
    if (fFile) delete fFile;
  }

} /* namespace Hal */
