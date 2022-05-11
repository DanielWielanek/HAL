/*
 * Parameter.cpp
 *
 *  Created on: 21-06-2013
 *      Author: Daniel Wielanek
 */

#include "Cout.h"
#include "HalStd.h"
#include <TBrowser.h>
#include <TCollection.h>
#include <TNamed.h>
#include <TPaveText.h>
#include <TVirtualPad.h>

#include "HalStdString.h"
#include "Parameter.h"

#if !defined(__CINT__)
// templateClassImp(HalStringsPlus);
#endif
//---------------- BASE CLASS -----------------------------------------
namespace Hal {
  Parameter::Parameter() : fMergeMode(Parameter::EParameterMerge::kEqual) { fName = "NULL"; }

  Parameter::Parameter(TString name, Char_t merge_mode) {
    fName = name;
    SetMergeMode(merge_mode);
  }

  void Parameter::SetMergeMode(Char_t mergemode) {
    if (mergemode == '+') {
      fMergeMode = EParameterMerge::kPlus;
    } else if (mergemode == '=') {
      fMergeMode = EParameterMerge::kEqual;
    } else if (mergemode == '*') {
      fMergeMode = EParameterMerge::kMultiply;
    } else if (mergemode == 'f') {
      fMergeMode = EParameterMerge::kFirst;
    } else if (mergemode == 'l') {
      fMergeMode = EParameterMerge::kLast;
    } else if (mergemode == 'M') {
      fMergeMode = EParameterMerge::kMaximum;
    } else if (mergemode == 'm') {
      fMergeMode = EParameterMerge::kMinimum;
    } else if (mergemode == 'a') {
      fMergeMode = EParameterMerge::kAverage;
    } else {
      Cout::PrintInfo("Wrong merge mode in Parameter", Hal::EInfo::kLessWarning);
    }
  }

  Bool_t Parameter::CheckName(const TObject* obj) {
    TString name_first = this->GetName();
    TString name_sec   = obj->GetName();
    if (name_first.EqualTo(name_sec)) {
      return kTRUE;
    } else {
      Cout::PrintInfo(
        Form("Warning in merging name %s differ than %s in class %s", this->GetName(), obj->GetName(), this->ClassName()),
        Hal::EInfo::kLessWarning);
      return kTRUE;
    }
  }

  void Parameter::Browse(TBrowser* /*b*/) {
    gPad->Clear();
    gPad->Range(0, 0, 1, 1);
    TPaveText* pt = new TPaveText(0, 0, 1, 1);
    pt->AddText(ClassName());
    pt->AddLine(0.0, 0.33, 1.0, 0.33);
    pt->AddText(GetName());
    pt->AddLine(0.0, 0.66, 1.0, 0.66);
    pt->AddText(HTMLExtract(0));
    pt->Draw();
  }

  Parameter::~Parameter() {}
  //------------------------------------------------------------- Double_t
  ParameterDouble::ParameterDouble() : Parameter(), fValue(0.0) {}

  ParameterDouble::ParameterDouble(TString name, Double_t value, Char_t merge_mode) : Parameter(name, merge_mode) {
    fValue = value;
  }

  void ParameterDouble::Add(const Object* val) {
    if (CheckName(val)) {
      const ParameterDouble* val_typed = (ParameterDouble*) val;
      switch (fMergeMode) {
        case EParameterMerge::kPlus: {
          fValue += val_typed->GetValue();
        } break;
        case EParameterMerge::kEqual: {
          if (fValue != val_typed->GetValue()) {
            Cout::PrintInfo(Form("Differ values during adding %s", this->GetName()), Hal::EInfo::kLessWarning);
          }
        } break;
        case EParameterMerge::kMultiply: {
          fValue = fValue * val_typed->GetValue();
        } break;
        case EParameterMerge::kFirst: {
          // do nothing this->fValue = fValue;
        } break;
        case EParameterMerge::kLast: {
          fValue = val_typed->GetValue();
        } break;
        case EParameterMerge::kMaximum: {
          if (fValue < val_typed->GetValue()) { fValue = val_typed->GetValue(); }
        } break;
        case EParameterMerge::kMinimum: {
          if (fValue > val_typed->GetValue()) { fValue = val_typed->GetValue(); }
        } break;
        case EParameterMerge::kAverage: {
          fValue = fValue * 0.5 + val_typed->GetValue() * 0.5;
        } break;
        default:
          Cout::PrintInfo(Form("Unknow merging flag druing merging %s", this->GetName()), Hal::EInfo::kLessError);
          break;
      }
    }
  }

