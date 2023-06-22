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
 * abstract class that represents almost all I/O operations (except writing histograms after completion of analysis
 */

namespace Hal {
  class MagField;
  class BranchInfo : public TObject {
  public:
    enum class EFlag { kInActive, kInPassive, kOut, kVirtual, kNull };

  private:
    TString fName     = {""};
    TObject* fPointer = {nullptr};
    EFlag fFlag       = {EFlag::kNull};

  public:
    BranchInfo(TString name = "", TObject* pointer = nullptr, EFlag used = EFlag::kNull) :
      fName(name), fPointer(pointer), fFlag(used) {}
    EFlag GetFlag() const { return fFlag; }
    TString GetBranchName() const { return fName; }
    void SetFlag(EFlag Flag = EFlag::kNull) { fFlag = Flag; }
    void SetBranchName(const TString name) { fName = name; }
    TObject* GetPointer() const { return fPointer; }
    void SetPointer(TObject* pointer = nullptr) { pointer = fPointer; }
    virtual ~BranchInfo() {};
    ClassDef(BranchInfo, 1)
  };
  class IOManager : public TObject {
  public:
  private:
    MagField* fField;
    std::vector<TString> fBranchNameList;
    TString fInputName;

  protected:
    /**
     * list of branches
     */
    std::vector<BranchInfo> fBranches;

  protected:
    /**
     * add branch
     * @param name name of the branch
     * @param object pointer to object
     * @param flag
     */
    void AddBranch(TString name, TObject* object, BranchInfo::EFlag flag);
    /**
     * look for branch with given name
     * @param name
     * @return
     */
    BranchInfo FindBranch(TString name) const;
    /**
     * update the branch list
     */
    virtual void RefreshBranchList() {};
    /**
     * Internal function for data registering
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
    IOManager() : fField(nullptr), fBranchNameList(), fInputName("unknown") {};
    /**
     *
     * @return number of entries in data
     */
    virtual Int_t GetEntries() const = 0;
    /**
     * get entry from data
     * @param i entry number
     * @param flag entry flag if 1 get all branches
     * @return
     */
    virtual Int_t GetEntry(Int_t i, Int_t flag) = 0;
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
    BranchInfo::EFlag GetBranchStatus(const char* BrName);
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
    std::vector<TString> GetBranchNameList() {
      RefreshBranchList();
      return fBranchNameList;
    };
    /**
     * prints basic information about I/0
     */
    void PrintInfo();
    /**
     * fill tree with data
     */
    virtual void FillTree() = 0;
    /**
     * close manager (close root file, write the tree with data)
     */
    virtual void CloseManager() {};
    virtual ~IOManager();
    ClassDef(IOManager, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_IO_STEER_IOMANAGER_H_ */
