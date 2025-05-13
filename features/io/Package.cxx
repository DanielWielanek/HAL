/*
 * HalPackage.cxx
 *
 *  Created on: 11-07-2013
 *      Author: Daniel Wielanek
 */

#include "Package.h"

#include "Cout.h"
#include "Parameter.h"
#include "Std.h"
#include "StdString.h"

#include <TBrowser.h>
#include <TCollection.h>
#include <TH1.h>
#include <TList.h>
#include <TObjArray.h>
#include <TPRegexp.h>
#include <TSeqCollection.h>

namespace Hal {
  Package::Package() {
    fArrayObjects = new TObjArray(1);
    fArrayObjects->SetName("ArrayObjects");
    fArrayObjects->SetOwner(kTRUE);
    fNo        = 0;
    fNameClass = ("Hal::Package");
    fComment   = " ";
    fMerged    = 1;
  }

  Package::Package(const TObject* object, Bool_t cut_template) {
    fArrayObjects = new TObjArray(1);
    fArrayObjects->SetName("ArrayObjects");
    fArrayObjects->SetOwner(kTRUE);
    fNo        = 0;
    fNameClass = (object->ClassName());
    if (cut_template) {
      TString temp = fNameClass.String();
      TPRegexp regs("[A-z,0-9,:]+");
      TString temp2 = temp(regs);
      fNameClass.SetString(temp2);
    }
    fComment = " ";
    fMerged  = 1;
  }

  void Package::SetAdditionalCounter(Int_t counter) { fNameClass = (Form("%s_%i", fNameClass.String().Data(), counter)); }

  TObject* Package::GetObject(Int_t i) const {
    if (i < fNo && i >= 0)
      return fArrayObjects->At(i);
    else {
      Cout::PrintInfo("No object at such index in Hal::Package", Hal::EInfo::kWarning);
      return NULL;
    }
  }

  Int_t Package::GetEntries() const { return fNo; }

  TString Package::GetComment() const { return fComment.GetString(); }

  void Package::SetComment(TString name) { fComment.SetString(name); }

  TObject* Package::GetObjectByName(TString name, Int_t index, Bool_t quiet) const {
    Int_t counter = 0;
    for (Int_t i = 0; i < fNo; i++) {
      TString temp_name = ((fArrayObjects->At(i))->GetName());
      if (name.EqualTo(temp_name)) {
        counter++;
        if (counter > index) return fArrayObjects->At(i);
      }
    }
    if (!quiet)
      Cout::PrintInfo(Form("object named %s not found in package %s", name.Data(), (fNameClass.GetString()).Data()),
                      Hal::EInfo::kLowWarning);
    return NULL;
  }

  void Package::PrintInfo(Int_t counter, Int_t deep) const {
    if (deep == 0) Cout::Stars(kCyan);
    Cout::ShiftInStars(deep, Form("Hal::Package Info"), kCyan);
    Cout::ShiftInStars(deep, Form("ClassName : %s", (fNameClass.GetString()).Data()), kGreen);
    Cout::ShiftText(deep, Form("Comment : %s", fComment.GetString().Data()), "L");
    Cout::ShiftInStars(deep, "Main List", -1);
    Cout::ShiftDatabase(deep, 3, "Index", "ObjectType", "ObjectName");
    for (Int_t i = 0; i < fNo; i++) {
      TString index = Hal::Std::RoundToString(i);
      TString type  = (fArrayObjects->At(i))->ClassName();
      TString name  = (fArrayObjects->At(i))->GetName();
      Cout::ShiftDatabase(deep, 3, index.Data(), type.Data(), name.Data());
    }
    // print lists
    if (counter >= deep) {
      Cout::ShiftInStars(deep, "List of exotic objects", kGreen);
      for (int i = 0; i < fNo; i++) {
        TString index = Hal::Std::RoundToString(i);
        TString type  = (fArrayObjects->At(i))->ClassName();
        TString name  = (fArrayObjects->At(i))->GetName();
        if (type == "TList") {
          TList* list = (TList*) fArrayObjects->At(i);
          // looop over TList
          Cout::ShiftInStars(deep, Form("TList %s", list->GetName()), kBlue);
          for (int j = 0; j <= list->LastIndex(); j++) {
            if (list->At(j)->ClassName() == this->ClassName()) {
              Cout::ShiftInStars(deep, "Hal::Packages in TList!", kCyan);
              ((Package*) list->At(j))->PrintInfo(counter, deep + 1);
            } else {
              // other object
              TString Index = Hal::Std::RoundToString(j);
              TString Type  = list->At(j)->ClassName();
              TString Name  = list->At(j)->GetName();
              Cout::ShiftDatabase(deep, 3, Index.Data(), Type.Data(), Name.Data());
            }
          }
          Cout::ShiftInStars(deep, Form("TList %s End", list->GetName()), kBlue);
          delete list;
        } else if (name == this->ClassName()) {
          Cout::ShiftInStars(deep, Form("Ha::lPackage in Hal::Package! [No : %i]", i), kCyan);
          ((Package*) fArrayObjects->At(i))->PrintInfo(counter, deep + 1);
        }
      }
    }
    Cout::ShiftInStars(deep, Form("End of %s package", fNameClass.GetString().Data()), kCyan);
  }