  TString ParameterDouble::HTMLExtract(Int_t /*counter*/, TString /*dir*/) const { return Form("%4.3f", fValue); }

  ParameterDouble::~ParameterDouble() {}
  //-------------------------------------------------------------Float_t
  //------------------------------------------------------------- Double_t
  ParameterFloat::ParameterFloat() : Parameter(), fValue(0.0) {}

  ParameterFloat::ParameterFloat(TString name, Float_t value, Char_t merge_mode) : Parameter(name, merge_mode) { fValue = value; }

  void ParameterFloat::Add(const Object* val) {
    if (CheckName(val)) {
      const ParameterFloat* val_typed = (ParameterFloat*) val;
      switch (fMergeMode) {
        case EParameterMerge::kPlus: {
          fValue += val_typed->GetValue();
        } break;
        case EParameterMerge::kEqual: {
          if (fValue != val_typed->GetValue()) {
            Cout::PrintInfo(Form("Differ values during adding %s", this->GetName()), Hal::EInfo::kLessWarning);
          }
        } break;
        case EParameterMerge::kMultiply: {
          fValue = fValue * val_typed->GetValue();
        } break;
        case EParameterMerge::kFirst: {
          // do nothing this->fValue = fValue;
        } break;
        case EParameterMerge::kLast: {
          fValue = val_typed->GetValue();
        } break;
        case EParameterMerge::kAverage: {
          fValue = fValue * 0.5 + val_typed->GetValue() * 0.5;
        } break;
        case EParameterMerge::kMaximum: {
          if (fValue < val_typed->GetValue()) { fValue = val_typed->GetValue(); }
        } break;
        case EParameterMerge::kMinimum: {
          if (fValue > val_typed->GetValue()) { fValue = val_typed->GetValue(); }
        } break;
        default:
          Cout::PrintInfo(Form("Unknow merging flag druing merging %s", this->GetName()), Hal::EInfo::kLessError);
          break;
      }
    }
  }

  TString ParameterFloat::HTMLExtract(Int_t /*counter*/, TString /*dir*/) const { return Form("%4.3f", fValue); }

  ParameterFloat::~ParameterFloat() {}
  //-------------------------------------------------------------TString
  ParameterString::ParameterString() : Parameter() {}

  ParameterString::ParameterString(TString name, TString value, Char_t merge_mode) : Parameter(name, merge_mode) {
    fValue = value;
  }

  void ParameterString::Add(const Object* val) {
    if (CheckName(val)) {
      ParameterString* val_typed = (ParameterString*) val;
      switch (fMergeMode) {
        case EParameterMerge::kPlus: {
          fValue = fValue + val_typed->GetValue();
        } break;
        case EParameterMerge::kEqual: {
          if (fValue.EqualTo(val_typed->GetValue()) == kFALSE) {
            Cout::PrintInfo(Form("Differ values during adding %s", this->GetName()), Hal::EInfo::kLessWarning);
          }
        } break;
        case EParameterMerge::kMultiply: {
          Cout::PrintInfo("Multiply flag in ParameterString", Hal::EInfo::kLessWarning);
        } break;
        case EParameterMerge::kFirst: {
          // do nothing this->fValue = fValue;
        } break;
        case EParameterMerge::kLast: {
          fValue = val_typed->GetValue();
        } break;
        case EParameterMerge::kMaximum: {
          if (fValue.Length() < val_typed->GetValue().Length()) { fValue = val_typed->GetValue(); }
        } break;
        case EParameterMerge::kMinimum: {
          if (fValue.Length() > val_typed->GetValue().Length()) { fValue = val_typed->GetValue(); }
        } break;
        default:
          Cout::PrintInfo(Form("Unknow merging flag druing merging %s", this->GetName()), Hal::EInfo::kLessError);
          break;
      }
    }
  }

  TString ParameterString::HTMLExtract(Int_t /*counter*/, TString /*dir*/) const { return fValue; }

  ParameterString::~ParameterString() {}
  //-------------------------------------------------------------Uint_t
  ParameterUInt::ParameterUInt() : Parameter(), fValue(0) {}

  ParameterUInt::ParameterUInt(TString name, UInt_t value, Char_t merge_mode) : Parameter(name, merge_mode) { fValue = value; }

