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

  protected:
    void RegisterInternal(const char* name, const char* folderName, TNamed* obj, Bool_t toFile);
    void RegisterInternal(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile);
    Bool_t InitInternal();

  public:
    IOManager(TString name = "root_virtual.root", Int_t entries = 1);
    void SetOutput(TString name) { fOutFileName = name; }
    void SetOutTreeName(TString name) { fOutTreeName = name; }
    Int_t GetEntries() const;
    Int_t GetEntry(Int_t i, Int_t flag);
    TFile* GetInFile();
    void AddFriend(TString /*name*/) {};
    void SetInChain(TChain* tempChain, Int_t ident = -1);
    void FillTree();
    virtual void CloseManager();
    virtual ~IOManager();
    ClassDef(IOManager, 1)
  };
}  // namespace HalOTF

#endif /* HAL_EXAMPLES_ONTHEFLY_OTFIOMANAGER_H_ */
