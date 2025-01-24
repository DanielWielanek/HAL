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
  class MultiDimFile;
  class CorrelationHisto;
  /**
   * class for interpolation of multimensional data
   */
  class MultiDimExtrapolator : public Object {
    MultiDimFile* fInFile = {nullptr};

  public:
    MultiDimExtrapolator() {};
    /**
     * opens file with data, file should be created via MultiDimFile class
     * @param file
     */
    void OpenFile(TString file);
    /**
     * extrapolate value for given position
     * @param pos
     * @return
     */
    Double_t Extrapolate(const std::vector<Double_t>& pos) const;
    /**
     *
     * @return correlation histogram for all entries
     */
    Hal::CorrelationHisto* GetCorrHisto() const;
    /**
     *
     * @return information about file congiguration (e.g. number of dimensions etc.)
     */
    MultiDimDataManager* GetConfig() const;
    virtual ~MultiDimExtrapolator();
    ClassDef(MultiDimExtrapolator, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_MINIMIZER_MULTIDIMEXTRAPOLATOR_H_ */
