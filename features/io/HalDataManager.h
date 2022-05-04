/*
 * HalDataManager.h
 *
 *  Created on: 30-04-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_IO_HALDATAMANAGER_H_
#define HAL_FEATURES_IO_HALDATAMANAGER_H_

#include <TObject.h>

/**
 * wrapper around FairRootManager
 */
class TFile;
class TList;
class TChain;
class FairRootManager;

class HalRootManager : public TObject {
public:
  HalRootManager() {};
  virtual void Register(const char* name, const char* folderName, TNamed* obj, Bool_t toFile)      = 0;
  virtual void Register(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile) = 0;
  virtual void SetInChain(TChain* tempChain, Int_t ident = -1)                                     = 0;
  virtual void UpdateBranches()                                                                    = 0;
  virtual Int_t CheckBranch(const char* BrName)                                                    = 0;
  virtual TObject* GetObject(const char* BrName)                                                   = 0;
  virtual TFile* GetInFile()                                                                       = 0;
  virtual TList* GetBranchNameList()                                                               = 0;
  virtual ~HalRootManager() {};
  ClassDef(HalRootManager, 1)
};

class HalFairRootManager : public HalRootManager {
public:
  HalFairRootManager() {};
  TObject* GetObject(const char* BrName);
  TFile* GetInFile();
  void UpdateBranches();
  void Register(const char* name, const char* folderName, TNamed* obj, Bool_t toFile);
  void Register(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile);
  void SetInChain(TChain* tempChain, Int_t ident = -1);
  Int_t CheckBranch(const char* BrName);
  TList* GetBranchNameList();
  virtual ~HalFairRootManager() {};
  ClassDef(HalFairRootManager, 1)
};


class HalDataManager : public TObject {
  HalRootManager* fManager;

public:
  HalDataManager(HalRootManager* mngr = nullptr);
  void SetManager(HalRootManager* mngr) { fManager = mngr; }
  HalRootManager* GetManager() const { return fManager; };
  static HalDataManager* Instance();
  virtual ~HalDataManager();
  ClassDef(HalDataManager, 1)
};


#endif /* HAL_FEATURES_IO_HALDATAMANAGER_H_ */
