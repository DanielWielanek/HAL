/*
 * IOManager.h
 *
 *  Created on: 10 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FEATURES_IO_STEER_IOMANAGER_H_
#define HAL_FEATURES_IO_STEER_IOMANAGER_H_

#include <TObject.h>

class TChain;
class TFile;
namespace Hal {
  class Field;
  class IOManager : public TObject {
    Field* fField;

  public:
    IOManager() : fField(nullptr) {};
    virtual Int_t GetEntries() const = 0;
    virtual Int_t GetEntry(Int_t i)  = 0;
    virtual Bool_t Init()            = 0;
    virtual Field* GetField() const { return fField; };
    virtual void Register(const char* name, const char* folderName, TNamed* obj, Bool_t toFile)      = 0;
    virtual void Register(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile) = 0;
    virtual void SetInChain(TChain* tempChain, Int_t ident = -1)                                     = 0;
    virtual void UpdateBranches()                                                                    = 0;
    virtual Int_t CheckBranch(const char* BrName)                                                    = 0;
    virtual TObject* GetObject(const char* BrName)                                                   = 0;
    virtual TFile* GetInFile()                                                                       = 0;
    virtual TList* GetBranchNameList()                                                               = 0;
    virtual ~IOManager() {};
    ClassDef(IOManager, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_IO_STEER_IOMANAGER_H_ */
