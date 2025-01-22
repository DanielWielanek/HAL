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
  class MultiDimFile : public Object {
    TFile* fFile;
    TTree* fTree;
    std::vector<Float_t>* fValues     = {nullptr};
    MultiDimDataManager* fDataManager = {nullptr};
    Int_t fMode                       = {0};
    Bool_t Read() const { return (fMode == -1) ? kTRUE : kFALSE; };
    Bool_t Write() const { return (fMode == 1) ? kTRUE : kFALSE; };

  public:
    MultiDimFile(TString file = "", TString option = "", Bool_t writeParams = kTRUE);
    void SetManager(const Hal::MultiDimDataManager& mng);
    void GetEntry(Int_t i);
    Int_t GetEntries() const;
    std::vector<Float_t>* GetValues() const { return fValues; }
    void Fill();
    MultiDimDataManager* GetConfig() const { return fDataManager; }
    virtual ~MultiDimFile();
    ClassDef(MultiDimFile, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_MINIMIZER_MULTIDIMFILE_H_ */
