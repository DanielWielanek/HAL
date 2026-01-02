/*
 * MinimzerResult.h
 *
 *  Created on: 16 wrz 2025
 *      Author: daniel
 */

#ifndef HAL_FEATURES_MINIMIZER_MINIMIZERRESULT_H_
#define HAL_FEATURES_MINIMIZER_MINIMIZERRESULT_H_

#include <vector>

#include "FitParam.h"
#include "Object.h"

namespace Hal {
  /**
   * contains information about fitted parameter
   */
  class FittedParam : public Object {
    FitParam fFitParam;
    Double_t fVal       = {0};
    Double_t fLowError  = {0};
    Double_t fHighError = {0};

  public:
    FittedParam() {};
    FittedParam(FitParam par) { fFitParam = par; };
    virtual ~FittedParam() {};
    /**
     *
     * @return fit param
     */
    const FitParam& GetFitParam() const { return fFitParam; }
    /**
     * sets fit param
     */
    void SetFitParam(const FitParam& FitParam) { fFitParam = FitParam; }
    /**
     * set upper error
     * @param HighError
     */
    void SetHighError(Double_t HighError = 0) { fHighError = HighError; }
    /**
     * set lower error
     * @param LowError
     */
    void SetLowError(Double_t LowError = 0) { fLowError = LowError; }
    /**
     * set fitted value
     * @param Val
     */
    void SetVal(Double_t Val = 0) { fVal = Val; }
    /**
     *
     * @return upper error
     */
    Double_t GetHighError() const { return fHighError; }
    /**
     *
     * @return lower error
     */
    Double_t GetLowError() const { return fLowError; }
    /**
     *
     * @return fitted value
     */
    Double_t GetVal() const { return fVal; };
    ClassDef(FittedParam, 1)
  };

  /**
   * result of fiting
   */
  class MinimizerResult : public Object {
    std::vector<FittedParam> fParams;

  public:
    MinimizerResult() {};
    /**
     * add parameter
     * @param par
     */
    void AddParam(FittedParam par) { fParams.push_back(par); }
    /**
     * get param by index
     * @param index
     * @return
     */
    FittedParam GetParam(Int_t index) const;
    /**
     * get param by name
     * @param name
     * @return
     */
    FittedParam GetParam(TString name) const;
    /**
     *
     * @return number of parameters
     */
    Int_t GetNParams() const { return fParams.size(); }
    virtual ~MinimizerResult() {};
    ClassDef(MinimizerResult, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_MINIMIZER_MINIMIZERRESULT_H_ */
