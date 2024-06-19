/*
 * HalFairRootManager2.h
 *
 *  Created on: 5 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FAIR_ROOTMANAGER_H_
#define HAL_FAIR_ROOTMANAGER_H_

#include "DataManager.h"

#include "IOManager.h"


/**
 * IO manager for FairRoot
 */
class TObject;
class TFile;
class TChain;
class TList;
class FairRootManager;
namespace Hal {
  class InputRootDataInfo;
  namespace Fair {
    class RootManager : public IOManager {
      FairRootManager* fFairManager;

    protected:
      virtual void RegisterInternal(const char* name, const char* folderName, TNamed* obj, Bool_t toFile);
      virtual void RegisterInternal(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile);
      virtual void RefreshBranchList();
      virtual Bool_t InitInternal();

    public:
      RootManager();
      TObject* GetObject(const char* BrName);
      void UpdateBranches();
      void FillTree() {};
      void SetInChain(TChain* tempChain, Int_t ident = -1);
      Int_t CheckBranch(const char* BrName);
      Int_t GetEntries() const { return -1; }
      Int_t GetEntry(Int_t /*i*/, Int_t /*flag*/) { return -1; };
      virtual ~RootManager() {};
      ClassDef(RootManager, 1)
    };
  }  // namespace Fair
}  // namespace Hal
#endif /* HAL_FAIR_ROOTMANAGER_H_ */
