/*
 * VirtualIOManager2.h
 *
 *  Created on: 19 gru 2024
 *      Author: daniel
 */

#ifndef HAL_DATA_IO_VIRTUALIOMANAGER_H_
#define HAL_DATA_IO_VIRTUALIOMANAGER_H_

#include <RtypesCore.h>
#include <TString.h>

#include "IOManager.h"

class TTree;
namespace Hal {
  class VirtualSource;
} /* namespace Hal */

namespace Hal {

  class VirtualIOManager : public IOManager {
    TString fInFileName;
    TString fOutFileName;
    TString fOutTreeName;
    Int_t fEntries;
    TFile* fInFile;
    TFile* fOutFile;
    TTree* fOutTree;
    VirtualSource* fSource;

  protected:
    virtual void RegisterInternal(const char* name, const char* folderName, TNamed* obj, Bool_t toFile);
    virtual void RegisterInternal(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile);
    virtual Bool_t InitInternal();

  public:
    VirtualIOManager(VirtualSource* source = nullptr, Int_t events = 10000);
    void SetOutput(TString name) { fOutFileName = name; }
    void SetOutTreeName(TString name) { fOutTreeName = name; }
    Int_t GetEntries() const;
    virtual Int_t GetEntry(Int_t i, Int_t flag);
    virtual TFile* GetInFile();
    virtual void AddFriend(TString /*name*/) {};
    virtual void SetInChain(TChain* tempChain, Int_t ident = -1);
    virtual void FillTree();
    virtual void CloseManager();
    virtual ~VirtualIOManager();
    ClassDef(VirtualIOManager, 1)

      virtual void CdToOutput();
  };

} /* namespace Hal */

#endif /*  */
