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
    switch (flag) {
      case EBranchFlag::kIn: {
        fInBranches.push_back(std::pair<TString, TObject*>(name, object));
      } break;
      case EBranchFlag::kOut: {
        fOutBranches.push_back(std::pair<TString, TObject*>(name, object));
      } break;
      case EBranchFlag::kVirtual: {
        fOutVirtual.push_back(std::pair<TString, TObject*>(name, object));
      } break;
      default: {
        Cout::PrintInfo(Form("Bad flag in %s-> AddBranch(%s)", ClassName(), name.Data()), EInfo::kWarning);
        return;
      } break;
    }
    fList->AddLast(new TObjString(name));
  }

  std::pair<TString, TObject*> IOManager::FindBranch(TString name, EBranchFlag flag) const {
    switch (flag) {
      case EBranchFlag::kIn: {
        for (auto branch : fInBranches) {
          if (branch.first.EqualTo(name)) return branch;
        }
      } break;
      case EBranchFlag::kOut: {
        for (auto branch : fOutBranches) {
          if (branch.first.EqualTo(name)) return branch;
        }
      } break;
      case EBranchFlag::kVirtual: {
        for (auto branch : fOutVirtual) {
          if (branch.first.EqualTo(name)) return branch;
        }
      } break;
      default: {
        for (auto branch : fInBranches) {
          if (branch.first.EqualTo(name)) return branch;
        }
        for (auto branch : fOutBranches) {
          if (branch.first.EqualTo(name)) return branch;
        }
        for (auto branch : fOutVirtual) {
          if (branch.first.EqualTo(name)) return branch;
        }
      } break;
    }
    return std::pair<TString, TObject*>(name, nullptr);
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

  IOManager::EBranchStatus IOManager::GetBranchStatus(const char* BrName) {
    RefreshBranchList();
    TString name = BrName;
    for (auto branch : fInBranches) {
      if (branch.first.EqualTo(name)) return EBranchStatus::kInput;
    }
    for (auto branch : fOutVirtual) {
      if (branch.first.EqualTo(name)) return EBranchStatus::kVirtual;
    }
    for (auto branch : fOutBranches) {
      if (branch.first.EqualTo(name)) return EBranchStatus::kOutput;
    }
    return IOManager::EBranchStatus::kNull;
  }

  TObject* Hal::IOManager::GetObject(const char* BrName) {
    std::pair<TString, TObject*> branch = FindBranch(BrName, EBranchFlag::kAny);
    if (branch.second == nullptr) {
      RefreshBranchList();
      // try again if branch not found
      branch = FindBranch(BrName, EBranchFlag::kAny);
    }
    return branch.second;
  }

  void IOManager::PrintInfo() {
    Cout::PrintInfo(Form("IO Manager %s", ClassName()), EInfo::kInfo);
    if (GetInFile()) { Cout::PrintInfo(Form("Input file %s", GetInFile()->GetName()), EInfo::kInfo); }
    UpdateBranches();
    RefreshBranchList();
    Cout::Database(2, "Branch Name", "Type");
    for (auto branch : fInBranches) {
      Cout::Database(2, branch.first.Data(), "INPUT");
    }

    for (auto branch : fOutBranches) {
      Cout::Database(2, branch.first.Data(), "OUTPUT");
    }
    for (auto branch : fOutVirtual) {
      Cout::Database(2, branch.first.Data(), "VIRTUAL");
    }
  }

  IOManager::~IOManager() {
    if (fList) delete fList;
  }

}  // namespace Hal


/* namespace Hal */
