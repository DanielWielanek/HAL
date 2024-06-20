/*
 * ChainBuilder.h
 *
 *  Created on: 19 cze 2024
 *      Author: daniel
 */

#ifndef HAL_FEATURES_IO_INPUTDATAINFO_H_
#define HAL_FEATURES_IO_INPUTDATAINFO_H_

#include <TObject.h>
#include <TString.h>

#include <vector>

class TChain;
/**
 * basic class for storing information about input files
 */
namespace Hal {

  class InputDataInfo : public TObject {
  protected:
    std::vector<std::vector<TString>> fFileNames;
    TString fListName;

  public:
    /**
     * default ctor
     * @param file single root file, or list of root files (.list extension) or xml file (.xml extension)
     * @param treename tree name if, empty, try to find a tree
     */
    InputDataInfo(TString file = "data.root");
    /**
     *
     * @param fileLists list of files with lists
     * @param treeNames list of tree names
     */
    InputDataInfo(std::initializer_list<TString> fileLists);
    /**
     *
     * @param files direct list of files vec[0] = list of main root files vec[1] - list of friends etc.
     */
    InputDataInfo(std::vector<std::vector<TString>> files);
    /**
     * return name of the file
     * @param level = level of file, -1 for main file
     * @param posJ
     * @return
     */
    TString GetSafeFile(Int_t level, Int_t entry) const;
    /**
     *
     * @param level depth -1 for main file
     * @return
     */
    std::vector<TString> GetSafeFiles(Int_t level) const;
    void AddFile(TString file);
    void AddFriend(TString file, Int_t level);
    void OverwriteSourceName(TString name) { fListName = name; }
    virtual void Print(Option_t* option = "") const;
    Int_t GetNFiles() const;
    Int_t GetFriendsLevel() const;
    TString GetSourceName() const { return fListName; }
    std::vector<std::vector<TString>> GetListOfFiles() const { return fFileNames; }
    virtual ~InputDataInfo() {};
    ClassDef(InputDataInfo, 1);
  };

  /**
   * class for storing information about input root trees, contains also information about tree names
   */
  class InputRootDataInfo : public InputDataInfo {
    TString GetChainName(TString file) const;
    void GuessTrees();
    std::vector<TString> fTreeNames;

  public:
    /**
     * default ctor
     * @param file single root file, or list of root files (.list extension) or xml file (.xml extension)
     * @param treename tree name if, empty, try to find a tree
     */
    InputRootDataInfo(TString file = "data.root", TString treename = "");
    /**
     *
     * @param fileLists list of files with lists
     * @param treeNames list of tree names
     */
    InputRootDataInfo(std::initializer_list<TString> fileLists, std::initializer_list<TString> treeNames = {});
    /**
     *
     * @param files direct list of files vec[0] = list of main root files vec[1] - list of friends etc.
     */
    InputRootDataInfo(std::vector<std::vector<TString>> files);
    /**
     *
     * @return TChain with data, works only with root files
     */
    TChain* GetChain();
    virtual void Print(Option_t* option = "") const;
    ClassDef(InputRootDataInfo, 1)
  };


} /* namespace Hal */

#endif /* HAL_FEATURES_IO_INPUTDATAINFO_H_ */
