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

#include <RtypesCore.h>
#include <TFile.h>
#include <TList.h>
#include <TNamed.h>
#include <TObjString.h>

namespace Hal {

  void IOManager::AddBranch(TString name, TObject* object, EBranchFlag flag) {
    for (auto str : fBranches) {
      if (str.fName.EqualTo(name)) return;
    }
    fBranches.push_back(BranchInfo(name, object, flag));
    fBranchNameList.push_back(name);
  }

  Hal::IOManager::BranchInfo IOManager::FindBranch(TString name) const {
    for (auto branch : fBranches) {
      if (branch.fName.EqualTo(name)) return branch;
    }
    BranchInfo br(name, nullptr, EBranchFlag::kNull);
    br.fName = name;
    return br;
  }

  void IOManager::Register(const char* name, const char* folderName, TNamed* obj, Bool_t toFile) {
    if (toFile) {
      AddBranch(name, obj, EBranchFlag::kOut);
    } else {
      AddBranch(name, obj, EBranchFlag::kVirtual);
    }
    RegisterInternal(name, folderName, obj, toFile);
  }

  void IOManager::Register(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile) {
    if (toFile) {
      AddBranch(name, obj, EBranchFlag::kOut);
    } else {
      AddBranch(name, obj, EBranchFlag::kVirtual);
    }
    RegisterInternal(name, Foldername, obj, toFile);
  }

  IOManager::EBranchFlag IOManager::GetBranchStatus(const char* BrName) {
    RefreshBranchList();
    TString name = BrName;
    for (auto branch : fBranches) {
      if (branch.fName.EqualTo(name)) return branch.fFlag;
    }
    return IOManager::EBranchFlag::kNull;
  }

  TObject* Hal::IOManager::GetObject(const char* BrName) {
    TString name = BrName;
    for (auto& branch : fBranches) {
      if (branch.fName.EqualTo(name)) {
        if (branch.fFlag == EBranchFlag::kInPassive) { branch.fFlag = EBranchFlag::kInActive; }
        return branch;
      }
    }
    // refresh and try again
    RefreshBranchList();
    for (auto& branch : fBranches) {
      if (branch.fName.EqualTo(name)) {
        if (branch.fFlag == EBranchFlag::kInPassive) { branch.fFlag = EBranchFlag::kInActive; }
        return branch;
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
    TString types[]     = {"Input (Active)", "Input (Passive)", "Output", "Virtual"};
    EBranchFlag flags[] = {EBranchFlag::kInActive, EBranchFlag::kInPassive, EBranchFlag::kOut, EBranchFlag::kVirtual};
    for (int i = 0; i < 4; i++) {
      for (auto branch : fBranches) {
        if (branch.fFlag == flags[i]) Cout::Database({branch.fName, types[i]});
      }
    }
  }

  IOManager::~IOManager() {}

}  // namespace Hal


/* namespace Hal */
