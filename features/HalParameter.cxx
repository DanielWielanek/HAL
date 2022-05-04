/*
 * HalParameter.cpp
 *
 *  Created on: 21-06-2013
 *      Author: Daniel Wielanek
 */

#include "HalParameter.h"
#include "HalCout.h"
#include "HalStd.h"
#include <TBrowser.h>
#include <TCollection.h>
#include <TNamed.h>
#include <TPaveText.h>
#include <TVirtualPad.h>

#include "HalStdString.h"

#if !defined(__CINT__)
// templateClassImp(HalStringsPlus);
#endif
//---------------- BASE CLASS -----------------------------------------
HalParameter::HalParameter() : fMergeMode(HalParameter::EHalParameterMerge::kEqual) { fName = "NULL"; }

HalParameter::HalParameter(TString name, Char_t merge_mode) {
  fName = name;
  SetMergeMode(merge_mode);
}

void HalParameter::SetMergeMode(Char_t mergemode) {
  if (mergemode == '+') {
    fMergeMode = EHalParameterMerge::kPlus;
  } else if (mergemode == '=') {
    fMergeMode = EHalParameterMerge::kEqual;
  } else if (mergemode == '*') {
    fMergeMode = EHalParameterMerge::kMultiply;
  } else if (mergemode == 'f') {
    fMergeMode = EHalParameterMerge::kFirst;
  } else if (mergemode == 'l') {
    fMergeMode = EHalParameterMerge::kLast;
  } else if (mergemode == 'M') {
    fMergeMode = EHalParameterMerge::kMaximum;
  } else if (mergemode == 'm') {
    fMergeMode = EHalParameterMerge::kMinimum;
  } else if (mergemode == 'a') {
    fMergeMode = EHalParameterMerge::kAverage;
  } else {
    HalCout::PrintInfo("Wrong merge mode in HalParameter", Hal::EInfo::kLessWarning);
  }
}

Bool_t HalParameter::CheckName(const TObject* obj) {
  TString name_first = this->GetName();
  TString name_sec   = obj->GetName();
  if (name_first.EqualTo(name_sec)) {
    return kTRUE;
  } else {
    HalCout::PrintInfo(
      Form("Warning in merging name %s differ than %s in class %s", this->GetName(), obj->GetName(), this->ClassName()),
      Hal::EInfo::kLessWarning);
    return kTRUE;
  }
}

