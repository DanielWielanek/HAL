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
#include "Std.h"

#include <utility>
#include <vector>

#include "Package.h"
#include "PackageTable.h"
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
    RefreshBranchList();
    TString name = BrName;
    for (auto branch : fBranches) {
      if (branch.GetBranchName().EqualTo(name)) return branch.GetFlag();
    }
    return BranchInfo::EFlag::kNull;
  }

  TObject* Hal::IOManager::GetObject(const char* BrName) {
    TString name = BrName;
    for (auto& branch : fBranches) {
      if (branch.GetBranchName().EqualTo(name)) {
        if (branch.GetFlag() == BranchInfo::EFlag::kInPassive) { branch.SetFlag(BranchInfo::EFlag::kInActive); }
        return branch.GetPointer();
      }
    }
    // refresh and try again
    RefreshBranchList();
    for (auto& branch : fBranches) {
      if (branch.GetBranchName().EqualTo(name)) {
        if (branch.GetFlag() == BranchInfo::EFlag::kInPassive) { branch.SetFlag(BranchInfo::EFlag::kInActive); }
        return branch.GetPointer();
      }
    }
    return nullptr;
  }

  void IOManager::PrintInfo() {
    Cout::PrintInfo(Form("IO Manager %s", ClassName()), EInfo::kInfo);
    TString inFile = "";
    if (GetInFile()) {
      inFile = GetInFile()->GetName();
      Cout::PrintInfo(Form("Input file %s", GetInFile()->GetName()), EInfo::kInfo);
    } else {
      Cout::PrintInfo("Cannot get name of input file from file", EInfo::kInfo);
    }
    if (!inFile.EqualTo(GetInputFileName()) && inFile.Length() > 0) {
      TString label = Form("IOManager input file %s, TFile input name %s", GetInputFileName().Data(), inFile.Data());
      Cout::PrintInfo(label, EInfo::kInfo);
    }

    UpdateBranches();
    RefreshBranchList();
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

  IOManager::~IOManager() {}

  Package* IOManager::Report() const {
    Package* report     = new Package(this);
    PackageTable* table = new PackageTable({"Branch name", "Object class", "Status"});
    auto getStr         = [](BranchInfo::EFlag flag) {
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
      table->AddRow({name, className, flagStr});
      Cout::DebugInfo(2222);
    }
    report->AddObject(table);
    return report;
  }

}  // namespace Hal


/* namespace Hal */
