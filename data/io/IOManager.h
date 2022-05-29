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
#include <TString.h>

class TChain;
class TFile;
/**
 * abstract class that represents almost all I/O operations (except writing histograms after completiion of analysis
 */
namespace Hal {
  class MagField;
  class IOManager : public TObject {
    MagField* fField;

  public:
    IOManager() : fField(nullptr) {};
    /**
     *
     * @return number of entries in data
     */
    virtual Int_t GetEntries() const = 0;
    /**
     * get entry from data
     * @param i
     * @return
     */
    virtual Int_t GetEntry(Int_t i) = 0;
    /**
     * initialize this task
     * @return
     */
    virtual Bool_t Init() = 0;
    /**
     * set magnetic field
     * @param field
     */
    virtual void SetField(MagField* field) { fField = field; };
    /**
     * set name of the output file
     * @param name
     */
    virtual void SetOutput(TString name) {};
    /**
     * return magnetic field for this data
     * @return
     */
    virtual MagField* GetField() const { return fField; };
    /**
     * register data in output file
     * @param name name of the branch
     * @param folderName name of the directory with branch
     * @param obj pointer to written object
     * @param toFile if true then data will be written to the output
     */
    virtual void Register(const char* name, const char* folderName, TNamed* obj, Bool_t toFile) = 0;
    /**
     * register data in output file
     * @param name name of the branch
     * @param folderName name of the directory with branch
     * @param obj pointer to written object
     * @param toFile if true then data will be written to the output
     */
    virtual void Register(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile) = 0;
    /**
     * not used
     * @param tempChain
     * @param ident
     */
    virtual void SetInChain(TChain* tempChain, Int_t ident = -1) = 0;
    /**
     * not used
     */
    virtual void UpdateBranches() = 0;
    /**
     * return 1 if branch exists
     * @param BrName
     * @return
     */
    virtual Int_t CheckBranch(const char* BrName) = 0;
    /**
     * return object from branch, return nullptr if object /branch does not exist
     * @param BrName
     * @return
     */
    virtual TObject* GetObject(const char* BrName) = 0;
    /**
     * return pointer to input root file
     * @return
     */
    virtual TFile* GetInFile() = 0;
    /**
     *
     * @return list to branches with data
     */
    virtual TList* GetBranchNameList() = 0;
    virtual ~IOManager() {};
    ClassDef(IOManager, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_IO_STEER_IOMANAGER_H_ */
