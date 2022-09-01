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
    Int_t fEntries;
    std::vector<TString> fInFileName;
    std::vector<std::vector<TString>> fFriendName;
    TString fOutFileName;
    TString fOutTreeName;
    TFile* fOutFile;
    TTree* fOutTree;
    std::vector<TFile*> fInFile;
    std::vector<TChain*> fInChain;
    std::vector<TObject**> fObjects;

  protected:
    TString GetChain(TFile* file) const;
    virtual void RegisterInternal(const char* name, const char* folderName, TNamed* obj, Bool_t toFile);
    virtual void RegisterInternal(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile);

  public:
    /**
     *
     * @param name file with data
     */
    RootIOManager(TString name);
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
     * @return
     */
    virtual Int_t GetEntry(Int_t i);
    Bool_t Init();
    /**
     * return pointer to the inputfile
     * @return
     */
    TFile* GetInFile();
    void AddFile(TString name) { fInFileName.push_back(name); };
    void AddFriend(TString friendName, Int_t level);
    void UpdateBranches();
    void SetInChain(TChain* tempChain, Int_t ident = -1);
    void FillTree();
    virtual void CloseManager();
    virtual ~RootIOManager();
    ClassDef(RootIOManager, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_IO_ROOTIOMANAGER_H_ */
