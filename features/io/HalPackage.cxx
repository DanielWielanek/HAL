/*
 * HalPackage.cxx
 *
 *  Created on: 11-07-2013
 *      Author: Daniel Wielanek
 */

#include "HalPackage.h"

#include "HalCout.h"
#include "HalParameter.h"
#include "HalStd.h"
#include "HalStdString.h"

#include <TBrowser.h>
#include <TCollection.h>
#include <TH1.h>
#include <TList.h>
#include <TObjArray.h>
#include <TPRegexp.h>
#include <TSeqCollection.h>


HalPackage::HalPackage() {
  fArrayObjects = new TObjArray(1);
  fArrayObjects->SetName("ArrayObjects");
  fArrayObjects->SetOwner(kTRUE);
  fNo        = 0;
  fNameClass = ("HalPackage");
  fComment   = " ";
  fMerged    = 1;
}

HalPackage::HalPackage(const TObject* object, Bool_t cut_template) {
  fArrayObjects = new TObjArray(1);
  fArrayObjects->SetName("ArrayObjects");
  fArrayObjects->SetOwner(kTRUE);
  fNo        = 0;
  fNameClass = (object->ClassName());
  if (cut_template) {
    TString temp = fNameClass.String();
    TPRegexp regs("[A-z,0-9]+");
    TString temp2 = temp(regs);
    fNameClass.SetString(temp2);
  }
  fComment = " ";
  fMerged  = 1;
}

void HalPackage::SetAdditionalCounter(Int_t counter) { fNameClass = (Form("%s_%i", fNameClass.String().Data(), counter)); }

TObject* HalPackage::GetObject(Int_t i) const {
  if (i < fNo && i >= 0)
    return fArrayObjects->At(i);
  else {
    HalCout::PrintInfo("No object at such index in HalPackage", Hal::EInfo::kImportantWarning);
    return NULL;
  }
}

Int_t HalPackage::GetEntries() const { return fNo; }

TString HalPackage::GetComment() const { return fComment.GetString(); }

void HalPackage::SetComment(TString name) { fComment.SetString(name); }

TObject* HalPackage::GetObjectByName(TString name, Int_t index) const {
  Int_t counter = 0;
  for (Int_t i = 0; i < fNo; i++) {
    TString temp_name = ((fArrayObjects->At(i))->GetName());
    if (name.EqualTo(temp_name)) {
      counter++;
      if (counter > index) return fArrayObjects->At(i);
    }
  }
  HalCout::PrintInfo(Form("object named %s not found in package %s", name.Data(), (fNameClass.GetString()).Data()),
                      Hal::EInfo::kLessWarning);
  return NULL;
}

void HalPackage::PrintInfo(Int_t counter, Int_t deep) const {
  if (deep == 0) HalCout::Stars(kCyan);
  HalCout::ShiftInStars(deep, Form("HalPackage Info"), kCyan);
  HalCout::ShiftInStars(deep, Form("ClassName : %s", (fNameClass.GetString()).Data()), kGreen);
  HalCout::ShiftText(deep, Form("Comment : %s", fComment.GetString().Data()), "L");
  HalCout::ShiftInStars(deep, "Main List", -1);
  HalCout::ShiftDatabase(deep, 3, "Index", "ObjectType", "ObjectName");
  for (Int_t i = 0; i < fNo; i++) {
    TString index = HalStd::RoundToString(i);
    TString type  = (fArrayObjects->At(i))->ClassName();
    TString name  = (fArrayObjects->At(i))->GetName();
    HalCout::ShiftDatabase(deep, 3, index.Data(), type.Data(), name.Data());
  }
  // print lists
  if (counter >= deep) {
    HalCout::ShiftInStars(deep, "List of exotic objects", kGreen);
    for (int i = 0; i < fNo; i++) {
      TString index = HalStd::RoundToString(i);
      TString type  = (fArrayObjects->At(i))->ClassName();
      TString name  = (fArrayObjects->At(i))->GetName();
      if (type == "TList") {
        TList* list = (TList*) fArrayObjects->At(i);
        // looop over TList
        HalCout::ShiftInStars(deep, Form("TList %s", list->GetName()), kBlue);
        for (int j = 0; j <= list->LastIndex(); j++) {
          if (list->At(j)->ClassName() == this->ClassName()) {
            HalCout::ShiftInStars(deep, "HalPackages in TList!", kCyan);
            ((HalPackage*) list->At(j))->PrintInfo(counter, deep + 1);
          } else {
            // other object
            TString Index = HalStd::RoundToString(j);
            TString Type  = list->At(j)->ClassName();
            TString Name  = list->At(j)->GetName();
            HalCout::ShiftDatabase(deep, 3, Index.Data(), Type.Data(), Name.Data());
          }
        }
        HalCout::ShiftInStars(deep, Form("TList %s End", list->GetName()), kBlue);
        delete list;
      } else if (name == this->ClassName()) {
        HalCout::ShiftInStars(deep, Form("HalPackage in HalPackage! [No : %i]", i), kCyan);
        ((HalPackage*) fArrayObjects->At(i))->PrintInfo(counter, deep + 1);
      }
    }
  }
  HalCout::ShiftInStars(deep, Form("End of %s package", fNameClass.GetString().Data()), kCyan);
}