  Package* Package::Report() const {
    Package* pack = new Package(this, kTRUE);
    pack->AddObject(new ParameterString("ClassName", "Hal::Package"));
    // pack->AddObject(new
    // HalParameterString("SubClassName",fNameClass.String()));
    for (Int_t i = 0; i < fNo; i++) {
      pack->AddObject(fArrayObjects->At(i));
    }
    return pack;
  }

  Int_t Package::GetMerged() const { return fMerged; }

  void Package::SumHalParameter(TObject* first, const TObject* second) {
    TString name = first->ClassName();
    ((Parameter*) first)->Add((Parameter*) second);
  }

  void Package::SumHistograms(TObject* first, const TObject* second) {
    TString name = first->ClassName();
    ((TH1*) first)->Add((TH1*) second);
  }

  void Package::SumOther(TObject* first, const TObject* second) {
    Cout::PrintInfo("Not supported adding", Hal::EInfo::kLowWarning);
    Cout::PrintInfo(Form("For class %s %s", second->ClassName(), first->ClassName()), Hal::EInfo::kLowWarning);
  }

  void Package::SumPackages(TObject* first, const TObject* second) { ((Package*) first)->Add((Package*) second); }

  void Package::SumList(TObject* first, const TObject* second) {
    TList* fst = (TList*) first;
    TList* sec = (TList*) second;
    for (int i = 0; i < fst->GetEntries(); i++) {
      MergeObjects(fst->At(i), sec->At(i));
    }
  }

  const char* Package::GetName() const { return fNameClass.GetName(); }

  TObject* Package::GetObjectByClassName(TString name, Int_t index) const {
    Int_t counter = 0;
    for (Int_t i = 0; i < fNo; i++) {
      TString namet = (fArrayObjects->At(i))->ClassName();
      if (name.EqualTo(namet)) {
        counter++;
        if (counter > index) return fArrayObjects->At(i);
      } else {
      }
    }
    Cout::PrintInfo(Form("class %s not found in package %s", name.Data(), (fNameClass.GetString()).Data()), Hal::EInfo::kWarning);
    return NULL;
  }

  void Package::SumPack(TObject* first, const TObject* second) { ((Object*) first)->Add((Object*) second); }

  void Package::SumTObjString(TObject* first, const TObject* second) {
    TString stringA = ((TObjString*) first)->GetString();
    TString stringB = ((TObjString*) second)->GetString();
    if (stringA.EqualTo(stringB)) {
      // do nothing
    } else {
      Cout::PrintInfo(Form("TObjString not compatible (%s vs %s", stringA.Data(), stringB.Data()), Hal::EInfo::kLowWarning);
    }
  }

  void Package::Browse(TBrowser* b) {
    if (fArrayObjects) {
      for (int i = 0; i < fArrayObjects->GetEntries(); i++) {
        TObject* obj = fArrayObjects->At(i);
        b->Add(obj, obj->GetName());
      }
    }
  }

  Package::~Package() {
    fArrayObjects->Delete();
    delete fArrayObjects;
  }

  void Package::AddObject(TObject* object) {
    if (object != NULL) {
      fArrayObjects->AddLast(object);
      fNo++;
    } else {
      Cout::PrintInfo("Cannot add NULL object to Hal::Package", Hal::EInfo::kError);
    }
  }

