/*
 * ChainBuilder.cxx
 *
 *  Created on: 19 cze 2024
 *      Author: daniel
 */

#include "InputDataInfo.h"

#include <TChain.h>
#include <TCollection.h>
#include <TDirectoryFile.h>
#include <TFile.h>
#include <TKey.h>
#include <TList.h>
#include <TNamed.h>
#include <TTree.h>
#include <initializer_list>
#include <iostream>


#include "Cout.h"
#include "Std.h"
#include "StdString.h"
#include "XMLNode.h"

namespace Hal {


  InputDataInfo::InputDataInfo(TString file) : fListName(file) {
    if (file.EndsWith(".list")) {
      auto vec = Hal::Std::GetLinesFromFile(file, kTRUE);
      fFileNames.push_back(vec);
    } else if (file.EndsWith(".xml")) {
      XMLFile xmlFile(file);
      auto root  = xmlFile.GetRootNode();
      auto files = root->GetChild("files");
      for (int i = 0; i < files->GetNChildren(); i++) {
        auto list = files->GetChild(i);
        fFileNames.push_back(std::vector<TString>());
        for (int j = 0; j < list->GetNChildren(); i++) {
          fFileNames[i].push_back(list->GetChild(j)->GetValue());
        }
      }
    } else {
      fFileNames.resize(1);
      fFileNames[0].push_back(file);
    }
  }

  InputDataInfo::InputDataInfo(std::initializer_list<TString> fileLists) {
    fListName = Form("initializer_list");
    for (auto file : fileLists) {
      fFileNames.push_back(Hal::Std::GetLinesFromFile(file, kTRUE));
    }
  }

  InputDataInfo::InputDataInfo(std::vector<std::vector<TString>> files) {
    fListName  = Form("2d vector");
    fFileNames = files;
  }

  TString InputDataInfo::GetSafeFile(Int_t level, Int_t entry) const {
    if (level + 1 < fFileNames.size()) {
      if (fFileNames[level + 1].size() > entry) return fFileNames[level + 1][entry];
    }
    return "";
  }

  std::vector<TString> InputDataInfo::GetSafeFiles(Int_t level) const {
    if (level + 1 < fFileNames.size()) { return fFileNames[level + 1]; }
    return std::vector<TString>();
  }

  void InputDataInfo::AddFile(TString file) { AddFriend(file, -1); }

  void InputDataInfo::AddFriend(TString file, Int_t level) {
    if (fFileNames.size() < level + 2) { fFileNames.resize(level + 2); }
    fFileNames[level + 1].push_back(file);
  }

  Int_t InputDataInfo::GetNFiles() const {
    if (fFileNames.size() == 0) return -1;
    return fFileNames[0].size();
  }

  Int_t InputDataInfo::GetFriendsLevel() const { return fFileNames.size() - 1; }

  TString InputRootDataInfo::GetChainName(TString file) const {
    TFile* ftemp = new TFile(file);
    auto list    = (TList*) ftemp->GetListOfKeys();
    for (int i = 0; i < list->GetEntries(); i++) {
      TString keyName = ((TKey*) list->At(i))->GetName();
      auto tree       = dynamic_cast<TTree*>(ftemp->Get(keyName));
      if (tree) {
        ftemp->Close();
        delete ftemp;
        return keyName;
      }
    }
    ftemp->Close();
    delete ftemp;
    return "";
  }

  void InputRootDataInfo::GuessTrees() {
    for (auto& list : fFileNames) {
      if (Hal::Std::FileExists(list[0]))
        fTreeNames.push_back(GetChainName(list[0]));
      else {
        Hal::Cout::PrintInfo(Form("Can't find tree in %s", list[0]), EInfo::kDebugInfo);
        fTreeNames.push_back(" ");
      }
    }
  }

