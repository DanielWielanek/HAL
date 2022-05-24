/*
 * HalPackageExtractor.cxx
 *
 *  Created on: 23-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "AnaFile.h"


#include <TCollection.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TH1.h>
#include <TKey.h>
#include <TList.h>
#include <TNamed.h>
#include <TObjArray.h>
#include <TSeqCollection.h>
#include <cstdlib>
#include <cstring>

#include "Cout.h"
#include "Package.h"
#include "Package2HTML.h"
#include "Parameter.h"
#include "StdString.h"
namespace Hal {
  AnaFile::AnaFile(TString filename, TString packname) :
    fHTMLCounter(0), fMainPackageArray(NULL), fCutContainerArray(NULL), fMetaDataPack(NULL), fFilename(filename) {
    fFile = new TFile(filename);
    if (fFile->IsZombie()) {
      delete fFile;
      exit(0);
    }
    fMetaDataPack        = (Package*) fFile->Get("HalInfo/RunInfo");
    ParameterString* str = (ParameterString*) fMetaDataPack->GetObjectByName("Software ver");
    Int_t SoftVer        = Hal::Std::VersionId(str->GetValue());
    if (SoftVer <= 201612) {
      Cout::PrintInfo("This file version might be not compatible wtih current version, use "
                      "macro/path/fix_files.C to fix it",
                      Hal::EInfo::kWarning);
    }
    TDirectory* PhysDir = (TDirectory*) fFile->Get("HalPhysics");
    fPhysKeys           = PhysDir->GetListOfKeys();
    fCurrentPackID      = 0;
    fMainPackageArray   = new TObjArray();
    fCutContainerArray  = new TObjArray();
    fMainPackageArray->SetOwner(kTRUE);
    fCutContainerArray->SetOwner(kFALSE);
    if (packname != "") {
      fMaxPackID = 1;
      for (int i = 0; i < fPhysKeys->GetEntries(); i++) {
        TString name = ((TKey*) (fPhysKeys->At(0)))->GetName();
        if (name != packname) {
          fPhysKeys->RemoveAt(i);
        } else {
          fMainPackageArray->AddLast(dynamic_cast<Package*>(PhysDir->Get(name)));
          if (FindCutContainer(0) == kFALSE) { Cout::PrintInfo("Cut container not found !", Hal::EInfo::kWarning); }
        }
      }
    } else {
      for (int i = 0; i < fPhysKeys->GetEntries(); i++) {
        TString name            = ((TKey*) (fPhysKeys->At(i)))->GetName();
        TObject* pack_candidate = PhysDir->Get(name);
        if (!pack_candidate->InheritsFrom("Hal::Package")) {
          fPhysKeys->RemoveAt(i);
          delete pack_candidate;
        } else {
          fMainPackageArray->AddLast(pack_candidate);
        }
      }
      fMaxPackID = fPhysKeys->GetEntries();
      for (int i = 0; i < fMaxPackID; i++) {
        if (FindCutContainer(i) == kFALSE) { Cout::PrintInfo("Cut container not found !", Hal::EInfo::kWarning); }
      }
    }
    fMainPackageArray->Compress();
  }

  AnaFile::AnaFile(TString filename, Int_t id) :
    fHTMLCounter(0), fMainPackageArray(NULL), fCutContainerArray(NULL), fMetaDataPack(NULL), fFilename(filename) {
    fFile = new TFile(filename);
    if (fFile->IsZombie()) { exit(0); }
    fMetaDataPack        = (Package*) fFile->Get("HalInfo/RunInfo");
    ParameterString* str = (ParameterString*) fMetaDataPack->GetObjectByName("Software ver");
    Int_t SoftVer        = Hal::Std::VersionId(str->GetValue());
    if (SoftVer <= 201612) {
      Cout::PrintInfo("This file version might be not compatible wtih current version, use "
                      "macro/path/fix_files.C to fix it",
                      Hal::EInfo::kWarning);
    }
    TDirectory* PhysDir = (TDirectory*) fFile->Get("HalPhysics");
    fPhysKeys           = (TList*) PhysDir->GetListOfKeys()->Clone();
    fCurrentPackID      = 0;
    fMainPackageArray   = new TObjArray();
    fCutContainerArray  = new TObjArray();
    fMainPackageArray->SetOwner(kTRUE);
    fCutContainerArray->SetOwner(kFALSE);
    if (id >= 0) {
      fMaxPackID = 1;
      if (id > fPhysKeys->GetEntries()) {
        Cout::PrintInfo(Form("Package with id = %i  not found !", id), Hal::EInfo::kWarning);
        return;
      }
      TString name = ((TKey*) (fPhysKeys->At(id)))->GetName();
      fMainPackageArray->AddLast(dynamic_cast<Package*>(PhysDir->Get(name)));
      if (FindCutContainer(0) == kFALSE) { Cout::PrintInfo("Cut container not found !", Hal::EInfo::kWarning); }
    } else {
      for (int i = 0; i < fPhysKeys->GetEntries(); i++) {
        TString name            = ((TKey*) (fPhysKeys->At(i)))->GetName();
        TObject* pack_candidate = PhysDir->Get(name);
        if (!pack_candidate->InheritsFrom("Hal::Package")) {
          fPhysKeys->RemoveAt(i--);
          delete pack_candidate;
        } else {
          fMainPackageArray->AddLast(pack_candidate);
        }
      }
      fMaxPackID = fPhysKeys->GetEntries();
      for (int i = 0; i < fMaxPackID; i++) {
        if (FindCutContainer(i) == kFALSE) { Cout::PrintInfo("Cut container not found !", Hal::EInfo::kWarning); }
      }
    }
    fMainPackageArray->Compress();
  }

  TObject* AnaFile::GetMainObject(Int_t i) const { return GetMainPackage()->GetObject(i); }

  TObject* AnaFile::GetMainObject(TString name, Int_t index) const { return GetMainPackage()->GetObjectByName(name, index); }

  void AnaFile::PrintMonitorCutList(TList* list) const {
    Cout::Database(5, "No", "Type", "UnitX", "UnitY", "UnitZ");
    TString no, type, unitx, unity, unitz;
    for (int i = 0; i < list->GetEntries(); i++) {
      Package* mon = (Package*) list->At(i);
      TString size = mon->GetComment();
      no           = Hal::Std::RoundToString(i);
      type         = size;
      unitx = unity = unitz = "-";
      if (size == "HalCutMonitorX") {
        unitx = ((ParameterString*) mon->GetObjectByName("AxisX"))->GetValue();
      } else if (size == "HalCutMonitorXY") {
        unitx = ((ParameterString*) mon->GetObjectByName("AxisX"))->GetValue();
        unity = ((ParameterString*) mon->GetObjectByName("AxisY"))->GetValue();
      } else if (size == "HalCutMonitorXYZ") {
        unitx = ((ParameterString*) mon->GetObjectByName("AxisX"))->GetValue();
        unity = ((ParameterString*) mon->GetObjectByName("AxisY"))->GetValue();
        unitz = ((ParameterString*) mon->GetObjectByName("AxisZ"))->GetValue();
      } else {
        Cout::PrintInfo("Oops, unknown Monitor :(", Hal::EInfo::kLowWarning);
      }
      Cout::Database(5, no.Data(), type.Data(), unitx.Data(), unity.Data(), unitz.Data());
    }
  }

  Int_t AnaFile::GetMerged() const { return GetMainPackage()->GetMerged(); }

  Package* AnaFile::GetCutCollection(Hal::ECutUpdate update, Int_t no) const {
    TList* list      = NULL;
    TString listName = Form("Hal%sCutCollectionList", Hal::Std::UpdateEnumToString(update).Data());
    list             = ((TList*) GetCutContainer()->GetObjectByName(listName));
    return (Package*) list->At(no);
  }

  void AnaFile::ExportHTML(TString fdirname, Int_t task_id) const {
    if (task_id < 0) {
      Package2HTML* pack = new Package2HTML(fFilename, fdirname);
      delete pack;
    } else {
      Package2HTML* pack = new Package2HTML(GetMainPackage(), fMetaDataPack, fdirname, task_id);
      delete pack;
    }
  }

  Int_t AnaFile::GetCollectionsNo(Hal::ECutUpdate update) const {
    TString label = "";
    switch (update) {
      case Hal::ECutUpdate::kEvent: label = "Event_collections_No"; break;
      case Hal::ECutUpdate::kTrack: label = "Track_collections_No"; break;
      case Hal::ECutUpdate::kTwoTrack: label = "TwoTrack_collections_No"; break;
      case Hal::ECutUpdate::kTwoTrackBackground: label = "TwoTrack_collections_background_No"; break;
      default: return 0; break;
    }
    if (GetCutContainer()->Exist(label, 0)) { return ((ParameterInt*) GetCutContainer()->GetObjectByName(label))->GetValue(); }
    return 0;
  }

  TH1* AnaFile::GetHistogramPassed(Hal::ECutUpdate update, Int_t collection, Int_t no) const {
    Package* pack = GetCutCollection(update, collection);
    TList* lista  = (TList*) pack->GetObjectByName("CutMonitorList");
    if (lista == NULL) return NULL;
    Package* cutmon = (Package*) lista->At(no);  //(HalPackage*)pack->GetObject(no);
    return (TH1*) cutmon->GetObjectByName("Passed");
  }

  TH1* AnaFile::GetHistogramFailed(Hal::ECutUpdate update, Int_t collection, Int_t no) const {
    Package* pack = GetCutCollection(update, collection);
    TList* lista  = (TList*) pack->GetObjectByName("CutMonitorList");
    if (lista == NULL) return NULL;
    Package* cutmon = (Package*) lista->At(no);
    // HalPackage *cutmon = (HalPackage*)pack->GetObject(no);
    return (TH1*) cutmon->GetObjectByName("Failed");
  }

  Package* AnaFile::GetPackage(TString packname) const {
    for (int i = 0; i < fMainPackageArray->GetEntriesFast(); i++) {
      TString name = ((TKey*) fPhysKeys->At(i))->GetName();
      TObject* obj = fMainPackageArray->UncheckedAt(i);
      if (obj->InheritsFrom("Hal::Package") && name == packname) { return (Package*) obj; }
    }
    Cout::FailSucced(Form("Package %s not found !", packname.Data()), "WARNING", kRed);
    return NULL;
  }

  Bool_t AnaFile::SwitchPackage(TString name) {
    Bool_t not_switched = kTRUE;
    for (int i = 0; i < fPhysKeys->GetEntries(); i++) {
      TKey* key        = (TKey*) fPhysKeys->At(i);
      TString key_name = key->GetName();
      if (key_name.EqualTo(name)) {
        fCurrentPackID = i;
        not_switched   = kFALSE;
      }
    }
    if (not_switched) {
      Cout::PrintInfo(Form("%s not found in HalPhysics", name.Data()), Hal::EInfo::kLowWarning);
      return kFALSE;
    }
    return kTRUE;
  }

  Bool_t AnaFile::SwitchPackage(Int_t no) {
    if (no < 0) return kFALSE;
    if (no >= fMaxPackID) return kFALSE;
    fCurrentPackID = no;
    return kTRUE;
  }

  Bool_t AnaFile::FindCutContainer(Int_t pack_no) {
    Package* mainpack = static_cast<Package*>(fMainPackageArray->UncheckedAt(pack_no));
    for (int i = 0; i < mainpack->GetEntries(); i++) {
      TObject* obj      = mainpack->GetObject(i);
      TString classname = obj->ClassName();
      if (classname == "HalPackage") {  // possible canditate
        Package* pack = (Package*) mainpack->GetObject(i);
        TString pack_class_name(pack->GetName(), strlen(pack->GetName()));
        if (pack_class_name.EqualTo("HalCutContainer")) {
          fCutContainerArray->AddAtAndExpand(pack, pack_no);
          return kTRUE;
        }
      }
    }
    return kFALSE;
  }

  Package* AnaFile::GetMetaData() const { return fMetaDataPack; }

  AnaFile::~AnaFile() {
    delete fMetaDataPack;
    delete fMainPackageArray;
    delete fCutContainerArray;
    fFile->Close();
    delete fFile;
  }

  Int_t AnaFile::GetMainObjectsNo() const { return GetMainPackage()->GetEntries(); }

  void AnaFile::GetPackageList(TString filename) {
    TFile* file      = new TFile(filename);
    TDirectory* tdir = (TDirectory*) file->Get("HalPhysics");
    TList* list      = tdir->GetListOfKeys();
    for (int i = 0; i < list->GetEntries(); i++) {
      TString name    = ((TKey*) (list->At(i)))->GetName();
      TObject* object = tdir->Get(name);
      Cout::Text("List of keys ", "L");
      if (object->InheritsFrom("Hal::Package")) {
        Cout::Text(object->GetName(), "L");
      } else {
        Cout::PrintInfo(Form("%s found in HalPhysics but don't inherit from HalPackage", name.Data()), Hal::EInfo::kLowWarning);
      }
    }
    file->Close();
  }

  ULong64_t AnaFile::GetPassedNo(Hal::ECutUpdate update, Int_t collection, Option_t* opt) const {
    Package* pack  = GetCutCollection(update, collection);
    TString option = opt;
    if (option == "fast")
      option = "PassedFast";
    else
      option = "PassedSlow";
    ParameterULong64* entries = (ParameterULong64*) pack->GetObjectByName(option);
    return entries->GetValue();
  }

  ULong64_t AnaFile::GetFailedNo(Hal::ECutUpdate update, Int_t collection, Option_t* opt) const {
    Package* pack  = GetCutCollection(update, collection);
    TString option = opt;
    if (option == "fast")
      option = "FailedFast";
    else
      option = "FailedSlow";
    ParameterULong64* entries = (ParameterULong64*) pack->GetObjectByName(option);
    return entries->GetValue();
  }

  Int_t AnaFile::GetCutsNo(Hal::ECutUpdate update, Int_t collection, TString opt) const {
    Package* pack  = GetCutCollection(update, collection);
    TString option = opt;
    if (option == "fast") {
      TList* list = (TList*) pack->GetObjectByName("FastCutList");
      if (list) return list->GetEntries();
    } else if (option == "slow") {
      TList* list = (TList*) pack->GetObjectByName("CutList");
      if (list) return list->GetEntries();
    } else {
      Int_t total  = 0;
      TList* listf = (TList*) pack->GetObjectByName("FastCutList");
      TList* list  = (TList*) pack->GetObjectByName("CutList");
      if (listf) total += listf->GetEntries();
      if (list) total += list->GetEntries();
      return total;
    }
    return 0;
  }

  Int_t AnaFile::GetCuMonitorsNo(Hal::ECutUpdate update, Int_t collection, TString opt) const {
    Package* pack  = GetCutCollection(update, collection);
    TString option = opt;
    Int_t d1, d2, d3, d4;
    d1 = d2 = d3 = d4 = 0;
    TList* list       = (TList*) pack->GetObjectByName("CutMonitorList");
    if (list == NULL) return 0;
    for (int iCut = 0; iCut < list->GetEntries(); iCut++) {
      Package* mon = (Package*) list->At(iCut);
      if (((TString) mon->GetName()) == "HalCutMonitorX") d1++;
      if (((TString) mon->GetName()) == "HalCutMonitorXY") d2++;
      if (((TString) mon->GetName()) == "HalCutMonitorXYZ") d3++;
      d4++;
    }
    if (option == "1d") return d1;
    if (option == "2d") return d2;
    if (option == "3d") return d3;
    return d4;
  }

  void AnaFile::PrintCuts(Hal::ECutUpdate update, Int_t col) const {
    TList* list   = NULL;
    TString label = "";
    list =
      ((TList*) GetCutContainer()->GetObjectByName(Form("Hal%sCutCollectionList", Hal::Std::UpdateEnumToString(update).Data())));
    label = Form("%s Cuts", Hal::Std::UpdateEnumToString(update).Data());
    if (list == NULL) return;
    if (col == -1) {
      for (int i = 0; i < list->GetEntries(); i++) {
        Cout::Text(Form("%s Cut col: %i", label.Data(), i), "M", kCyan);
        PrintCutsInPackage((Package*) list->At(i));
      }
    } else {
      Cout::Text(Form("%s Cut col: %i", label.Data(), col), "M", kCyan);
      PrintCutsInPackage((Package*) list->At(col));
    }
  }

  void AnaFile::PrintCutsInPackage(Package* pack) const {
    TList* fast_cuts             = (TList*) pack->GetObjectByName("FastCutList");
    TList* slow_cuts             = (TList*) pack->GetObjectByName("CutList");
    ParameterULong64* passedFast = (ParameterULong64*) pack->GetObjectByName("PassedFast");
    ParameterULong64* passedSlow = (ParameterULong64*) pack->GetObjectByName("PassedSlow");
    ParameterULong64* failedFast = (ParameterULong64*) pack->GetObjectByName("FailedFast");
    ParameterULong64* failedSlow = (ParameterULong64*) pack->GetObjectByName("FailedSlow");
    TString PassedFast           = Hal::Std::RoundToString(passedFast->GetValue());
    TString PassedSlow           = Hal::Std::RoundToString(passedSlow->GetValue());
    TString FailedFast           = Hal::Std::RoundToString(failedFast->GetValue());
    TString FailedSlow           = Hal::Std::RoundToString(failedSlow->GetValue());
    Cout::Database(3, "Type", "Failed", "Passed");
    Cout::Database(3, "Fast", PassedFast.Data(), FailedFast.Data());
    Cout::Database(3, "Slow", PassedFast.Data(), FailedSlow.Data());
    Cout::Text("Fast Cuts", "M", kCyan);
    Cout::Database(5, "Name", "Passed %", "Param", "Min", "Max");
    for (int i = 0; i < fast_cuts->GetEntries(); i++)
      PrintCut((Package*) fast_cuts->At(i));
    Cout::Text("Slow Cuts", "M", kCyan);
    for (int i = 0; i < slow_cuts->GetEntries(); i++)
      PrintCut((Package*) slow_cuts->At(i));
  }

  void AnaFile::PrintCut(Package* cut) const {
    TString name    = cut->GetName();
    Double_t passed = (Double_t)(((ParameterULong64*) cut->GetObjectByName("Passed")))->GetValue();
    Double_t failed = (Double_t)(((ParameterULong64*) cut->GetObjectByName("Failed")))->GetValue();
    Int_t cut_size  = (((ParameterInt*) cut->GetObjectByName("CutSize")))->GetValue();
    passed          = passed / (passed + failed) * 100.0;
    if (cut_size == 0) {
      TString unit_name = ((ParameterString*) cut->GetObjectByName("UnitName_0i"))->GetValue();
      TString min       = Hal::Std::RoundToString(((ParameterDouble*) cut->GetObjectByName("MinCut_0"))->GetValue(), 3);
      TString max       = Hal::Std::RoundToString(((ParameterDouble*) cut->GetObjectByName("MaxCut_0"))->GetValue(), 3);
      Cout::Database(5, name.Data(), Form("%4.2f", passed), unit_name.Data(), min.Data(), max.Data());
      return;
    }
    TString min, max, unit;

    for (int i = 0; i < cut_size; i++) {
      min  = Form("MinCut_%i", i);
      max  = Form("MaxCut_%i", i);
      unit = Form("UnitName_%i", i);
      unit = ((ParameterString*) cut->GetObjectByName(unit))->GetValue();
      min  = Hal::Std::RoundToString(((ParameterDouble*) cut->GetObjectByName(min))->GetValue(), 3);
      max  = Hal::Std::RoundToString(((ParameterDouble*) cut->GetObjectByName(max))->GetValue(), 3);
      if (i == 0) {
        Cout::Database(5, name.Data(), Form("%4.2f", passed), unit.Data(), min.Data(), max.Data());
      } else {
        Cout::Database(5, "", "", unit.Data(), min.Data(), max.Data());
      }
    }
  }

  Package* AnaFile::GetCutPackage(Hal::ECutUpdate update, Int_t collection, Int_t cutNo, Option_t* opt) const {
    TList* list   = NULL;
    TString label = "";
    list =
      ((TList*) GetCutContainer()->GetObjectByName(Form("Hal%sCutCollectionList", Hal::Std::UpdateEnumToString(update).Data())));
    label = Form("%s Cuts", Hal::Std::UpdateEnumToString(update).Data());
    if (list == NULL) return NULL;
    Package* cutCollection = (Package*) list->At(collection);
    if (cutCollection == NULL) return NULL;
    TString option   = opt;
    TList* list_fast = (TList*) cutCollection->GetObjectByName("FastCutList");
    TList* list_slow = (TList*) cutCollection->GetObjectByName("CutList");
    if (option == "all") {
      if (cutNo > list_fast->GetEntries()) {
        return (Package*) list_slow->At(cutNo - list_fast->GetEntries());
      } else {
        return (Package*) list_fast->At(cutNo);
      }
    } else if (option == "slow") {
      return (Package*) list_slow->At(cutNo);
    } else {  // fast cuts
      return (Package*) list_fast->At(cutNo);
    }
  }

  Package* AnaFile::GetMainPackage() const { return static_cast<Package*>(fMainPackageArray->UncheckedAt(fCurrentPackID)); }

  Package* AnaFile::GetCutContainer() const { return static_cast<Package*>(fCutContainerArray->UncheckedAt(fCurrentPackID)); }
}  // namespace Hal
