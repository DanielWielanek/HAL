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
#include "StdString.h"
#include "XMLNode.h"

#include <iostream>
#include <vector>

namespace Hal {

  Double_t ErrorCalc::GetStatError() const { return fStatisticalUncert; }

  Double_t ErrorCalc::BarlowTest(Int_t prec, TString opt) {
    Double_t totalErr = 0;
    fUncertainties.resize(fValues.size());
    if (prec >= 0) Hal::Cout::Database({"Name", "Value", "Percent"});
    TString flag = Form("%%4.%if", prec);
    auto getForm = [&](Double_t val) { return TString(Form(flag, val)); };
    enum class option { kMean = 0, kMax = 1 };
    option intopt = option::kMax;
    if (Hal::Std::FindParam(opt, "mean")) intopt = option::kMean;
    for (int i = 0; i < fValues.size(); i++) {
      Double_t uncert = 0;
      switch (intopt) {
        case option::kMean: uncert = BarlowTestParameterMean(i); break;
        case option::kMax: uncert = BarlowTestParameterMax(i); break;
      }
      fUncertainties[i] = uncert;
      if (uncert >= 0) {
        totalErr += uncert * uncert;
        if (prec >= 0) {
          std::cout << Hal::Cout::GetColor(kGreen);
          Hal::Cout::Database({fValues[i].first, getForm(uncert), getForm(100. * uncert / GetMeasuredValue())});
          std::cout << Hal::Cout::GetDisableColor();
        }
      } else {
        if (prec >= 0) {
          std::cout << Hal::Cout::GetColor(kRed);
          Hal::Cout::Database({fValues[i].first, getForm(uncert), "-"});
          std::cout << Hal::Cout::GetDisableColor();
        }
      }
    }

    fTotalSysError = TMath::Sqrt(totalErr);
    if (prec >= 0) {
      std::cout << Hal::Cout::GetColor(kBlue);
      Hal::Cout::Database({"Total Sys", getForm(fTotalSysError), getForm(100.0 * fTotalSysError / GetMeasuredValue())});
      Hal::Cout::Database({"Statistical", getForm(GetStatError()), getForm(100. * GetStatError() / GetMeasuredValue())});
      Hal::Cout::Database({"Value", getForm(GetMeasuredValue()), " "});
      std::cout << Hal::Cout::GetDisableColor();
    }
    return totalErr;
  }