HalPackage* HalPackage::Report() const {
  HalPackage* pack = new HalPackage(this, kTRUE);
  pack->AddObject(new HalParameterString("ClassName", "HalPackage"));
  // pack->AddObject(new
  // HalParameterString("SubClassName",fNameClass.String()));
  for (Int_t i = 0; i < fNo; i++) {
    pack->AddObject(fArrayObjects->At(i));
  }
  return pack;
}

Int_t HalPackage::GetMerged() const { return fMerged; }

void HalPackage::SumHalParameter(TObject* first, const TObject* second) {
  TString name = first->ClassName();
  ((HalParameter*) first)->Add((HalParameter*) second);
}

void HalPackage::SumHistograms(TObject* first, const TObject* second) {
  TString name = first->ClassName();
  ((TH1*) first)->Add((TH1*) second);
}

void HalPackage::SumOther(TObject* first, const TObject* second) {
  HalCout::PrintInfo("Not supported adding", Hal::EInfo::kLessWarning);
  HalCout::PrintInfo(Form("For class %s %s", second->ClassName(), first->ClassName()), Hal::EInfo::kLessWarning);
}

void HalPackage::SumPackages(TObject* first, const TObject* second) { ((HalPackage*) first)->Add((HalPackage*) second); }

void HalPackage::SumList(TObject* first, const TObject* second) {
  TList* fst = (TList*) first;
  TList* sec = (TList*) second;
  for (int i = 0; i < fst->GetEntries(); i++) {
    MergeObjects(fst->At(i), sec->At(i));
  }
}

const char* HalPackage::GetName() const { return fNameClass.GetName(); }

TObject* HalPackage::GetObjectByClassName(TString name, Int_t index) const {
  Int_t counter = 0;
  for (Int_t i = 0; i < fNo; i++) {
    TString namet = (fArrayObjects->At(i))->ClassName();
    if (name.EqualTo(namet)) {
      counter++;
      if (counter > index) return fArrayObjects->At(i);
    } else {
    }
  }
  HalCout::PrintInfo(Form("class %s not found in package %s", name.Data(), (fNameClass.GetString()).Data()),
                      Hal::EInfo::kImportantWarning);
  return NULL;
}

void HalPackage::SumPack(TObject* first, const TObject* second) { ((HalObject*) first)->Add((HalObject*) second); }

void HalPackage::SumTObjString(TObject* first, const TObject* second) {
  TString stringA = ((TObjString*) first)->GetString();
  TString stringB = ((TObjString*) second)->GetString();
  if (stringA.EqualTo(stringB)) {
    // do nothing
  } else {
    HalCout::PrintInfo(Form("TObjString not compatible (%s vs %s", stringA.Data(), stringB.Data()), Hal::EInfo::kLessWarning);
  }
}

void HalPackage::Browse(TBrowser* b) {
  if (fArrayObjects) {
    for (int i = 0; i < fArrayObjects->GetEntries(); i++) {
      TObject* obj = fArrayObjects->At(i);
      b->Add(obj, obj->GetName());
    }
  }
}

HalPackage::~HalPackage() {
  fArrayObjects->Delete();
  delete fArrayObjects;
}

void HalPackage::AddObject(TObject* object) {
  if (object != NULL) {
    fArrayObjects->AddLast(object);
    fNo++;
  } else {
    HalCout::PrintInfo("Cannot add NULL object to HalPackage", Hal::EInfo::kLessError);
  }
}

HalPackage& HalPackage::operator+(const HalPackage& rhs) {
  if (this->fNameClass.GetString().EqualTo(rhs.fNameClass.GetString())) {
    HalCout::PrintInfo("Trying to merging incompatibile HalPackages ! This can result in "
                        "crash !",
                        Hal::EInfo::kLessWarning);
  }
  for (Int_t i = 0; i < fNo; i++) {
    MergeObjects(this->fArrayObjects->At(i), rhs.fArrayObjects->At(i));
  }
  this->fNo += rhs.fNo;
  return *this;
}

