/*
 * HalDataManager.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_IO_DATAMANAGER_H_
#define HAL_FEATURES_IO_DATAMANAGER_H_

#include <TObject.h>

/**
 * wrapper around FairRootManager
 */
class TFile;
class TList;
class TChain;
namespace Hal {
  class IOManager;
  class MagField;
  class DataManager : public TObject {
    IOManager* fManager;

  public:
    DataManager(IOManager* mngr = nullptr);
    static DataManager* Instance();
    Int_t CheckBranch(const char* BrName);
    Int_t GetEntry(Int_t i);
    Int_t GetEntries() const;
    Bool_t Init();
    void Register(const char* name, const char* folderName, TNamed* obj, Bool_t toFile);
    void Register(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile);
    void SetInChain(TChain* tempChain, Int_t ident = -1);
    void UpdateBranches();
    void SetManager(IOManager* mngr) { fManager = mngr; }
    TObject* GetObject(const char* BrName);
    TFile* GetInFile();
    TList* GetBranchNameList();
    virtual MagField* GetField() const;
    virtual ~DataManager();
    ClassDef(DataManager, 1)
  };
}  // namespace Hal

#endif /* HAL_FEATURES_IO_DATAMANAGER_H_ */
