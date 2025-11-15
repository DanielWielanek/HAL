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
class TCollection;
namespace Hal {
  class InputDataInfo;
}


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
    BranchInfo(const BranchInfo& other) : TObject(other), fBrName(other.fBrName), fPointer(other.fPointer), fFlag(other.fFlag) {}
    EFlag GetFlag() const { return fFlag; }
    TString GetBranchName() const { return fBrName; }
    void SetFlag(EFlag Flag = EFlag::kNull) { fFlag = Flag; }
    void SetBranchName(const TString name) { fBrName = name; }
    TObject* GetPointer() const { return fPointer; }
    void SetPointer(TObject* pointer = nullptr) { fPointer = pointer; }
    virtual ~BranchInfo() {};
    ClassDef(BranchInfo, 1)
  };
  /**
   * abstract class that represents almost all I/O operations (except writing histograms after completion of analysis
   */
  class IOManager : public TObject {
  public:
  private:
    MagField* fField = {nullptr};
    std::vector<TString> fBranchNameList;

  protected:
    InputDataInfo* fDataInfo = {nullptr};
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
     * Internal function for data registering
     * @param name
     * @param folderName
     * @param obj
     * @param toFile
     */
    virtual void RegisterInternal(TString name, TString folderName, TObject* obj, Bool_t toFile) = 0;
    virtual Bool_t InitInternal()                                                                = 0;

  public:
    IOManager() : fField(nullptr), fBranchNameList() {};
    /**
     * IO manager
     * @param info - information input data
     */
    IOManager(InputDataInfo* info);
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
     * set branch mode to active without returing object
     * @param brName
     */
    void ActivateBranch(TString brName);
    /**
     * return pointer to input root file
     * @return
     */
    TString GetSourceName() const;
    /**
     *
     * @return list to branches with data
     */
    std::vector<TString> GetBranchNameList() {
      UpdateBranches();
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
     * @return number of friens of first file, return -1 if no friends added
     */
    Int_t GetFriendsLevel() const;
    /**
     * add file to list
     * @param name
     */
    void AddFile(TString name);
    /**
     * add friend to list of files
     * @param friendName
     * @param level
     */
    void AddFriend(TString friendName, Int_t level);
    /**
     * set to 0 status of unused branches
     */
    virtual void LockUnusedBranches() {};
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
     * @return return names of files at given level if level==-1 return list of main files
     */
    std::vector<TString> GetFileNameList(Int_t level) const;
    /**
     * switch gFile to output file
     */
    virtual void CdToOutput() = 0;
    virtual ~IOManager();
    ClassDef(IOManager, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_IO_STEER_IOMANAGER_H_ */
