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

class TObject;
class TFile;
class TChain;
class TList;
namespace Hal {
  namespace Fair {
    class RootManager : public IOManager {
    public:
      RootManager() {};
      TObject* GetObject(const char* BrName);
      TFile* GetInFile();
      void UpdateBranches();
      void Register(const char* name, const char* folderName, TNamed* obj, Bool_t toFile);
      void Register(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile);
      void SetInChain(TChain* tempChain, Int_t ident = -1);
      Int_t CheckBranch(const char* BrName);
      Int_t GetEntries() const { return -1; }
      Int_t GetEntry(Int_t i) { return -1; };
      virtual Bool_t Init() { return kTRUE; }
      TList* GetBranchNameList();
      virtual ~RootManager() {};
      ClassDef(RootManager, 1)
    };
  }  // namespace Fair
}  // namespace Hal
#endif /* HAL_FAIR_ROOTMANAGER_H_ */
