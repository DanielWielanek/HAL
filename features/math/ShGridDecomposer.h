/*
 * ShNumDecomposer.h
 *
 *  Created on: 29 wrz 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_MATH_SHNUMDECOMPOSER_H_
#define HAL_FEATURES_MATH_SHNUMDECOMPOSER_H_


#include "ShIndexes.h"
#include <Rtypes.h>
#include <TObject.h>
#include <complex>
#include <vector>

#include "SimpleMathFunc.h"

namespace Hal {
  namespace Sh {
    /**
     * base class for grid-based decomposition into SH
     */
    class GridDecomposerBase : public TObject {
    protected:
      const int fBins;
      const int fMaxL;
      const int fNtheta;
      const int fNphi;
      const double fStep;
      const double fDeltaAngle;
      Indexes fIndexes;
      std::vector<std::vector<double>> fSlmRe;
      std::vector<std::vector<double>> fSlmIm;  // l x r
    public:
      /**
       * default constructor
       * @param step - bin width
       * @param nbins - number of bins (number of samples along radius)
       * @param maxL - maximal L
       * @param nTheta - number of samples along theta angle
       * @param nPhi - number of samples along phi angle
       */
      GridDecomposerBase(double step = 0.01, int nbins = 100, int maxL = 5, int nTheta = 40, int nPhi = 80);
      /**
       * do decomposition
       * @param
       */
      virtual void Decompose(double* /*params*/) = 0;
      /**
       *
       * @return real decomposed functions
       */
      const std::vector<std::vector<double>>& GetReal() const { return fSlmRe; }
      /**
       *
       * @return imaginary decomposed functions
       */
      const std::vector<std::vector<double>>& GetImag() const { return fSlmIm; }
      /**
       * indexes to number the output
       * @return
       */
      const Indexes& GetIndexes() const { return fIndexes; }
      virtual ~GridDecomposerBase() {};
      ClassDef(GridDecomposerBase, 0)
    };

    /**
     * decomposition on grid that use double values
     */
    class GridDecomposerScalar : public GridDecomposerBase {
    protected:
      struct angleData {
        double sine;
        double cosine;
        double angle;
      };
      std::vector<angleData> fThetaTable;  //!
      std::vector<angleData> fPhiTable;    //!
    protected:
      void ComputeLegendres(std::vector<double>& flat, std::vector<std::vector<double>>& Plm, double x);
      std::vector<std::complex<double>> InnerDecompose(int i, double val);
      Math::Function3D* fFunction = {nullptr};

    public:
      /**
       * default constructor
       * @param step - bin width
       * @param nbins - number of bins (number of samples along radius)
       * @param maxL - maximal L
       * @param nTheta - number of samples along theta angle
       * @param nPhi - number of samples along phi angle
       */
      GridDecomposerScalar(double step = 0.01, int nbins = 100, int maxL = 20, int nTheta = 40, int nPhi = 80);
      /**
       * do Decomposition of function with given parameters
       * @param params parameters of decomposed functions
       */
      void Decompose(double* params);
      /**
       * set function to decompose
       * @param func
       */
      void SetFunction(Math::Function3D* func) { fFunction = func; }
      virtual ~GridDecomposerScalar();
      ClassDef(GridDecomposerScalar, 0)
    };
  }  // namespace Sh

} /* namespace Hal */

#endif /* HAL_FEATURES_MATH_SHNUMDECOMPOSER_H_ */
