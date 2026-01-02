/*
 * MultiDimMInimizerSquare.h
 *
 *  Created on: 16 wrz 2025
 *      Author: daniel
 */

#ifndef HAL_FEATURES_MINIMIZER_MULTIDIMMINIMIZERSQUARE_H_
#define HAL_FEATURES_MINIMIZER_MULTIDIMMINIMIZERSQUARE_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <vector>

#include "MultiDimMinimizer.h"

namespace Hal {
  class MultiDimFile;
  class MinimizerResult;
} /* namespace Hal */


namespace Hal {
  /**
   * try to minimize map by searching in multidim file
   */
  class MultiDimMinimizerSquare : public MultiDimMinimizer {
    std::vector<std::vector<int>> fPoints;
    std::vector<int> fStateVector;
    std::vector<int> fStateVectorInt;
    std::vector<int> fStateVectorMin;
    std::vector<int> fErrLow, fErrHigh;
    std::vector<int> fStateMin;
    Hal::MultiDimFile* fFileRaw = {nullptr};
    Double_t fChiMin            = 1E+9;
    Double_t fThreshold         = {0};


    void LoopOverParameter(Int_t n);
    void LoopOverParameterError(Int_t n);
    void FindParam();
    void FindError();

  public:
    MultiDimMinimizerSquare() {};
    MinimizerResult Minimize();
    virtual ~MultiDimMinimizerSquare() {};

  protected:
    virtual void Init();
    ClassDef(MultiDimMinimizerSquare, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_MINIMIZER_MULTIDIMMINIMIZERSQUARE_H_ */