  InputRootDataInfo::InputRootDataInfo(TString file, TString treename) : InputDataInfo(file) {
    if (treename.Length() != 0) {
      fTreeNames.push_back(treename);
      return;
    }
    if (file.EndsWith(".xml")) {
      XMLFile xmlFile(file);
      auto root      = xmlFile.GetRootNode();
      auto files     = root->GetChild("files");
      auto treenames = root->GetChild("treenames");
      if (treenames) {
        for (int i = 0; i < treenames->GetNChildren(); i++) {
          fTreeNames.push_back(treenames->GetChild(i)->GetValue());
        }
      }
    }
  }

  InputRootDataInfo::InputRootDataInfo(std::initializer_list<TString> fileLists, std::initializer_list<TString> treeNames) :
    InputDataInfo(fileLists) {
    if (treeNames.size() != 0) { fTreeNames = Hal::Std::GetVector(treeNames); }
  }

  InputRootDataInfo::InputRootDataInfo(std::vector<std::vector<TString>> files) : InputDataInfo(files) {}

  TChain* InputRootDataInfo::GetChain() {
    if (fChain) return fChain;
    if (fTreeNames.size() == 0) { GuessTrees(); }
    fChain = new TChain(fTreeNames[0]);
    for (auto name : fFileNames[0])
      fChain->AddFile(name);
    int entries = fChain->GetEntries();
    for (unsigned int i = 1; i < fFileNames.size(); i++) {
      TChain* minichain = new TChain(fTreeNames[i]);
      for (auto friendName : fFileNames[i])
        minichain->AddFile(friendName);
      fChain->AddFriend(minichain);
      if (minichain->GetEntries() != entries) {
        Hal::Cout::PrintInfo("Warning different number of entries detected in ChainBuilder", Hal::EInfo::kError);
      }
    }
    return fChain;
  }

  void InputDataInfo::Print(Option_t* option) const {
    std::vector<TString> header;
    header.push_back("No");
    header.push_back("Main file");
    for (int i = 1; i < fFileNames.size(); i++) {
      header.push_back(Form("Friend_%i", i - 1));
    }
    Hal::Cout::SetLineLenght(26 * header.size());
    Hal::Cout::Database(header);
    for (unsigned int pos = 0; pos < fFileNames[0].size(); pos++) {
      std::vector<TString> str;
      str.push_back(Form("%i", pos));
      for (unsigned int level = 0; level < fFileNames.size(); level++) {
        if (fFileNames[level].size() > pos) {
          TString path = fFileNames[level][pos];
          if (path.Length() > 25) {
            int crop = path.Length() - 25;
            path     = TString(path(crop, path.Length()));
            path     = Form("..%s", path.Data());
          }
          str.push_back(path);
        } else {
          str.push_back(" ");
        }
      }
      Hal::Cout::Database(str);
    }
    Hal::Cout::SetLineLenght(100);
  }

  void InputRootDataInfo::Print(Option_t* option) const {
    std::vector<TString> header;
    header.push_back("No");
    header.push_back("Main file");
    for (unsigned int i = 1; i < fFileNames.size(); i++) {
      header.push_back(Form("Friend_%i", i - 1));
    }
    Hal::Cout::SetLineLenght(26 * header.size());
    Hal::Cout::Database(header);
    std::vector<TString> line;
    line.push_back("Tree names:");
    for (auto tree : fTreeNames) {
      line.push_back(tree);
    }
    Hal::Cout::Database(line);
    for (unsigned int pos = 0; pos < fFileNames[0].size(); pos++) {
      std::vector<TString> str;
      str.push_back(Form("%i", pos));
      for (unsigned int level = 0; level < fFileNames.size(); level++) {
        if (fFileNames[level].size() > pos) {
          TString path = fFileNames[level][pos];
          if (path.Length() > 25) {
            int crop = path.Length() - 25;
            path     = TString(path(crop, path.Length()));
            path     = Form("..%s", path.Data());
          }
          str.push_back(path);
        } else {
          str.push_back(" ");
        }
      }
      Hal::Cout::Database(str);
    }
    Hal::Cout::SetLineLenght(100);
  }

} /* namespace Hal */
