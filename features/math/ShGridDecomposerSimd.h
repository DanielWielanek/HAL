/*
 * ShGridDecomposerSimd.h
 *
 *  Created on: 29 wrz 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_MATH_SHGRIDDECOMPOSERSIMDGRIDDECOMPOSER_H_
#define HAL_FEATURES_MATH_SHGRIDDECOMPOSERSIMDGRIDDECOMPOSER_H_

#include "ShGridDecomposer.h"
#include "StdSimd.h"

namespace Hal {
  namespace Sh {
    /**
     * vectorized version of algo for decomposition into SH
     */
    class GridDecomposerSimd : public GridDecomposerBase {
    protected:
      const float fEfPhi;
      struct angleDataV {
        Float_v sine;
        Float_v cosine;
        Float_v angle;
      };
      std::vector<angleDataV> fThetaTable;  //!
      std::vector<angleDataV> fPhiTable;    //!
      Indexes fIndexes;
      Math::SimdFunctionF3D* fFunction = {nullptr};

    protected:
      void ComputeLegendres(std::vector<double>& flat,
                            std::vector<std::vector<Float_v>>& Plm,  // @suppress("Invalid template argument")
                            double x);                               // @suppress("Invalid template argument")
      std::vector<std::complex<double>> InnerDecompose(int i, double val);

    public:
      GridDecomposerSimd(double step = 0.01, int nbins = 100, int maxL = 20, int nTheta = 40, int nPhi = 80);
      void Decompose(double* params);
      void SetFunction(Math::SimdFunctionF3D* func) { fFunction = func; }
      virtual ~GridDecomposerSimd();
      ClassDef(GridDecomposerSimd, 0)
    };
  }  // namespace Sh
}  // namespace Hal
#endif /* HAL_FEATURES_MATH_SHGRIDDECOMPOSERSIMDGRIDDECOMPOSER_H_ */
