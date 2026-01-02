/*
 * MultiDimMinuit.h
 *
 *  Created on: 23 sty 2025
 *      Author: daniel
 */

#ifndef HAL_FEATURES_MINIMIZER_MULTIDIMMINUIT_H_
#define HAL_FEATURES_MINIMIZER_MULTIDIMMINUIT_H_

#include <Rtypes.h>
#include <TString.h>
#include <vector>

#include "MultiDimMinimizer.h"

namespace Hal {
  class MinimizerResult;
} /* namespace Hal */

namespace ROOT {
  namespace Math {
    class Minimizer;
  } /* namespace Math */
} /* namespace ROOT */


namespace Hal {
  class MultiDimInterpolator;
  /**
   * use n-dimensional map but with interpolation to fit with ROOT and estimate the statistical uncertainties
   */
  class MultiDimMinuit : public MultiDimMinimizer {
  public:
    /**minim algo similar to those in CorrFit */
    enum EMinAlgo { kMinuitMigrad = 0, kMinuitSimplex = 1, kMinuitCombined = 2, kMinuitScan = 3, kMinuitFumili = 4 };

  protected:
    std::vector<TString> AlgoToOptions(EMinAlgo algo);
    EMinAlgo fMinAlgo                 = {EMinAlgo::kMinuitMigrad};
    ROOT::Math::Minimizer* fMinimizer = {nullptr};
    double GetEval(const double* params);

  public:
    MultiDimMinuit();
    /**
     * set minimizalization algo
     * @param algo
     */
    void SetMinAlgo(EMinAlgo algo) { fMinAlgo = algo; }
    /**
     * run minimizer
     */
    MinimizerResult Minimize();
    virtual ~MultiDimMinuit();
    ClassDef(MultiDimMinuit, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_MINIMIZER_MULTIDIMMINUIT_H_ */
