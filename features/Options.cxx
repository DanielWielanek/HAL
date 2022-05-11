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
namespace Hal {
  OptionConverter::OptionConverter() {}

  Bool_t OptionConverter::GetOptionInt(Int_t& val, TString option) const {
    int i = 0;
    for (auto strval : fNames) {
      i++;
      if (strval.EqualTo(option)) {
        val = fValues[i];
        return kTRUE;
      }
    }
    if (HalStd::FindParam(option, "forced:")) {
      Cout::PrintInfo("replacing HalFastCut by force !", Hal::EInfo::kLessWarning);
      TRegexp reg("[9-0]+");
      option = option(reg);
      val    = option.Atoi();
      return kTRUE;
    }

    return kFALSE;
  }

  void OptionConverter::RegisterOption(TString option) {
    fNames.push_back(option);
    fValues.push_back(fValues.size());
  }

  void OptionConverter::RegisterOption(TString option, Int_t no) {
    int size = fNames.size();
    fNames.push_back(option);
    fValues.push_back(no);
  }

  Bool_t OptionConverter::GetString(TString& string, Int_t value) const {
    if (value > 0 && value < fNames.size()) {
      string = fNames[value];
      return kTRUE;
    } else {
      return kFALSE;
    }
  }

  OptionConverter::~OptionConverter() {}

  //==================================================

  OptionArray::OptionArray() {}

  void OptionArray::RegisterLabel(TString label) {
    for (unsigned int i = 0; i < fLabels.size(); i++) {
      TString tmp = fLabels[i];
      if (tmp == label) {
        Cout::PrintInfo(Form("Label %s already registered", label.Data()), Hal::EInfo::kLessWarning);
        return;
      }
    }
    if (label.Contains(":") || label.Contains("NULL")) {
      Cout::PrintInfo("Label can't contain ':' or NULL", Hal::EInfo::kLessError);
      return;
    }
    // to be sure that all arrays exist
    fLabels.push_back(label);
    fParameters.push_back(std::vector<TString>());
  }

  Bool_t OptionArray::Add(TString option) {
    TRegexp regexp("[0-9a-zA-Z]+:");
    TRegexp regexp2(":.[0-9a-zA-Z]+");
    TString label = option(regexp);
    label.Remove(label.Length() - 1);
    TString value = option(regexp2);
    value.Remove(0, 1);
    Int_t label_no = -1;
    for (unsigned int i = 0; i < fLabels.size(); i++) {
      if (GetLabel(i).EqualTo(label)) {
        label_no = i;
        break;
      }
    }
    if (label_no == -1) {
      Cout::PrintInfo(Form("Can't add string %s because there is no label ", label.Data()), Hal::EInfo::kImportantWarning);
      return kFALSE;
    }
    fParameters[label_no].push_back(value);
    return kTRUE;
  }

  TString OptionArray::GetByLabel(TString label, Int_t no) const {
    for (unsigned int i = 0; i < fLabels.size(); i++) {
      if (GetLabel(i).EqualTo(label)) {
        Int_t entries = fParameters[i].size();
        if (entries == 0 || entries <= no) {
          if (entries == 0) Cout::PrintInfo(Form("No entries for label %s", label.Data()), Hal::EInfo::kLessWarning);
          if (entries < no) Cout::PrintInfo(Form("Not enough entries for label %s", label.Data()), Hal::EInfo::kLessWarning);
          return "NULL";
        }
        return fParameters[i][no];
      }
    }
    return "NULL";
  }

  Int_t OptionArray::GetByLabelNo(TString label) const {
    for (unsigned int i = 0; i < fLabels.size(); i++) {
      if (GetLabel(i).EqualTo(label)) { return fParameters[i].size(); }
    }
    return 0;
  }

  TString OptionArray::GetLabel(Int_t i) const { return fLabels[i]; }

  void OptionArray::Print(Option_t* /*opt*/) const {
    for (unsigned int i = 0; i < fLabels.size(); i++) {
      Cout::Text(Form("Label %s", fLabels[i].Data()), "L", kYellow);
      for (int j = 0; j < fParameters[i].size(); j++) {
        TString val = fParameters[i][j];
        Cout::Text(val, "R", kGreen);
      }
    }
  }

  OptionArray::~OptionArray() {}
}  // namespace Hal
