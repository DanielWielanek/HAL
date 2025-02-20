/*
 * MinimizerStepConf.cxx
 *
 *  Created on: 22 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "MinimizerStepConf.h"

#include "Cout.h"
#include "XMLNode.h"

#include <TMath.h>
#include <iostream>

namespace Hal {

  MinimizerStepConf::MinimizerStepConf() {}

  MinimizerStepConf::~MinimizerStepConf() {}

  void MinimizerStepConf::ConfigureParameter(TString name, Double_t step, Double_t min, Double_t max, TString /*option*/) {
    if (step <= 0) step = 0.01;
    if (min > max) {
      Double_t temp = min;
      min           = max;
      max           = temp;
    }

    Int_t points   = 1 + (int) TMath::Nint((max - min) / step);  //+1 to take max
    Bool_t created = kFALSE;
    for (auto& iPar : fParams) {
      if (iPar.GetParName().EqualTo(name)) {
        iPar.SetMapRange(min, max, points);
        iPar.SetIsDiscrete(kTRUE);
        created = kTRUE;
      }
    }
    if (!created) {
      FitParam param;
      param.SetParName(name);
      param.SetIsDiscrete(kTRUE);
      param.SetMapRange(min, max, points);
      fParams.push_back(param);
    }
  }

  void MinimizerStepConf::SetParameters(std::vector<FitParam>& input, Bool_t overwrite) const {
    if (overwrite) {
      input.clear();
      for (auto iPar : fParams) {
        FitParam par;
        par.SetParName(iPar.GetParName());
        par.SetMapRange(iPar.GetMapMin(), iPar.GetMapMax(), iPar.GetNPoints());
        par.SetIsDiscrete(kTRUE);
        input.push_back(par);
      }
      return;
    }
    for (auto iPar : fParams) {
      for (auto& oPar : input) {
        if (iPar.GetParName().EqualTo(oPar.GetParName())) {
          oPar.SetMapRange(iPar.GetMapMin(), iPar.GetMapMax(), iPar.GetNPoints());
          oPar.SetIsDiscrete(kTRUE);
        }
      }
    }
  }

  MinimizerStepConf::MinimizerStepConf(const MinimizerStepConf& other, std::vector<int> order) {
    if (order.size() != other.fParams.size()) {
      Hal::Cout::PrintInfo(Form("Cannot configure MinimizerStepConf::MinimizerStepConf conf pars no: %i !=%i (order no)",
                                (int) other.fParams.size(),
                                (int) order.size()),
                           EInfo::kError);
      return;
    }
    fParams.resize(other.fParams.size());
    for (unsigned int i = 0; i < order.size(); i++) {
      fParams[i] = other.fParams[order[i]];
    }
  }

  void MinimizerStepConf::LoadFromXML(TString xmlFile) {
    XMLFile file(xmlFile);
    auto root = file.GetRootNode();
    for (int i = 0; i < root->GetNChildren(); i++) {
      auto child   = root->GetChild(i);
      TString name = child->GetName();
      name.ToLower();
      if (name == "param") {
        auto minim     = child->GetAttrib("min");
        auto maxim     = child->GetAttrib("max");
        auto step      = child->GetAttrib("step");
        auto paramname = child->GetAttrib("name");
        if (minim && maxim && name && step) {
          double min_val  = minim->GetValue().Atof();
          double max_val  = maxim->GetValue().Atof();
          double dx       = step->GetValue().Atof();
          TString parname = paramname->GetValue();
          ConfigureParameter(parname, dx, min_val, max_val, "");
        }
      }
    }
  }

} /* namespace Hal */
