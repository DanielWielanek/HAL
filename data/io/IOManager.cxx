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
#include "Std.h"
#include "XMLNode.h"

#include <fstream>
#include <utility>
#include <vector>

#include <RtypesCore.h>
#include <TFile.h>
#include <TList.h>
#include <TNamed.h>
#include <TObjString.h>


namespace Hal {

  void IOManager::AddBranch(TString name, TObject* object, BranchInfo::EFlag flag) {
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
    BranchInfo br(name, nullptr, BranchInfo::EFlag::kNull);
    br.GetBranchName() = name;
    return br;
  }

  void IOManager::Register(const char* name, const char* folderName, TNamed* obj, Bool_t toFile) {
    if (toFile) {
      AddBranch(name, obj, BranchInfo::EFlag::kOut);
    } else {
      AddBranch(name, obj, BranchInfo::EFlag::kVirtual);
    }
    RegisterInternal(name, folderName, obj, toFile);
  }

  void IOManager::Register(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile) {
    if (toFile) {
      AddBranch(name, obj, BranchInfo::EFlag::kOut);
    } else {
      AddBranch(name, obj, BranchInfo::EFlag::kVirtual);
    }
    RegisterInternal(name, Foldername, obj, toFile);
  }

  BranchInfo::EFlag IOManager::GetBranchStatus(const char* BrName) {
    UpdateBranches();
    TString name = BrName;
    for (auto branch : fBranches) {
      if (branch.GetBranchName().EqualTo(name)) return branch.GetFlag();
    }
    return BranchInfo::EFlag::kNull;
  }

  TObject* IOManager::GetObject(const char* BrName) {
    TString name = BrName;
    for (auto& branch : fBranches) {
      if (branch.GetBranchName().EqualTo(name)) {
        if (branch.GetFlag() == BranchInfo::EFlag::kInPassive) { branch.SetFlag(BranchInfo::EFlag::kInActive); }
        auto pointer = branch.GetPointer();
        if (!pointer) { Hal::Cout::PrintInfo(Form("Branch %s exists, but contains non-TObject", name.Data()), EInfo::kError); }
        return pointer;
      }
    }
    // refresh and try again
    UpdateBranches();
    for (auto& branch : fBranches) {
      if (branch.GetBranchName().EqualTo(name)) {
        if (branch.GetFlag() == BranchInfo::EFlag::kInPassive) { branch.SetFlag(BranchInfo::EFlag::kInActive); }
        auto pointer = branch.GetPointer();
        if (!pointer) { Hal::Cout::PrintInfo(Form("Branch %s exists, but contains non-TObject", name.Data()), EInfo::kError); }
        return pointer;
      }
    }
    return nullptr;
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
      TObject* obj      = branch.GetPointer();
      TString className = "unknown";
      if (obj) { className = obj->ClassName(); }
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
