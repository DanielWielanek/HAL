/*
 * MultiDimDataManager.h
 *
 *  Created on: 16 sty 2025
 *      Author: daniel
 */

#ifndef HAL_FEATURES_MINIMIZER_MULTIDIMDATAMANAGER_H_
#define HAL_FEATURES_MINIMIZER_MULTIDIMDATAMANAGER_H_

#include "FitParam.h"
#include "Object.h"
namespace Hal {
  /**
   * class that handles file with multidimensional data
   */
  class MultiDimDataManager : public Object {
    std::vector<FitParam> fParams;
    std::vector<Int_t> fTempVec;

  public:
    MultiDimDataManager() {};
    /**
     * add parameter
     * @param name
     * @param low
     * @param high
     * @param step
     */
    void AddParameter(TString name, Double_t low, Double_t high, Double_t step);
    /**
     * add parameter
     * @param param
     */
    void AddParameter(FitParam& param);
    /**
     * initialize mamanger by initialization of parameters
     */
    void Init();
    /**
     *
     * @return number of parameters/dimensions
     */
    inline Int_t GetParametersNo() const { return fParams.size(); }
    /**
     *
     * @param paramsId
     * @return index in TTree for given point in grid [par1 index ][par2 index] ..
     */
    Int_t GetIndexInt(std::vector<int> paramsId) const;
    /**
     *
     * @param paramsVal
     * @return index in Tree for given coordinates (or the closest place)
     */
    Int_t GetIndexAny(std::vector<double> paramsVal) const;
    /**
     * return parameter configuration copy
     * @param pos
     * @return
     */
    const FitParam& GetParam(Int_t pos) const { return fParams[pos]; }
    /**
     * return array of allowed values for given entry in tree
     * @param entry
     * @return
     */
    std::vector<Double_t> GetValues(Int_t entry) const;
    /**
     *
     * @param entry
     * @return positions in parameters space for given entry in tree
     */
    std::vector<Int_t> GetIndexes(Int_t entry) const;
    /**
     * return copy of parameters
     * @return
     */
    std::vector<FitParam> GetParams() const { return fParams; }
    virtual void Print(Option_t* option = "") const;
    virtual ~MultiDimDataManager() {};
    ClassDef(MultiDimDataManager, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_MINIMIZER_MULTIDIMDATAMANAGER_H_ */