  ErrorCalc::ErrorCalc(TString name, Double_t val, Double_t err) {
    ErrorCalc::SetName(name);
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

  Double_t ErrorCalc::SumError(std::vector<Double_t> vec) {
    double sq = 0;
    for (auto el : vec) {
      sq += el * el;
    }
    return TMath::Sqrt(sq);
  }

  Double_t ErrorCalc::BarlowTestParameterMean(Int_t parId) const {
    auto uncerts         = fValues[parId].second;
    int m                = 0;
    double total_uncert2 = 0;
    for (int i = 0; i < uncerts.size(); i++) {
      double value      = uncerts[i].first;
      double uncert     = uncerts[i].second;
      double par_diff   = TMath::Abs(fMeasurement - value);
      double sigma_diff = TMath::Sqrt(TMath::Abs(fStatisticalUncert * fStatisticalUncert - uncert * uncert));
      if (par_diff < sigma_diff) {  // no contribution
                                    // ignore
      } else {
        double real_uncert = TMath::Sqrt(par_diff * par_diff - sigma_diff * sigma_diff);
        m++;
        total_uncert2 += real_uncert * real_uncert;
      }
    }
    if (m <= 0) return -1;
    return TMath::Sqrt(total_uncert2 / m);
  }


  Double_t ErrorCalc::BarlowTestParameterMax(Int_t parId) const {
    auto uncerts        = fValues[parId].second;
    int m               = 0;
    double total_uncert = 0;
    for (int i = 0; i < uncerts.size(); i++) {
      double value      = uncerts[i].first;
      double uncert     = uncerts[i].second;
      double par_diff   = TMath::Abs(fMeasurement - value);
      double sigma_diff = TMath::Sqrt(TMath::Abs(fStatisticalUncert * fStatisticalUncert - uncert * uncert));
      if (par_diff < sigma_diff) {  // no contribution
                                    // ignore
      } else {
        double real_uncert = TMath::Sqrt(par_diff * par_diff - sigma_diff * sigma_diff);
        total_uncert       = TMath::Max(total_uncert, real_uncert);
        m++;
      }
    }
    if (m <= 0) return -1;
    return total_uncert;
  }

  Double_t ErrorCalc::GetSysError(Int_t par) const {
    if (fValues.size() == 0) {
      Hal::Cout::PrintInfo("ErrorCalc::GetSystError - cannot get error lack of values !", EInfo::kError);
      return -1;
    }
    if (fUncertainties.size() == 0) {
      Hal::Cout::PrintInfo("ErrorCalc::GetSystError - cannot get error, did you call BarlowTest?", EInfo::kError);
      return -1;
    }
    if (par < 0 || par >= fUncertainties.size()) {
      Hal::Cout::PrintInfo("ErrorCalc::GetSystError - cannot find error with given index!", EInfo::kError);
      return -1;
    }
    return fUncertainties[par];
  }

  Double_t ErrorCalc::GetSysError(TString name) const {
    if (fValues.size() == 0) {
      Hal::Cout::PrintInfo("ErrorCalc::GetSystError - cannot get error lack of values !", EInfo::kError);
      return -1;
    }
    if (fUncertainties.size() == 0) {
      Hal::Cout::PrintInfo("ErrorCalc::GetSystError - cannot get error, did you call BarlowTest?", EInfo::kError);
      return -1;
    }
    for (int i = 0; i < fValues.size(); i++) {
      auto parName = fValues[i].first;
      if (parName == name) { return fUncertainties[i]; }
    }
    Hal::Cout::PrintInfo("ErrorCalc::GetSystError - cannot find error with given index!", EInfo::kError);
    return -1;
  }

  Double_t ErrorCalc::GetTotalSysError() const {
    if (fValues.size() == 0) {
      Hal::Cout::PrintInfo("ErrorCalc::GetSystError - cannot get error lack of values !", EInfo::kError);
      return -1;
    }
    if (fUncertainties.size() == 0) {
      Hal::Cout::PrintInfo("ErrorCalc::GetSystError - cannot get error, did you call BarlowTest?", EInfo::kError);
      return -1;
    }
    return fTotalSysError;
  }

  void ErrorCalc::ExportToXML(TString name, Int_t precision) {
    if (fUncertainties.size() == 0 || fValues.size() == 0 || fUncertainties.size() != fValues.size()) {
      Hal::Cout::PrintInfo("ErrorCalc::ExportToXML - cannot get error lack of values/uncertainties !", EInfo::kError);
      return;
    }
    TString precFlag = Form("%%4.%if", precision);
    if (precision < 0) precFlag = "%f";
    Hal::XMLFile file(name, "recreate");
    file.CreateRootNode("sys_errors");
    auto& root = file.GetRootNode();
    root.AddChild(ExportToXMLNode(precision));
  }

  Hal::XMLNode ErrorCalc::ExportToXMLNode(Int_t prec) {
    if (fUncertainties.size() == 0 || fValues.size() == 0 || fUncertainties.size() != fValues.size()) {
      Hal::Cout::PrintInfo("ErrorCalc::ExportToXMLNode - cannot get error lack of values/uncertainties !", EInfo::kError);
      return XMLNode::NullNode();
    }
    TString precFlag = Form("%%4.%if", prec);
    if (prec < 0) precFlag = "%f";

    Hal::XMLNode root("parameter", "");
    root.AddAttrib("parName", GetName());
    for (int i = 0; i < fValues.size(); i++) {
      Hal::XMLNode errorNode("SysError", fValues[i].first);
      errorNode.AddAttrib("Value", Form(precFlag, GetSysError(i)));
      root.AddChild(errorNode);
    }
    Hal::XMLNode errorNode("TotalSysError", Form(precFlag, GetTotalSysError()));
    Hal::XMLNode statNode("StatError", Form(precFlag, GetStatError()));
    Hal::XMLNode value("Measured", Form(precFlag, GetMeasuredValue()));


    root.AddChild(errorNode);
    root.AddChild(statNode);
    root.AddChild(value);
    return root;
  }

} /* namespace Hal */
