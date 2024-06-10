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
#include <TString.h>

class TList;
class TChain;
class TFile;
/**
 * abstract class that represents almost all I/O operations (except writing histograms after completion of analysis
 */

namespace Hal {
  class Package;
  class MagField;
  class BranchInfo : public TObject {
  public:
    enum class EFlag { kInActive, kInPassive, kOut, kVirtual, kNull };

  private:
    TString fBrName   = {""};
    TObject* fPointer = {nullptr};  //!
    EFlag fFlag       = {EFlag::kNull};

  public:
    BranchInfo(TString name = "", TObject* pointer = nullptr, EFlag used = EFlag::kNull) :
      fBrName(name), fPointer(pointer), fFlag(used) {}
    EFlag GetFlag() const { return fFlag; }
    TString GetBranchName() const { return fBrName; }
    void SetFlag(EFlag Flag = EFlag::kNull) { fFlag = Flag; }
    void SetBranchName(const TString name) { fBrName = name; }
    TObject* GetPointer() const { return fPointer; }
    void SetPointer(TObject* pointer = nullptr) { fPointer = pointer; }
    virtual ~BranchInfo() {};
    ClassDef(BranchInfo, 1)
  };

  class IOManager : public TObject {
  public:
  private:
    MagField* fField;
    std::vector<TString> fBranchNameList;
    std::vector<std::vector<TString>> fInFiles;
    TString fInputName;
    std::vector<TString> GetSafeFile(UInt_t pos) const;
    TString GetSafeFile(UInt_t i, UInt_t j) const;

  protected:
    /**
     * list of branches
     */
    std::vector<Hal::BranchInfo> fBranches;
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
    virtual Bool_t InitInternal()                                                                            = 0;

  public:
    IOManager() : fField(nullptr), fBranchNameList(), fInputName("unknown") {};
    IOManager(TString list);
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
    Bool_t Init();
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
    /**
     *
     * @return report
     */
    virtual TList* GetBranchesList() const;
    /**
     *
     * @return number of main files
     */
    Int_t GetNFiles() const;
    /**
     *
     * @param entry
     * @return names of files at given entry (entry is number of file in chain), returns main_tree, friend_1, friend_2 etc.
     */
    std::vector<TString> GetFilesNames(Int_t entry = 0) const;
    /**
     *
     * @return number of friens of first file, return -1 if no friens added
     */
    Int_t GetFriendsLevel() const;
    void AddFile(TString name);
    void AddFriend(TString friendName, Int_t level);
    /**
     *
     * @return name of first file with data
     */
    TString GetFirstDataFileName() const;
    /**
     * name of the first friend file
     * @param level
     * @return
     */
    TString GetFirstFriendFileName(Int_t level) const;
    /**
     *
     * @param level level
     * @return return names of fies at given level if leve==-1 return list of main files
     */
    std::vector<TString> GetFileNameList(Int_t level) const;
    /**
     *
     * @param level
     * @return list of friends
     */
    std::vector<TString> GetFriends(Int_t level) const;
    virtual ~IOManager();
    ClassDef(IOManager, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_IO_STEER_IOMANAGER_H_ */
