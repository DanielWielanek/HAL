/*
 * FemtoMathFuncs.h
 *
 *  Created on: 29 wrz 2026
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_BASE_FEMTOMATHFUNCS_H_
#define HAL_ANALYSIS_FEMTO_BASE_FEMTOMATHFUNCS_H_

#include "SimpleMathFunc.h"


namespace Hal {
  namespace Femto {
    /**
     * special class that represents "femto-gauss"
     */
    class Gauss3D : public Math::Function3D {
      double fRx = {0}, fRy = {0}, fRz = {0};
      double fNorm            = {0};
      const double fNormScale = {TMath::Power(TMath::TwoPi(), 1.5) * TMath::Sqrt2() * TMath::Sqrt2() * TMath::Sqrt2()};

    public:
      Gauss3D() {};
      virtual double Eval(double x, double y, double z) const;
      virtual void SetParams(double* p);
      virtual ~Gauss3D() {};
      ClassDef(Gauss3D, 1)
    };
    /**
     * special class that represents "femto-gauss" and uses SIMD
     */
    class Gaus3DSimd : public Math::SimdFunctionF3D {

      Float_v* fNorm      = {nullptr};  //!
      Float_v* fNormScale = {nullptr};  //!
      Float_v* fRx        = {nullptr};  //!
      Float_v* fRy        = {nullptr};  //!
      Float_v* fRz        = {nullptr};  //!

    public:
      Gaus3DSimd();
      virtual Float_v Eval(Float_v& x, Float_v& y, Float_v& z) const;
      virtual void SetParams(double* p);
      virtual ~Gaus3DSimd();
      ClassDef(Gaus3DSimd, 0)
    };
  }  // namespace Femto
} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_BASE_FEMTOMATHFUNCS_H_ */
