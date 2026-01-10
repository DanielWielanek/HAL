/*
 * MultiDimMinimizer.h
 *
 *  Created on: 16 wrz 2025
 *      Author: daniel
 */

#ifndef HAL_FEATURES_MINIMIZER_MULTIDIMMINIMIZER_H_
#define HAL_FEATURES_MINIMIZER_MULTIDIMMINIMIZER_H_

#include <RtypesCore.h>
#include <TObject.h>
#include <TString.h>
#include <vector>

#include "FitParam.h"

namespace Hal {
  class MinimizerResult;
  class ChiSqMap2D;
  class MultiDimInterpolator;
} /* namespace Hal */


namespace Hal {

  class MultiDimMinimizer : public TObject {
  protected:
    MultiDimInterpolator* fFunc = {nullptr};
    std::vector<FitParam> fParameters;
    std::vector<Double_t> fValues;
    std::vector<int> fMultiOffsets;
    Bool_t fInit = {kFALSE};
    Int_t fDim   = {0};
    virtual void Init();
    void PrintParameterSettings(TString message);

  public:
    MultiDimMinimizer();
    /**
     * open file with stored values, no need to store parameters
     * @param file
     */
    void OpenFile(TString file);
    /**
     * set par limits
     * @param par
     * @param min
     * @param max
     */
    void SetParLimits(Int_t par, Double_t min, Double_t max);
    /**
     * fix parameters
     * @param par
     * @param val
     */
    void FixParameter(Int_t par, Double_t val);
    /**
     *
     * @return number of parameters
     */
    Int_t GetParametersNo() const { return fParameters.size(); }
    /**
     * set initial parameter NOTE: it will be overwrite if SetParLimits/FixParamter is called
     * @param par
     * @param start
     */
    void SetStartParam(Int_t par, Double_t start);
    /**
     *
     * @param par
     * @return parameter's name
     */
    TString GetParameterName(Int_t par) const;
    /**
     * minimize
     * @return
     */
    virtual MinimizerResult Minimize() = 0;
    /**
     *
     * @return
     */
    std::vector<std::vector<Hal::ChiSqMap2D*>> GetChiMap() const;
    virtual ~MultiDimMinimizer();
    ClassDef(MultiDimMinimizer, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_MINIMIZER_MULTIDIMMINIMIZER_H_ */
