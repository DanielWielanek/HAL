/*
 * IOManager.cxx
 *
 *  Created on: 10 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "IOManager.h"

#include "Cout.h"
#include "InputDataInfo.h"
#include "Package.h"
#include "PackageTable.h"
#include "Parameter.h"
#include "Pointer.h"
#include "Std.h"
#include "XMLNode.h"

#include <fstream>
#include <utility>
#include <vector>

#include <RtypesCore.h>
#include <TChain.h>
#include <TFile.h>
#include <TList.h>
#include <TNamed.h>
#include <TObjString.h>


namespace Hal {
  BranchInfo::BranchInfo(TString name, const AbstractDoublePointer& pointer, EFlag used) :
    fBrName(name), fPointer(pointer.MakeCopy()), fFlag(used) {}

  BranchInfo::~BranchInfo() {
    if (fPointer) delete fPointer;
  }

  void IOManager::AddBranch(TString name, AbstractDoublePointer& object, BranchInfo::EFlag flag) {
    for (auto str : fBranches) {
      if (str.GetBranchName().EqualTo(name)) return;
    }
    fBranches.push_back(BranchInfo(name, object, flag));
    fBranchNameList.push_back(name);
  }

  Hal::BranchInfo IOManager::FindBranch(TString name) const {
    for (auto branch : fBranches) {
      if (branch.GetBranchName().EqualTo(name)) return branch;
    }
    return BranchInfo(name);
  }

  template<class T>
  Hal::DoublePointer<T> IOManager::Register(T* obj, TString branchName, Bool_t toFile) {
    BranchInfo::EFlag flag = BranchInfo::EFlag::kVirtual;
    if (toFile) flag = BranchInfo::EFlag::kOut;
    Hal::DoublePointer<T> pointer;
    pointer.Initialize(obj);

    if (IsRootOutput() && toFile) { GetOutChain()->SetBranchAddress(branchName, pointer.GetDoublePointer()); }
    RegisterInternal(branchName, pointer.GetDoublePointer(), toFile);
    AddBranch(branchName, pointer, flag);
    delete obj;
    return pointer;
  }

  BranchInfo::EFlag IOManager::GetBranchStatus(const char* BrName) {
    UpdateBranches();
    TString name = BrName;
    for (auto branch : fBranches) {
      if (branch.GetBranchName().EqualTo(name)) return branch.GetFlag();
    }
    return BranchInfo::EFlag::kNull;
  }
  template<class T>
  DoublePointer<T> IOManager::GetObject(TString branchName) {
    auto& branch = FindBranch(branchName);
    if (branch.GetFlag() == Hal::BranchInfo::EFlag::kNull) UpdateBranches();
    branch = FindBranch(branchName);
    if (branch.GetFlag() == Hal::BranchInfo::EFlag::kNull) { return DoublePointer<T>(nullptr); }
    auto pointer = branch.GetDoublePointer();
    if (!pointer->IsTObject()) {                               // good it's not tobject
      auto copied = dynamic_cast<DoublePointer<T>*>(pointer);  // must be
      if (!copied) { return DoublePointer<T>(nullptr); }
      return *copied;
    }
    // it's t object, it;s problem becase we store rather DoubleObjectPointers, so we have to make specialized interface
    auto copied = dynamic_cast<ObjectDoublePointer*>(pointer);
    if (!copied) {
      // ok its not template<TObject>, maybe it specialized template? e.g. tempate<TClonesArray> ?
      auto realTobject = dynamic_cast<DoublePointer<T>*>(pointer);
      if (realTobject) return realTobject;
      return DoublePointer<T>(nullptr);
    }
    // ok we know it's some tobject class, lets make a copy
    auto specializedcopy = copied->MakeSpecializedCopy<T>();
    return specializedcopy;
  }

  void IOManager::ActivateBranch(TString brName) {
    for (auto& branch : fBranches) {
      if (branch.GetBranchName().EqualTo(brName)) {
        if (branch.GetFlag() == BranchInfo::EFlag::kInPassive) {
          branch.SetFlag(BranchInfo::EFlag::kInActive);
          return;
        }
      }
    }
    // refresh and try again
    UpdateBranches();
    for (auto& branch : fBranches) {
      if (branch.GetBranchName().EqualTo(brName)) {
        if (branch.GetFlag() == BranchInfo::EFlag::kInPassive) {
          branch.SetFlag(BranchInfo::EFlag::kInActive);
          return;
        }
      }
    }
  }

  void IOManager::PrintInfo() {
    Cout::PrintInfo(Form("IO Manager %s", ClassName()), EInfo::kInfo);
    TString inFile = "";
    Cout::PrintInfo(Form("Input file %s", fDataInfo->GetSourceName().Data()), EInfo::kInfo);
    UpdateBranches();
    Cout::Database({"Branch Name", "Type"});
    TString types[]           = {"Input (Active)", "Input (Passive)", "Output", "Virtual"};
    BranchInfo::EFlag flags[] = {
      BranchInfo::EFlag::kInActive, BranchInfo::EFlag::kInPassive, BranchInfo::EFlag::kOut, BranchInfo::EFlag::kVirtual};
    for (int i = 0; i < 4; i++) {
      for (auto branch : fBranches) {
        if (branch.GetFlag() == flags[i]) Cout::Database({branch.GetBranchName(), types[i]});
      }
    }
  }

  IOManager::~IOManager() {
    if (fDataInfo) delete fDataInfo;
  }

  TList* IOManager::GetBranchesList() const {
    TList* list = new TList();
    list->SetName(Form("Branch List"));

    auto getStr = [](BranchInfo::EFlag flag) {
      TString flagStr = "unknown";
      switch (flag) {
        case BranchInfo::EFlag::kInActive: {
          flagStr = "INPUT (ACTIVE)";
        } break;
        case BranchInfo::EFlag::kInPassive: {
          flagStr = "INPUT (PASSIVE)";
        } break;
        case BranchInfo::EFlag::kOut: {
          flagStr = "OUTPUT";
        } break;
        case BranchInfo::EFlag::kVirtual: {
          flagStr = "VIRTUAL";
        } break;
        case BranchInfo::EFlag::kNull: {
          flagStr = "NULL";
        } break;
        default: break;
      };
      return flagStr;
    };
    for (auto branch : fBranches) {
      TString flagStr   = getStr(branch.GetFlag());
      TString name      = branch.GetBranchName();
      TString className = branch.GetStoredClassName();
      list->Add(new ParameterString(Form("Branch: %s", name.Data()), Form("%s [%s]", className.Data(), flagStr.Data())));
    }
    list->Add(new Hal::ParameterString("IOManager", this->ClassName()));
    return list;
  }
  Bool_t IOManager::Init() {
    if (!fDataInfo) {
      Hal::Cout::PrintInfo("IO manager cannot InputDataInfo !", EInfo::kCriticalError);
      return kFALSE;
    }
    if (fDataInfo->GetSafeFiles(-1).size() == 0) {
      Hal::Cout::PrintInfo("IO manager cannot find any files !", EInfo::kCriticalError);
      return kFALSE;
    }
    return InitInternal();
  }

  IOManager::IOManager(InputDataInfo* info) { fDataInfo = info; }

  TString IOManager::GetSourceName() const { return fDataInfo->GetSourceName(); };

  Int_t IOManager::GetNFiles() const { return fDataInfo->GetNFiles(); }

  std::vector<TString> IOManager::GetFilesNames(Int_t entry) const { return fDataInfo->GetSafeFiles(entry); }

  Int_t IOManager::GetFriendsLevel() const { return fDataInfo->GetFriendsLevel(); }

  void IOManager::AddFile(TString name) { fDataInfo->AddFile(name); }

  void IOManager::AddFriend(TString friendName, Int_t level) { fDataInfo->AddFriend(friendName, level); }

  TString IOManager::GetFirstDataFileName() const { return fDataInfo->GetSafeFile(-1, 0); }

  TString IOManager::GetFirstFriendFileName(Int_t level) const { return fDataInfo->GetSafeFile(level, 0); }

  std::vector<TString> IOManager::GetFileNameList(Int_t level) const { return fDataInfo->GetSafeFiles(level); }


}  // namespace Hal


/* namespace Hal */
