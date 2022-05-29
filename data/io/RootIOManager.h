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
#include <TString.h>

class TTree;
class TBranch;
/**
 * IO manager for ROOT files processed by AnalysisManager
 */
namespace Hal {

  class RootIOManager : public IOManager {
    std::vector<TString> fInFileName;
    TString fOutFileName;
    TString fOutTreeName;
    std::vector<TFile*> fInFile;
    TFile* fOutFile;
    std::vector<TTree*> fInTree;
    TTree* fOutTree;
    std::vector<TString> fBranchList;
    std::vector<TBranch*> fInBranches;
    std::vector<TBranch*> fOutBranches;
    std::vector<std::pair<TString, TObject*>> fOutBranchesVirtual;


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
     * return object from input file
     * @param BrName
     * @return
     */
    TObject* GetObject(const char* BrName);
    /**
     * return pointer to the inputfile
     * @return
     */
    TFile* GetInFile();
    void AddFriend(TString name) { fInFileName.push_back(name); };
    void UpdateBranches();
    void Register(const char* name, const char* folderName, TNamed* obj, Bool_t toFile);
    void Register(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile);
    void SetInChain(TChain* tempChain, Int_t ident = -1);
    Int_t CheckBranch(const char* BrName);
    TList* GetBranchNameList();
    virtual ~RootIOManager();
    ClassDef(RootIOManager, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_IO_ROOTIOMANAGER_H_ */