  void ParameterUInt::Add(const Object* val) {
    if (CheckName(val)) {
      ParameterUInt* val_typed = (ParameterUInt*) val;
      switch (fMergeMode) {
        case EParameterMerge::kPlus: {
          fValue += val_typed->GetValue();
        } break;
        case EParameterMerge::kEqual: {
          if (fValue != val_typed->GetValue()) {
            Cout::PrintInfo(Form("Differ values during adding %s", this->GetName()), Hal::EInfo::kLessWarning);
          }
        } break;
        case EParameterMerge::kMultiply: {
          fValue = fValue * val_typed->GetValue();
        } break;
        case EParameterMerge::kFirst: {
          // do nothing this->fValue = fValue;
        } break;
        case EParameterMerge::kLast: {
          fValue = val_typed->GetValue();
        } break;
        case EParameterMerge::kMaximum: {
          if (fValue < val_typed->GetValue()) { fValue = val_typed->GetValue(); }
        } break;
        case EParameterMerge::kMinimum: {
          if (fValue > val_typed->GetValue()) { fValue = val_typed->GetValue(); }
        } break;
        default:
          Cout::PrintInfo(Form("Unknow merging flag druing merging %s", this->GetName()), Hal::EInfo::kLessError);
          break;
      }
    }
  }

  TString ParameterUInt::HTMLExtract(Int_t /*counter*/, TString /*dir*/) const {
    return HalStd::RoundToString(fValue, 0, "separators");
  }

  ParameterUInt::~ParameterUInt() {}
  //------------------------------------------------------------ULong_t
  ParameterULong::ParameterULong() : Parameter(), fValue(0) {}

  ParameterULong::ParameterULong(TString name, ULong_t value, Char_t merge_mode) : Parameter(name, merge_mode) { fValue = value; }

  void ParameterULong::Add(const Object* val) {
    if (CheckName(val)) {
      ParameterULong* val_typed = (ParameterULong*) val;
      switch (fMergeMode) {
        case EParameterMerge::kPlus: {
          fValue += val_typed->GetValue();
        } break;
        case EParameterMerge::kEqual: {
          if (fValue != val_typed->GetValue()) {
            Cout::PrintInfo(Form("Differ values during adding %s", this->GetName()), Hal::EInfo::kLessWarning);
          }
        } break;
        case EParameterMerge::kMultiply: {
          fValue = fValue * val_typed->GetValue();
        } break;
        case EParameterMerge::kFirst: {
          // do nothing this->fValue = fValue;
        } break;
        case EParameterMerge::kLast: {
          fValue = val_typed->GetValue();
        } break;
        case EParameterMerge::kAverage: {
          fValue = fValue * 0.5 + val_typed->GetValue() * 0.5;
        } break;
        case EParameterMerge::kMaximum: {
          if (fValue < val_typed->GetValue()) { fValue = val_typed->GetValue(); }
        } break;
        case EParameterMerge::kMinimum: {
          if (fValue > val_typed->GetValue()) { fValue = val_typed->GetValue(); }
        } break;
        default:
          Cout::PrintInfo(Form("Unknow merging flag druing merging %s", this->GetName()), Hal::EInfo::kLessError);
          break;
      }
    }
  }

  TString ParameterULong::HTMLExtract(Int_t /*counter*/, TString /*dir*/) const {
    return HalStd::RoundToString(fValue, 0, "separators");
  }

  ParameterULong::~ParameterULong() {}
  //-------------------------------------------------------------ULong64_t
  ParameterULong64::ParameterULong64() : Parameter(), fValue(0) {}

  ParameterULong64::ParameterULong64(TString name, ULong64_t value, Char_t merge_mode) : Parameter(name, merge_mode) {
    fValue = value;
  }

  void ParameterULong64::Add(const Object* val) {
    if (CheckName(val)) {
      ParameterULong64* val_typed = (ParameterULong64*) val;
      switch (fMergeMode) {
        case EParameterMerge::kPlus: {
          fValue += val_typed->GetValue();
        } break;
        case EParameterMerge::kEqual: {
          if (fValue != val_typed->GetValue()) {
            Cout::PrintInfo(Form("Differ values during adding %s", this->GetName()), Hal::EInfo::kLessWarning);
          }
        } break;
        case EParameterMerge::kMultiply: {
          fValue = fValue * val_typed->GetValue();
        } break;
        case EParameterMerge::kAverage: {
          fValue = fValue * 0.5 + val_typed->GetValue() * 0.5;
        } break;
        case EParameterMerge::kFirst: {
          // do nothing this->fValue = fValue;
        } break;
        case EParameterMerge::kLast: {
          fValue = val_typed->GetValue();
        } break;
        case EParameterMerge::kMaximum: {
          if (fValue < val_typed->GetValue()) { fValue = val_typed->GetValue(); }
        } break;
        case EParameterMerge::kMinimum: {
          if (fValue > val_typed->GetValue()) { fValue = val_typed->GetValue(); }
        } break;
        default:
          Cout::PrintInfo(Form("Unknow merging flag druing merging %s", this->GetName()), Hal::EInfo::kLessError);
          break;
      }
    }
  }

