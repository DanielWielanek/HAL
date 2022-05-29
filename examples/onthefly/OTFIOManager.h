/*
 * OTFIOManager.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_OTFIOMANAGER_H_
#define HAL_EXAMPLES_ONTHEFLY_OTFIOMANAGER_H_

#include "IOManager.h"
#include <vector>

#include <RtypesCore.h>
#include <TString.h>

class TTree;
class TBranch;
namespace HalOTF {
  class IOManager : public Hal::IOManager {
    TString fInFileName;
    TString fOutFileName;
    TString fOutTreeName;
    Int_t fEntries;
    TFile* fInFile;
    TFile* fOutFile;
    TTree* fOutTree;
    std::vector<TString> fBranchList;
    std::vector<TBranch*> fInBranches;
    std::vector<TBranch*> fOutBranches;
    std::vector<std::pair<TString, TObject*>> fOutBranchesVirtual;

  public:
    IOManager(TString name = "root_virtual.root", Int_t entries = 1);
    void SetOutput(TString name) { fOutFileName = name; }
    void SetOutTreeName(TString name) { fOutTreeName = name; }
    Int_t GetEntries() const;
    Int_t GetEntry(Int_t i);
    Bool_t Init();
    TObject* GetObject(const char* BrName);
    TFile* GetInFile();
    void AddFriend(TString /*name*/) {};
    void UpdateBranches();
    void Register(const char* name, const char* folderName, TNamed* obj, Bool_t toFile);
    void Register(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile);
    void SetInChain(TChain* tempChain, Int_t ident = -1);
    Int_t CheckBranch(const char* BrName);
    TList* GetBranchNameList();
    virtual ~IOManager();
    ClassDef(IOManager, 1)
  };
}  // namespace HalOTF

#endif /* HAL_EXAMPLES_ONTHEFLY_OTFIOMANAGER_H_ */
