/*
 * MultiDimMinuit.h
 *
 *  Created on: 23 sty 2025
 *      Author: daniel
 */

#ifndef HAL_FEATURES_MINIMIZER_MULTIDIMMINUIT_H_
#define HAL_FEATURES_MINIMIZER_MULTIDIMMINUIT_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>
#include <vector>

#include "FitParam.h"
#include "Object.h"

namespace ROOT {
  namespace Math {
    class Minimizer;
  } /* namespace Math */
} /* namespace ROOT */


namespace Hal {
  class MultiDimExtrapolator;

  class MultiDimMinuit : public Object {
  public:
    /**minim algo similar to those in CorrFit */
    enum EMinAlgo { kMinuitMigrad = 0, kMinuitSimplex = 1, kMinuitCombined = 2, kMinuitScan = 3, kMinuitFumili = 4 };

  protected:
    std::vector<TString> AlgoToOptions(EMinAlgo algo);
    EMinAlgo fMinAlgo           = {EMinAlgo::kMinuitMigrad};
    MultiDimExtrapolator* fFunc = {nullptr};
    std::vector<FitParam> fParameters;
    std::vector<Double_t> fValues;
    ROOT::Math::Minimizer* fMinimizer = {nullptr};
    Bool_t fInit                      = {kFALSE};
    double GetEval(const double* params);

  public:
    MultiDimMinuit();
    /**
     * set minimizalization algo
     * @param algo
     */
    void SetMinAlgo(EMinAlgo algo) { fMinAlgo = algo; }
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
     * set initial parameter NOTE: it will be overwrite if SetParLimits/FixParamter is called
     * @param par
     * @param start
     */
    void SetStarParam(Int_t par, Double_t start);
    /**
     * run minimizer
     */
    void Minimize();
    /**
     *
     * @param par
     * @return parameter's name
     */
    TString GetParameterName(Int_t par) const;
    /**
     *
     * @param par fitted parameter if Mimize was called
     * @return
     */
    Double_t GetParameter(Int_t par) const;
    /**
     *
     * @param par
     * @return parameter uncertainty if Minimize was called
     */
    Double_t GetParError(Int_t par) const;
    /**
     *
     * @return number of parameters
     */
    Int_t GetParametersNo() const { return fParameters.size(); }
    virtual ~MultiDimMinuit();
    ClassDef(MultiDimMinuit, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_MINIMIZER_MULTIDIMMINUIT_H_ */