  Package& Package::operator+(const Package& rhs) {
    if (this->fNameClass.GetString().EqualTo(rhs.fNameClass.GetString())) {
      Cout::PrintInfo("Trying to merging incompatibile Hal::Packages ! This can result in "
                      "crash !",
                      Hal::EInfo::kLowWarning);
    }
    for (Int_t i = 0; i < fNo; i++) {
      MergeObjects(this->fArrayObjects->At(i), rhs.fArrayObjects->At(i));
    }
    this->fNo += rhs.fNo;
    return *this;
  }

  void Package::MergeObjects(TObject* first, const TObject* second) {
    TString name  = first->ClassName();
    TString name2 = second->ClassName();
    if (name != name2) {
      Cout::PrintInfo(Form("No compatybile classes %s %s", first->ClassName(), second->ClassName()), Hal::EInfo::kCriticalError);
    }
    if (name == "Hal::Package") {
      SumPackages(first, second);
    } else if (first->InheritsFrom("TH1")) {
      SumHistograms(first, second);
    } else if ((first->InheritsFrom("Hal::Parameter"))) {
      SumHalParameter(first, second);
    } else if (name == "TList") {
      SumList(first, second);
    } else if (first->InheritsFrom("Hal::Object")) {
      SumPack(first, second);
    } else if (name.EqualTo("TObjString")) {
      SumTObjString(first, second);
    } else {
      SumOther(first, second);
    }
  }

  void Package::Add(const Package* pack) {
    if (this->fNameClass.GetString().EqualTo(pack->fNameClass.GetString())) {
    } else {
      Cout::PrintInfo("Trying to merging incompatibile Hal::Packages ! This can result in "
                      "crash !",
                      Hal::EInfo::kLowWarning);
    }
    if (this->fNo != pack->fNo) {
      Cout::PrintInfo("Different number of object in Hal::PackageArray this may result in "
                      "crash!",
                      Hal::EInfo::kLowWarning);
    }
    for (Int_t i = 0; i < fNo; i++) {
      TString name = (fArrayObjects->At(i)->ClassName());
      MergeObjects(this->fArrayObjects->At(i), pack->fArrayObjects->At(i));
    }
    this->fMerged += pack->fMerged;
  }

  void Package::PrintInfo(Int_t max_deep) const { PrintInfo(max_deep - 1, 0); }

  void Package::PrintStruct(Int_t I, Int_t Index) const {
    TString text = "";
    for (int i = 0; i < I; i++) {
      text += "-";
    }
    TString final_text = Form("%s %s No [%i]", text.Data(), this->ClassName(), Index);
    if (Index < 0) Index = 0;
    Cout::Text(final_text, "L");
    for (Int_t i = 0; i < fNo; i++) {
      TString index = Hal::Std::RoundToString(i);
      TString type  = (fArrayObjects->At(i))->ClassName();
      TString name  = (fArrayObjects->At(i))->GetName();
      if (name == this->ClassName()) { ((Package*) fArrayObjects->At(i))->PrintStruct(I + 1, i); }
      // HalCout::ShiftDatabase(deep,3,index.Data(),type.Data(),name.Data());
    }
  }

  void Package::PrintStruct() const {
    Cout::Stars();
    PrintStruct(0, -1);
    Cout::Stars();
  }

  void Package::SetName(TString name) { fNameClass.SetString(name.Data()); }

  Bool_t Package::Exist(TString name, Int_t index, Option_t* opt) const {
    TString option = opt;
    if (option == " ") {  // check by name && index
      Int_t counter = 0;
      for (Int_t i = 0; i < fNo; i++) {
        TString temp_name = (TString) ((fArrayObjects->At(i))->GetName());
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
        TString temp_name = (TString) ((fArrayObjects->At(i))->GetName());
        if (name.EqualTo(temp_name)) { return kTRUE; }
      }
    }
    return kFALSE;
  }

  void Package::RemoveObjectByName(TString name, Int_t index, Bool_t remove) {
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

  Long64_t Package::Merge(TCollection* collection) {
    if (collection) {
      Package* pack = NULL;
      TIter iterator(collection);
      while ((pack = (Package*) iterator())) {
        Add(pack);
      }
    }
    return 1;
  }

}  // namespace Hal