  TString ParameterULong64::HTMLExtract(Int_t /*counter*/, TString /*dir*/) const {
    { return HalStd::RoundToString(fValue, 0, "separators"); }
  }

  ParameterULong64::~ParameterULong64() {}
  //--------------------------------------------------------------Int_t

  ParameterInt::ParameterInt() : Parameter(), fValue(0) {}

  ParameterInt::ParameterInt(TString name, Int_t value, Char_t merge_mode) : Parameter(name, merge_mode) { fValue = value; }

  void ParameterInt::Add(const Object* val) {
    if (CheckName(val)) {
      ParameterInt* val_typed = (ParameterInt*) val;
      switch (fMergeMode) {
        case EParameterMerge::kPlus: {
          fValue += val_typed->GetValue();
        } break;
        case EParameterMerge::kEqual: {
          if (fValue != val_typed->GetValue()) {
            Cout::PrintInfo(Form("Differ values during adding %s", this->GetName()), Hal::EInfo::kLessWarning);
          }
        } break;
        case EParameterMerge::kMultiply: {
          fValue = fValue * val_typed->GetValue();
        } break;
        case EParameterMerge::kAverage: {
          fValue = fValue * 0.5 + val_typed->GetValue() * 0.5;
        } break;
        case EParameterMerge::kFirst: {
          // do nothing this->fValue = fValue;
        } break;
        case EParameterMerge::kLast: {
          fValue = val_typed->GetValue();
        } break;
        case EParameterMerge::kMaximum: {
          if (fValue < val_typed->GetValue()) { fValue = val_typed->GetValue(); }
        } break;
        case EParameterMerge::kMinimum: {
          if (fValue > val_typed->GetValue()) { fValue = val_typed->GetValue(); }
        } break;
        default:
          Cout::PrintInfo(Form("Unknow merging flag druing merging %s", this->GetName()), Hal::EInfo::kLessError);
          break;
      }
    }
  }

  TString ParameterInt::HTMLExtract(Int_t /*counter*/, TString /*dir*/) const {
    return HalStd::RoundToString(GetValue(), 0, "separators");
  }

  ParameterInt::~ParameterInt() {}

  //------------------------------------------------------------- Bool_t
  ParameterBool::ParameterBool() : Parameter(), fValue(kFALSE) {}

  ParameterBool::ParameterBool(TString name, Bool_t value, Char_t merge_mode) : Parameter(name, merge_mode) { fValue = value; }

  void ParameterBool::Add(const Object* val) {
    if (CheckName(val)) {
      ParameterBool* val_typed = (ParameterBool*) val;
      switch (fMergeMode) {
        case EParameterMerge::kPlus: {
          fValue += val_typed->GetValue();
        } break;
        case EParameterMerge::kEqual: {
          if (fValue != val_typed->GetValue()) {
            Cout::PrintInfo(Form("Differ values during adding %s", this->GetName()), Hal::EInfo::kLessWarning);
          }
        } break;
        case EParameterMerge::kMultiply: {
          fValue = fValue && val_typed->GetValue();
        } break;
        case EParameterMerge::kLast: {
          fValue = val_typed->GetValue();
        } break;
        case EParameterMerge::kMaximum: {
          if (fValue < val_typed->GetValue()) { fValue = val_typed->GetValue(); }
        } break;
        case EParameterMerge::kMinimum: {
          if (fValue > val_typed->GetValue()) { fValue = val_typed->GetValue(); }
        } break;
        default:
          Cout::PrintInfo(Form("Unknow merging flag druing merging %s", this->GetName()), Hal::EInfo::kLessError);
          break;
      }
    }
  }

  ParameterBool::~ParameterBool() {}

  TString ParameterBool::HTMLExtract(Int_t /*counter*/, TString /*dir*/) const {
    if (fValue) {
      return "kTRUE";
    } else {
      return "kFALSE";
    }
  }

  Long64_t Parameter::Merge(TCollection* collection) {
    if (collection) {
      Parameter* pack = NULL;
      TIter iterator(collection);
      while ((pack = (Parameter*) iterator())) {
        Add(pack);
      }
    }
    return 1;
  }
}  // namespace Hal
