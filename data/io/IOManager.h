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

#include <TList.h>
#include <TObject.h>

class TList;
class TChain;
class TFile;
/**
 * abstract class that represents almost all I/O operations (except writing histograms after completiion of analysis
 */
namespace Hal {
  class MagField;
  class IOManager : public TObject {
  private:
    MagField* fField;
    TList* fBranchNameList;
    TString fInputName;

  protected:
    std::vector<std::pair<TString, TObject*>> fInBranches;
    std::vector<std::pair<TString, TObject*>> fOutBranches;
    std::vector<std::pair<TString, TObject*>> fOutVirtual;

  protected:
    enum class EBranchFlag { kAny, kIn, kOut, kVirtual };
    void AddBranch(TString name, TObject* object, EBranchFlag flag);
    std::pair<TString, TObject*> FindBranch(TString name, EBranchFlag flag) const;
    virtual void RefreshBranchList() {};
    /**
     * interfal function for data registering
     * @param name
     * @param folderName
     * @param obj
     * @param toFile
     */
    virtual void RegisterInternal(const char* name, const char* folderName, TNamed* obj, Bool_t toFile) = 0;
    /**
     * internal function for data registering
     * @param name
     * @param Foldername
     * @param obj
     * @param toFile
     */
    virtual void RegisterInternal(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile) = 0;

  public:
    enum class EBranchStatus { kInput, kOutput, kVirtual, kNull };
    IOManager() : fField(nullptr), fBranchNameList(new TList()), fInputName("unknown") {};
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
     *
     * @return name of main input file name if avaiable
     */
    TString GetInputFileName() const { return fInputName; }
    /**
     * set magnetic field
     * @param field
     */
    virtual void SetField(MagField* field) { fField = field; };
    /**
     * set name of the output file
     * @param name
     */
    virtual void SetOutput(TString /*name*/) {};
    /**
     *  set input file name
     * @param name
     */
    void SetInputName(TString name) { fInputName = name; }
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
    void Register(const char* name, const char* folderName, TNamed* obj, Bool_t toFile);
    /**
     * register data in output file
     * @param name name of the branch
     * @param folderName name of the directory with branch
     * @param obj pointer to written object
     * @param toFile if true then data will be written to the output
     */
    void Register(const char* name, const char* Foldername, TCollection* obj, Bool_t toFile);
    /**
     * not used
     * @param tempChain
     * @param ident
     */
    virtual void SetInChain(TChain* tempChain, Int_t ident = -1) = 0;
    /**
     * not used
     */
    virtual void UpdateBranches() {};
    /**
     * return 1 if branch exists
     * @param BrName
     * @return
     */
    EBranchStatus GetBranchStatus(const char* BrName);
    /**
     * return object from branch, return nullptr if object /branch does not exist
     * @param BrName
     * @return
     */
    virtual TObject* GetObject(const char* BrName);
    /**
     * return pointer to input root file
     * @return
     */
    virtual TFile* GetInFile() = 0;
    /**
     *
     * @return list to branches with data
     */
    TList* GetBranchNameList() {
      RefreshBranchList();
      return fBranchNameList;
    };
    /**
     * prints basic information about I/0
     */
    void PrintInfo();
    virtual void FillTree() {};
    virtual void CloseManager() {};
    virtual ~IOManager();
    ClassDef(IOManager, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_IO_STEER_IOMANAGER_H_ */