void HalParameter::Browse(TBrowser* /*b*/) {
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

HalParameter::~HalParameter() {}
//------------------------------------------------------------- Double_t
HalParameterDouble::HalParameterDouble() : HalParameter(), fValue(0.0) {}

HalParameterDouble::HalParameterDouble(TString name, Double_t value, Char_t merge_mode) : HalParameter(name, merge_mode) {
  fValue = value;
}

void HalParameterDouble::Add(const HalObject* val) {
  if (CheckName(val)) {
    const HalParameterDouble* val_typed = (HalParameterDouble*) val;
    switch (fMergeMode) {
      case EHalParameterMerge::kPlus: {
        fValue += val_typed->GetValue();
      } break;
      case EHalParameterMerge::kEqual: {
        if (fValue != val_typed->GetValue()) {
          HalCout::PrintInfo(Form("Differ values during adding %s", this->GetName()), Hal::EInfo::kLessWarning);
        }
      } break;
      case EHalParameterMerge::kMultiply: {
        fValue = fValue * val_typed->GetValue();
      } break;
      case EHalParameterMerge::kFirst: {
        // do nothing this->fValue = fValue;
      } break;
      case EHalParameterMerge::kLast: {
        fValue = val_typed->GetValue();
      } break;
      case EHalParameterMerge::kMaximum: {
        if (fValue < val_typed->GetValue()) { fValue = val_typed->GetValue(); }
      } break;
      case EHalParameterMerge::kMinimum: {
        if (fValue > val_typed->GetValue()) { fValue = val_typed->GetValue(); }
      } break;
      case EHalParameterMerge::kAverage: {
        fValue = fValue * 0.5 + val_typed->GetValue() * 0.5;
      } break;
      default: HalCout::PrintInfo(Form("Unknow merging flag druing merging %s", this->GetName()), Hal::EInfo::kLessError); break;
    }
  }
}

TString HalParameterDouble::HTMLExtract(Int_t /*counter*/, TString /*dir*/) const { return Form("%4.3f", fValue); }

HalParameterDouble::~HalParameterDouble() {}
//-------------------------------------------------------------Float_t
//------------------------------------------------------------- Double_t
HalParameterFloat::HalParameterFloat() : HalParameter(), fValue(0.0) {}

HalParameterFloat::HalParameterFloat(TString name, Float_t value, Char_t merge_mode) : HalParameter(name, merge_mode) {
  fValue = value;
}

void HalParameterFloat::Add(const HalObject* val) {
  if (CheckName(val)) {
    const HalParameterFloat* val_typed = (HalParameterFloat*) val;
    switch (fMergeMode) {
      case EHalParameterMerge::kPlus: {
        fValue += val_typed->GetValue();
      } break;
      case EHalParameterMerge::kEqual: {
        if (fValue != val_typed->GetValue()) {
          HalCout::PrintInfo(Form("Differ values during adding %s", this->GetName()), Hal::EInfo::kLessWarning);
        }
      } break;
      case EHalParameterMerge::kMultiply: {
        fValue = fValue * val_typed->GetValue();
      } break;
      case EHalParameterMerge::kFirst: {
        // do nothing this->fValue = fValue;
      } break;
      case EHalParameterMerge::kLast: {
        fValue = val_typed->GetValue();
      } break;
      case EHalParameterMerge::kAverage: {
        fValue = fValue * 0.5 + val_typed->GetValue() * 0.5;
      } break;
      case EHalParameterMerge::kMaximum: {
        if (fValue < val_typed->GetValue()) { fValue = val_typed->GetValue(); }
      } break;
      case EHalParameterMerge::kMinimum: {
        if (fValue > val_typed->GetValue()) { fValue = val_typed->GetValue(); }
      } break;
      default: HalCout::PrintInfo(Form("Unknow merging flag druing merging %s", this->GetName()), Hal::EInfo::kLessError); break;
    }
  }
}

TString HalParameterFloat::HTMLExtract(Int_t /*counter*/, TString /*dir*/) const { return Form("%4.3f", fValue); }

HalParameterFloat::~HalParameterFloat() {}
//-------------------------------------------------------------TString
HalParameterString::HalParameterString() : HalParameter() {}

HalParameterString::HalParameterString(TString name, TString value, Char_t merge_mode) : HalParameter(name, merge_mode) {
  fValue = value;
}

void HalParameterString::Add(const HalObject* val) {
  if (CheckName(val)) {
    HalParameterString* val_typed = (HalParameterString*) val;
    switch (fMergeMode) {
      case EHalParameterMerge::kPlus: {
        fValue = fValue + val_typed->GetValue();
      } break;
      case EHalParameterMerge::kEqual: {
        if (fValue.EqualTo(val_typed->GetValue()) == kFALSE) {
          HalCout::PrintInfo(Form("Differ values during adding %s", this->GetName()), Hal::EInfo::kLessWarning);
        }
      } break;
      case EHalParameterMerge::kMultiply: {
        HalCout::PrintInfo("Multiply flag in HalParameterString", Hal::EInfo::kLessWarning);
      } break;
      case EHalParameterMerge::kFirst: {
        // do nothing this->fValue = fValue;
      } break;
      case EHalParameterMerge::kLast: {
        fValue = val_typed->GetValue();
      } break;
      case EHalParameterMerge::kMaximum: {
        if (fValue.Length() < val_typed->GetValue().Length()) { fValue = val_typed->GetValue(); }
      } break;
      case EHalParameterMerge::kMinimum: {
        if (fValue.Length() > val_typed->GetValue().Length()) { fValue = val_typed->GetValue(); }
      } break;
      default: HalCout::PrintInfo(Form("Unknow merging flag druing merging %s", this->GetName()), Hal::EInfo::kLessError); break;
    }
  }
}

TString HalParameterString::HTMLExtract(Int_t /*counter*/, TString /*dir*/) const { return fValue; }

HalParameterString::~HalParameterString() {}
//-------------------------------------------------------------Uint_t
HalParameterUInt::HalParameterUInt() : HalParameter(), fValue(0) {}

HalParameterUInt::HalParameterUInt(TString name, UInt_t value, Char_t merge_mode) : HalParameter(name, merge_mode) {
  fValue = value;
}

void HalParameterUInt::Add(const HalObject* val) {
  if (CheckName(val)) {
    HalParameterUInt* val_typed = (HalParameterUInt*) val;
    switch (fMergeMode) {
      case EHalParameterMerge::kPlus: {
        fValue += val_typed->GetValue();
      } break;
      case EHalParameterMerge::kEqual: {
        if (fValue != val_typed->GetValue()) {
          HalCout::PrintInfo(Form("Differ values during adding %s", this->GetName()), Hal::EInfo::kLessWarning);
        }
      } break;
      case EHalParameterMerge::kMultiply: {
        fValue = fValue * val_typed->GetValue();
      } break;
      case EHalParameterMerge::kFirst: {
        // do nothing this->fValue = fValue;
      } break;
      case EHalParameterMerge::kLast: {
        fValue = val_typed->GetValue();
      } break;
      case EHalParameterMerge::kMaximum: {
        if (fValue < val_typed->GetValue()) { fValue = val_typed->GetValue(); }
      } break;
      case EHalParameterMerge::kMinimum: {
        if (fValue > val_typed->GetValue()) { fValue = val_typed->GetValue(); }
      } break;
      default: HalCout::PrintInfo(Form("Unknow merging flag druing merging %s", this->GetName()), Hal::EInfo::kLessError); break;
    }
  }
}

TString HalParameterUInt::HTMLExtract(Int_t /*counter*/, TString /*dir*/) const {
  return HalStd::RoundToString(fValue, 0, "separators");
}

HalParameterUInt::~HalParameterUInt() {}
//------------------------------------------------------------ULong_t
HalParameterULong::HalParameterULong() : HalParameter(), fValue(0) {}

HalParameterULong::HalParameterULong(TString name, ULong_t value, Char_t merge_mode) : HalParameter(name, merge_mode) {
  fValue = value;
}

void HalParameterULong::Add(const HalObject* val) {
  if (CheckName(val)) {
    HalParameterULong* val_typed = (HalParameterULong*) val;
    switch (fMergeMode) {
      case EHalParameterMerge::kPlus: {
        fValue += val_typed->GetValue();
      } break;
      case EHalParameterMerge::kEqual: {
        if (fValue != val_typed->GetValue()) {
          HalCout::PrintInfo(Form("Differ values during adding %s", this->GetName()), Hal::EInfo::kLessWarning);
        }
      } break;
      case EHalParameterMerge::kMultiply: {
        fValue = fValue * val_typed->GetValue();
      } break;
      case EHalParameterMerge::kFirst: {
        // do nothing this->fValue = fValue;
      } break;
      case EHalParameterMerge::kLast: {
        fValue = val_typed->GetValue();
      } break;
      case EHalParameterMerge::kAverage: {
        fValue = fValue * 0.5 + val_typed->GetValue() * 0.5;
      } break;
      case EHalParameterMerge::kMaximum: {
        if (fValue < val_typed->GetValue()) { fValue = val_typed->GetValue(); }
      } break;
      case EHalParameterMerge::kMinimum: {
        if (fValue > val_typed->GetValue()) { fValue = val_typed->GetValue(); }
      } break;
      default: HalCout::PrintInfo(Form("Unknow merging flag druing merging %s", this->GetName()), Hal::EInfo::kLessError); break;
    }
  }
}

TString HalParameterULong::HTMLExtract(Int_t /*counter*/, TString /*dir*/) const {
  return HalStd::RoundToString(fValue, 0, "separators");
}

HalParameterULong::~HalParameterULong() {}
//-------------------------------------------------------------ULong64_t
HalParameterULong64::HalParameterULong64() : HalParameter(), fValue(0) {}

HalParameterULong64::HalParameterULong64(TString name, ULong64_t value, Char_t merge_mode) : HalParameter(name, merge_mode) {
  fValue = value;
}

void HalParameterULong64::Add(const HalObject* val) {
  if (CheckName(val)) {
    HalParameterULong64* val_typed = (HalParameterULong64*) val;
    switch (fMergeMode) {
      case EHalParameterMerge::kPlus: {
        fValue += val_typed->GetValue();
      } break;
      case EHalParameterMerge::kEqual: {
        if (fValue != val_typed->GetValue()) {
          HalCout::PrintInfo(Form("Differ values during adding %s", this->GetName()), Hal::EInfo::kLessWarning);
        }
      } break;
      case EHalParameterMerge::kMultiply: {
        fValue = fValue * val_typed->GetValue();
      } break;
      case EHalParameterMerge::kAverage: {
        fValue = fValue * 0.5 + val_typed->GetValue() * 0.5;
      } break;
      case EHalParameterMerge::kFirst: {
        // do nothing this->fValue = fValue;
      } break;
      case EHalParameterMerge::kLast: {
        fValue = val_typed->GetValue();
      } break;
      case EHalParameterMerge::kMaximum: {
        if (fValue < val_typed->GetValue()) { fValue = val_typed->GetValue(); }
      } break;
      case EHalParameterMerge::kMinimum: {
        if (fValue > val_typed->GetValue()) { fValue = val_typed->GetValue(); }
      } break;
      default: HalCout::PrintInfo(Form("Unknow merging flag druing merging %s", this->GetName()), Hal::EInfo::kLessError); break;
    }
  }
}

TString HalParameterULong64::HTMLExtract(Int_t /*counter*/, TString /*dir*/) const {
  { return HalStd::RoundToString(fValue, 0, "separators"); }
}

HalParameterULong64::~HalParameterULong64() {}
//--------------------------------------------------------------Int_t

HalParameterInt::HalParameterInt() : HalParameter(), fValue(0) {}

HalParameterInt::HalParameterInt(TString name, Int_t value, Char_t merge_mode) : HalParameter(name, merge_mode) {
  fValue = value;
}

void HalParameterInt::Add(const HalObject* val) {
  if (CheckName(val)) {
    HalParameterInt* val_typed = (HalParameterInt*) val;
    switch (fMergeMode) {
      case EHalParameterMerge::kPlus: {
        fValue += val_typed->GetValue();
      } break;
      case EHalParameterMerge::kEqual: {
        if (fValue != val_typed->GetValue()) {
          HalCout::PrintInfo(Form("Differ values during adding %s", this->GetName()), Hal::EInfo::kLessWarning);
        }
      } break;
      case EHalParameterMerge::kMultiply: {
        fValue = fValue * val_typed->GetValue();
      } break;
      case EHalParameterMerge::kAverage: {
        fValue = fValue * 0.5 + val_typed->GetValue() * 0.5;
      } break;
      case EHalParameterMerge::kFirst: {
        // do nothing this->fValue = fValue;
      } break;
      case EHalParameterMerge::kLast: {
        fValue = val_typed->GetValue();
      } break;
      case EHalParameterMerge::kMaximum: {
        if (fValue < val_typed->GetValue()) { fValue = val_typed->GetValue(); }
      } break;
      case EHalParameterMerge::kMinimum: {
        if (fValue > val_typed->GetValue()) { fValue = val_typed->GetValue(); }
      } break;
      default: HalCout::PrintInfo(Form("Unknow merging flag druing merging %s", this->GetName()), Hal::EInfo::kLessError); break;
    }
  }
}

TString HalParameterInt::HTMLExtract(Int_t /*counter*/, TString /*dir*/) const {
  return HalStd::RoundToString(GetValue(), 0, "separators");
}

HalParameterInt::~HalParameterInt() {}

//------------------------------------------------------------- Bool_t
HalParameterBool::HalParameterBool() : HalParameter(), fValue(kFALSE) {}

HalParameterBool::HalParameterBool(TString name, Bool_t value, Char_t merge_mode) : HalParameter(name, merge_mode) {
  fValue = value;
}

void HalParameterBool::Add(const HalObject* val) {
  if (CheckName(val)) {
    HalParameterBool* val_typed = (HalParameterBool*) val;
    switch (fMergeMode) {
      case EHalParameterMerge::kPlus: {
        fValue += val_typed->GetValue();
      } break;
      case EHalParameterMerge::kEqual: {
        if (fValue != val_typed->GetValue()) {
          HalCout::PrintInfo(Form("Differ values during adding %s", this->GetName()), Hal::EInfo::kLessWarning);
        }
      } break;
      case EHalParameterMerge::kMultiply: {
        fValue = fValue && val_typed->GetValue();
      } break;
      case EHalParameterMerge::kLast: {
        fValue = val_typed->GetValue();
      } break;
      case EHalParameterMerge::kMaximum: {
        if (fValue < val_typed->GetValue()) { fValue = val_typed->GetValue(); }
      } break;
      case EHalParameterMerge::kMinimum: {
        if (fValue > val_typed->GetValue()) { fValue = val_typed->GetValue(); }
      } break;
      default: HalCout::PrintInfo(Form("Unknow merging flag druing merging %s", this->GetName()), Hal::EInfo::kLessError); break;
    }
  }
}

HalParameterBool::~HalParameterBool() {}

TString HalParameterBool::HTMLExtract(Int_t /*counter*/, TString /*dir*/) const {
  if (fValue) {
    return "kTRUE";
  } else {
    return "kFALSE";
  }
}

Long64_t HalParameter::Merge(TCollection* collection) {
  if (collection) {
    HalParameter* pack = NULL;
    TIter iterator(collection);
    while ((pack = (HalParameter*) iterator())) {
      Add(pack);
    }
  }
  return 1;
}
