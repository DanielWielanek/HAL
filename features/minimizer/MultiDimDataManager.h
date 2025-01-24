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
    void AddParameter(TString name, Double_t low, Double_t high, Double_t step);
    void AddParameter(FitParam& param);
    void Init();
    inline Int_t GetParametersNo() const { return fParams.size(); }
    Int_t GetIndexInt(std::vector<int> paramsId) const;
    Int_t GetIndexAny(std::vector<double> paramsVal) const;
    const FitParam& GetParam(Int_t pos) const { return fParams[pos]; }
    std::vector<Double_t> GetValues(Int_t entry) const;
    std::vector<Int_t> GetIndexes(Int_t entry) const;
    std::vector<FitParam> GetParams() const { return fParams; }
    virtual void Print(Option_t* option = "") const;
    virtual ~MultiDimDataManager() {};
    ClassDef(MultiDimDataManager, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_MINIMIZER_MULTIDIMDATAMANAGER_H_ */
