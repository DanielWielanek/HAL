/*
 * RootIoManager.h
 *
 *  Created on: 6 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_IO_ROOTIOMANAGER_H_
#define HAL_FEATURES_IO_ROOTIOMANAGER_H_

#include "IOManager.h"
#include <vector>

#include <RtypesCore.h>
#include <TList.h>
#include <TString.h>

class TTree;
class TBranch;
/**
 * IO manager for ROOT files processed by AnalysisManager
 */
namespace Hal {

  class RootIOManager : public IOManager {
    Int_t fEntries = {0};
    TString fOutFileName;
    TString fOutTreeName;
    TFile* fOutFile  = {nullptr};
    TTree* fOutTree  = {nullptr};
    TChain* fInChain = {nullptr};
    std::vector<TObject**> fObjects;  // just to call delete?

  protected:
    /**
     * add object to branches
     */
    void PushTObject(TObject** obj);
    virtual void RegisterInternal(TString name, TString folderName, TObject* obj, Bool_t toFile);
    TChain* GetInChain() { return fInChain; }
    virtual Bool_t InitInternal();

  public:
    /**
     *
     * @param name file with data do not use this constructor
     */
    RootIOManager(TString name);
    /**
     * main constructor
     * @param info
     */
    RootIOManager(InputDataInfo* info);
    /**
     * set name of the output file
     * @param name
     */
    void SetOutput(TString name) { fOutFileName = name; }
    /**
     * set name of the output tree
     */
    void SetOutTreeName(TString name) { fOutTreeName = name; }
    /**
     *
     * @return number of entries in file
     */
    Int_t GetEntries() const;
    /**
     * get entry from input root file
     * @param i
     * @param flag - flag
     * @return
     */
    Int_t GetEntry(Int_t i, Int_t flag = 1);
    virtual void UpdateBranches();
    void LockUnusedBranches();
    void SetInChain(TChain* tempChain, Int_t ident = -1);
    void FillTree();
    /**
     * return list of branches in chain (for some reason root doesn't check the friend chains
     * @param chain
     * @param friends if true return friend's branches
     * @return
     */
    static std::vector<TString> GetListOfBranches(TChain* chain, Bool_t friends);
    virtual void CloseManager();
    void CdToOutput();
    virtual ~RootIOManager();
    ClassDef(RootIOManager, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_IO_ROOTIOMANAGER_H_ */
