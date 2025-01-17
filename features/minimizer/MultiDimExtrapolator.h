/*
 * MultiDimExtrapolator.h
 *
 *  Created on: 16 sty 2025
 *      Author: daniel
 */

#ifndef HAL_FEATURES_MINIMIZER_MULTIDIMEXTRAPOLATOR_H_
#define HAL_FEATURES_MINIMIZER_MULTIDIMEXTRAPOLATOR_H_

#include "Object.h"

#include "MultiDimDataManager.h"

class TTree;
class TFile;
namespace Hal {
  class CorrelationHisto;
  class MultiDimExtrapolator : public Object {
    MultiDimDataManager* fDataManager = {nullptr};
    TTree* fTree                      = {nullptr};
    TFile* fFile                      = {nullptr};
    std::vector<Float_t>* fValues     = {nullptr};

  public:
    MultiDimExtrapolator() {};
    void LoadFromTFile(TString file);
    Double_t Extrapolate(std::vector<Double_t>& pos) const;
    Hal::CorrelationHisto* GetCorrHisto() const;
    MultiDimDataManager* GetDataManager() const { return fDataManager; }
    virtual ~MultiDimExtrapolator();
    ClassDef(MultiDimExtrapolator, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_MINIMIZER_MULTIDIMEXTRAPOLATOR_H_ */
