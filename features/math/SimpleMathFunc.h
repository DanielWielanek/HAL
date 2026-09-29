/*
 * SimpleFunc.h
 *
 *  Created on: 29 wrz 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_MATH_SIMPLEFUNC_H_
#define HAL_FEATURES_MATH_SIMPLEFUNC_H_


#include <Rtypes.h>
#include <TMath.h>
#include <TObject.h>

#include "StdSimd.h"

namespace Hal {

  /** simple math function
   */
  namespace Math {
    class Function3D : public TObject {
    public:
      Function3D() {};
      /**
       * eval at x,y,z
       * @param x
       * @param y
       * @param z
       * @return
       */
      virtual double Eval(double x, double y, double z) const = 0;
      /**
       * change parameters of fuction
       * @param
       */
      virtual void SetParams(double* /*p*/) {};
      virtual ~Function3D() {};
      ClassDef(Function3D, 0)
    };
    /** simple vectorized math function
     */
    class SimdFunctionF3D : public TObject {
    public:
      SimdFunctionF3D() {};
      /**
       * eval at point(s)
       * @param x
       * @param y
       * @param z
       * @return
       */
      virtual Float_v Eval(Float_v& x, Float_v& y, Float_v& z) const = 0;
      /**
       * change parameters
       * @param
       */
      virtual void SetParams(double* /*p*/) {};
      virtual ~SimdFunctionF3D() {};
      ClassDef(SimdFunctionF3D, 0)
    };

  }  // namespace Math

} /* namespace Hal */

#endif /* HAL_FEATURES_MATH_SIMPLEFUNC_H_ */
