/*
 * MultiDimFile.h
 *
 *  Created on: 22 sty 2025
 *      Author: daniel
 */

#ifndef HAL_FEATURES_MINIMIZER_MULTIDIMFILE_H_
#define HAL_FEATURES_MINIMIZER_MULTIDIMFILE_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>
#include <vector>

#include "Object.h"

class TFile;
class TTree;

namespace Hal {
  class MultiDimDataManager;
  /**
   * class for storing/reading files with multidimensional data
   */
  class MultiDimFile : public Object {
    TFile* fFile;
    TTree* fTree;
    std::vector<Float_t>* fValues     = {nullptr};
    MultiDimDataManager* fDataManager = {nullptr};
    Int_t fMode                       = {0};
    Bool_t Read() const { return (fMode == -1) ? kTRUE : kFALSE; };
    Bool_t Write() const { return (fMode == 1) ? kTRUE : kFALSE; };

  public:
    /**
     *
     * @param file name of the file
     * @param option if "create" or "recreate" make new file, otherwise works in read mode
     * @param writeParams if true positions in dimiensions are writted accoring to sheme
     * Vector[value][par1][par2]....
     */
    MultiDimFile(TString file = "", TString option = "", Bool_t writeParams = kTRUE);
    /**
     * set manager of information about parameters, used only in write mode
     * @param mng
     */
    void SetManager(const Hal::MultiDimDataManager& mng);
    /**
     * load entry from data
     * @param i
     */
    void GetEntry(Int_t i);
    /**
     *
     * @return number of points
     */
    Int_t GetEntries() const;
    /**
     * return values from tree
     * if data writed with "writeParams" options vector looks like
     * vec[data][par1][par2].. otherwise looks like vec[data]
     */
    std::vector<Float_t>* GetValues() const { return fValues; }
    /**
     * fill tree
     */
    void Fill();
    /**
     *
     * @return configuration of file
     */
    MultiDimDataManager* GetConfig() const { return fDataManager; }
    virtual ~MultiDimFile();
    ClassDef(MultiDimFile, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_MINIMIZER_MULTIDIMFILE_H_ */
