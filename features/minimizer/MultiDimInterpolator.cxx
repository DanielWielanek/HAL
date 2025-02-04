/*
 * MultiDimExtrapolator.cxx
 *
 *  Created on: 16 sty 2025
 *      Author: daniel
 */

#include "MultiDimInterpolator.h"

#include "CorrelationHisto.h"
#include "Cout.h"
#include "MultiDimDataManager.h"
#include "MultiDimFile.h"

#include <TFile.h>
#include <TNtuple.h>
#include <TString.h>

#include <iostream>

namespace Hal {

  void MultiDimInterpolator::OpenFile(TString file) { fInFile = new Hal::MultiDimFile(file); }

  Hal::CorrelationHisto* MultiDimInterpolator::GetCorrHisto() const {
    Hal::CorrelationHisto* res = new Hal::CorrelationHisto(fInFile->GetConfig()->GetParametersNo());
    auto params                = fInFile->GetConfig()->GetParams();
    for (int i = 0; i < params.size(); i++) {
      res->ConfigParam(i,
                       params[i].GetNPoints(),
                       params[i].GetMapMin() - 0.5 * params[i].GetStepSize(),
                       params[i].GetMapMax() + 0.5 * params[i].GetStepSize(),
                       params[i].GetParName());
    }
    res->Init();
    auto valuesTree = fInFile->GetValues();
    std::vector<Double_t> values(valuesTree->size() - 1);
    for (int i = 0; i < fInFile->GetEntries(); i++) {
      fInFile->GetEntry(i);
      for (int j = 0; j < values.size(); j++) {
        values[j] = (*valuesTree)[j + 1];
      }
      res->FillUnchecked(values, (*valuesTree)[0]);
    }
    return res;
  }

  Double_t MultiDimInterpolator::Extrapolate(const std::vector<Double_t>& x) const {
    const int dimensions = fInFile->GetConfig()->GetParametersNo();
    // if (dimensions == 0 || size.size() != min_values.size() || size.size() != max_values.size() || size.size() != x.size()) {
    //   throw std::invalid_argument("Dimension mismatch in input parameters.");
    //}

    // Znajdź dolne/górne indeksy i wagi dla każdego wymiaru
    std::vector<int> lower(dimensions), upper(dimensions);
    std::vector<double> weight(dimensions);
    for (int i = 0; i < dimensions; i++) {
      auto param        = fInFile->GetConfig()->GetParam(i);
      double scaled_pos = (x[i] - param.GetMapMin()) * param.GetOverStepSize();
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
        stride *= fInFile->GetConfig()->GetParam(i).GetNPoints();
      }

      fInFile->GetEntry(index);
      result += vertex_weight * (*fInFile->GetValues())[0];
    }

    return result;
  }

  MultiDimDataManager* MultiDimInterpolator::GetConfig() const { return fInFile->GetConfig(); }

  MultiDimInterpolator::~MultiDimInterpolator() {
    if (fInFile) delete fInFile;
  }

} /* namespace Hal */
