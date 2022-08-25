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
namespace Hal {
  namespace Fair {
    class RootManager : public IOManager {
    protected:
      virtual void RegisterInternal(const char* name, const char* folderName, TNamed* obj, Bool_t toFile);
      virtual void RegisterInternal(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile);
      virtual void RefreshBranchList();

    public:
      RootManager();
      TObject* GetObject(const char* BrName);
      TFile* GetInFile();
      void UpdateBranches();
      void SetInChain(TChain* tempChain, Int_t ident = -1);
      Int_t CheckBranch(const char* BrName);
      Int_t GetEntries() const { return -1; }
      Int_t GetEntry(Int_t /*i*/) { return -1; };
      virtual Bool_t Init();
      virtual ~RootManager() {};
      ClassDef(RootManager, 1)
    };
  }  // namespace Fair
}  // namespace Hal
#endif /* HAL_FAIR_ROOTMANAGER_H_ */
