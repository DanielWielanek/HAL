/*
 * HalOptions.cxx
 *
 *  Created on: 21-10-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "Options.h"

#include <TObjString.h>
#include <TRegexp.h>

#include <iostream>

namespace Hal {
  Options::Options(TString option) : fOriginalOpt(option) {
    option = option.ReplaceAll(" ", "+");
    option.ToLower();
    while (option.Contains("++"))
      option = option.ReplaceAll("++", "+");
    auto vector = Hal::Std::ExplodeString(option, '+', kFALSE);

    for (auto word : vector) {
      if (word.Contains("{")) {
        ParseBrackets(word);
      } else if (word.Contains("=")) {
        auto pair = Hal::Std::ExplodeString(word, '=', kTRUE);
        if (pair.size() == 2) {
          Flag expr;
          expr.flag  = pair[0];
          expr.value = pair[1];
          fEqualExpr.push_back(expr);
        }
      } else {
        fOptions.push_back(word);
      }
    }
  }

  void Options::ParseBrackets(TString bracket) {
    bracket.ReplaceAll("}", "");
    bracket.ReplaceAll("{", "");
    if (bracket.Contains("=")) {  // expression like {A=0,0,0}
      LabeledArray res;
      auto parsed = Hal::Std::ExplodeString(bracket, '=', false);
      if (parsed.size() < 1) return;
      res.name           = parsed[0];
      auto numberscommas = parsed[1];
      auto numbers       = Hal::Std::ExplodeString(numberscommas, ',', false);
      for (auto number : numbers) {
        res.values.push_back(number.Atof());
      }
      fLabeledArray.push_back(res);
    }
  }

  Bool_t Options::HasOption(TString flag) const {
    for (auto i : fOptions) {
      if (i == flag) return kTRUE;
    }
    return kFALSE;
  }

  Bool_t Options::HasNotOption(TString flag) const {
    flag = "!" + flag;
    return HasOption(flag);
  }

  Options::LabeledArray Options::GetLabeledArray(TString name) const {
    for (auto label : fLabeledArray) {
      if (label.name == name) return label;
    }
    return Options::LabeledArray();
  }

  TString Options::GetFlagValue(TString flag) const {
    for (auto label : fEqualExpr) {
      if (label.flag == flag) return label.value;
    }
    return "";
  }

  void Options::Print(Option_t* option) const {
    std::cout << "== " << ClassName() << " ==" << std::endl;
    std::cout << "Options:" << std::endl;
    for (auto opt : fOptions) {
      std::cout << "\t" << opt << std::endl;
    }
    std::cout << "Flags: flag [value]" << std::endl;
    for (auto flag : fEqualExpr) {
      std::cout << flag.flag << " [" << flag.value << "]" << std::endl;
    }

    std::cout << "ArraysL: name [values]" << std::endl;
    for (auto flag : fLabeledArray) {
      std::cout << "\t" << flag.name << " [";
      for (auto val : flag.values) {
        std::cout << Form("%4.2f ", val);
      }
      std::cout << "]" << std::endl;
    }
  }

  //================================================================

  MainOption::MainOption(int argc, char* argv[]) {
    std::vector<TString> arguments;
    for (int i = 1; i < argc; i++) {
      arguments.push_back(argv[i]);
    }
    for (auto i : arguments) {
      if (i.BeginsWith("-")) {
        auto res = Hal::Std::ExplodeString(i, '=', kFALSE);
        if (res.size() == 2) {
          auto param = res[0];
          std::pair<TString, TString> pars;
          pars.first  = param.ReplaceAll("-", "");
          pars.second = res[1];
          fParams.push_back(pars);
        } else {
          std::pair<TString, TString> pars;
          pars.first  = res[0];
          pars.second = "";
          fParams.push_back(pars);
        }
      } else {
        fArgs.push_back(i);
      }
    }
  };

  TString MainOption::GetParameterValue(TString par) const {
    for (auto i : fParams) {
      if (i.first == par) return i.second;
    }
    return "";
  }

  Bool_t MainOption::HaveParameter(TString opt) const {
    for (auto i : fParams) {
      if (i.first == opt) return kTRUE;
    }
    return kFALSE;
  }

  void MainOption::Print(Option_t* option) const {
    std::cout << "MainOption::Print" << std::endl;
    std::cout << "Arguments:" << std::endl;
    for (int i = 0; i < fArgs.size(); i++) {
      std::cout << Form("\t#%i %s", i, fArgs[i].Data()) << std::endl;
    }
    std::cout << "Parameters:" << std::endl;
    for (int i = 0; i < fParams.size(); i++) {
      std::cout << Form("\t%s:  %s", fParams[i].first.Data(), fParams[i].second.Data()) << std::endl;
    }
  }

}  // namespace Hal