void HalPackage::MergeObjects(TObject* first, const TObject* second) {
  TString name  = first->ClassName();
  TString name2 = second->ClassName();
  if (name != name2) {
    HalCout::PrintInfo(Form("No compatybile classes %s %s", first->ClassName(), second->ClassName()),
                        Hal::EInfo::kImportantError);
  }
  if (name == "HalPackage") {
    SumPackages(first, second);
  } else if (first->InheritsFrom("TH1")) {
    SumHistograms(first, second);
  } else if ((first->InheritsFrom("HalParameter"))) {
    SumHalParameter(first, second);
  } else if (name == "TList") {
    SumList(first, second);
  } else if (first->InheritsFrom("HalPack")) {
    SumPack(first, second);
  } else if (name.EqualTo("TObjString")) {
    SumTObjString(first, second);
  } else {
    SumOther(first, second);
  }
}

void HalPackage::Add(const HalPackage* pack) {
  if (this->fNameClass.GetString().EqualTo(pack->fNameClass.GetString())) {
  } else {
    HalCout::PrintInfo("Trying to merging incompatibile HalPackages ! This can result in "
                        "crash !",
                        Hal::EInfo::kLessWarning);
  }
  if (this->fNo != pack->fNo) {
    HalCout::PrintInfo("Different number of object in HalPackageArray this may result in "
                        "crash!",
                        Hal::EInfo::kLessWarning);
  }
  for (Int_t i = 0; i < fNo; i++) {
    TString name = (fArrayObjects->At(i)->ClassName());
    MergeObjects(this->fArrayObjects->At(i), pack->fArrayObjects->At(i));
  }
  this->fMerged += pack->fMerged;
}

void HalPackage::PrintInfo(Int_t max_deep) const { PrintInfo(max_deep - 1, 0); }

void HalPackage::PrintStruct(Int_t I, Int_t Index) const {
  TString text = "";
  for (int i = 0; i < I; i++) {
    text += "-";
  }
  TString final_text = Form("%s %s No [%i]", text.Data(), this->ClassName(), Index);
  if (Index < 0) Index = 0;
  HalCout::Text(final_text, "L");
  for (Int_t i = 0; i < fNo; i++) {
    TString index = HalStd::RoundToString(i);
    TString type  = (fArrayObjects->At(i))->ClassName();
    TString name  = (fArrayObjects->At(i))->GetName();
    if (name == this->ClassName()) { ((HalPackage*) fArrayObjects->At(i))->PrintStruct(I + 1, i); }
    // HalCout::ShiftDatabase(deep,3,index.Data(),type.Data(),name.Data());
  }
}

void HalPackage::PrintStruct() const {
  HalCout::Stars();
  PrintStruct(0, -1);
  HalCout::Stars();
}

void HalPackage::SetName(TString name) { fNameClass.SetString(name.Data()); }

Bool_t HalPackage::Exist(TString name, Int_t index, Option_t* opt) const {
  TString option = opt;
  if (option == " ") {  // check by name && index
    Int_t counter = 0;
    for (Int_t i = 0; i < fNo; i++) {
      TString temp_name = (TString)((fArrayObjects->At(i))->GetName());
      if (name.EqualTo(temp_name)) {
        counter++;
        if (counter > index) return kTRUE;
      }
    }
  } else if (option == "index") {
    if (index < fNo && index >= 0) return kTRUE;
  } else if (option == "classname") {
    Int_t counter = 0;
    for (Int_t i = 0; i < fNo; i++) {
      TString namet = (fArrayObjects->At(i))->ClassName();
      if (name.EqualTo(namet)) {
        counter++;
        if (counter > index) return kTRUE;
      }
    }
  } else if (option == "name") {  // only check by name
    for (Int_t i = 0; i < fNo; i++) {
      TString temp_name = (TString)((fArrayObjects->At(i))->GetName());
      if (name.EqualTo(temp_name)) { return kTRUE; }
    }
  }
  return kFALSE;
}

void HalPackage::RemoveObjectByName(TString name, Int_t index, Bool_t remove) {
  Int_t counter = 0;
  TObject* obj  = NULL;
  for (Int_t i = 0; i < fNo; i++) {
    TString temp_name = ((fArrayObjects->At(i))->GetName());
    if (name.EqualTo(temp_name)) {
      counter++;
      if (counter > index) {
        obj = fArrayObjects->RemoveAt(i);
        fNo--;
        fArrayObjects->Compress();
      }
    }
  }
  if (obj && remove) { delete obj; }
}

Long64_t HalPackage::Merge(TCollection* collection) {
  if (collection) {
    HalPackage* pack = NULL;
    TIter iterator(collection);
    while ((pack = (HalPackage*) iterator())) {
      Add(pack);
    }
  }
  return 1;
}
