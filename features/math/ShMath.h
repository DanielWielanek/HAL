/*
 * SHMath.h
 *
 *  Created on: 29 wrz 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_MATH_SHMATH_H_
#define HAL_FEATURES_MATH_SHMATH_H_


#include "Array.h"

#include "ShIndexes.h"

#include <complex>

namespace Hal {
  namespace Sh {


    /**
     * spherical harmonics functions
     */
    class YlmMath : public TObject {
    private:
      std::complex<double> Ceiphi(double phi) const;
      double fPrefactors[36];
      int fPrefshift[6];
      int fPlmshift[6];
      double fFactorials[24];
      mutable std::complex<double>* fYlms = {nullptr};  //!
      void InitializeYlms();

    public:
      /**
       * default constructor
       */
      YlmMath();
      virtual ~YlmMath();
      /**
       * copy ctor
       * @param aYlm
       */
      YlmMath(const YlmMath& aYlm);
      YlmMath& operator=(const YlmMath& aYlm);
      /**
       * calculate Legendre polynomial up to maxL (5)
       * @param l
       * @param m
       * @param ctheta cos(theta)
       * @return
       */
      double Legendre(int l, int m, double ctheta) const;
      /**
       * Calculate a set of legendre polynomials up to a given l
       * @param lmax
       * @param ctheta
       * @param lbuf
       */
      void LegendreUpToYlm(int lmax, double ctheta, double* lbuf) const;
      /**
       *
       * @param l
       * @param m
       * @param theta
       * @param phi
       * @return Ylm function
       */
      std::complex<double> Ylm(int l, int m, double theta, double phi) const;
      /**
       *
       * @param l
       * @param m
       * @param x
       * @param y
       * @param z
       * @return Ylm function for cartesian coordinates
       */
      std::complex<double> Ylm(int l, int m, double x, double y, double z) const;
      /**
       *
       * @param lmax
       * @param x
       * @param y
       * @param z
       * @return Calculate a set of Ylms up to a given l
       */
      std::complex<double>* YlmUpToL(int lmax, double x, double y, double z);
      /**
       *
       * @param lmax
       * @param ctheta
       * @param phi
       * @return Calculate a set of Ylms up to a given l
       */
      std::complex<double>* YlmUpToL(int lmax, double ctheta, double phi);
      /**
       *
       * @param l
       * @param m
       * @param theta
       * @param phi
       * @return real part of Ylm
       */
      double ReYlm(int l, int m, double theta, double phi) const;
      /**
       *
       * @param l
       * @param m
       * @param x
       * @param y
       * @param z
       * @return real part of Ylm, uses Cartesian coordinates
       */
      double ReYlm(int l, int m, double x, double y, double z) const;
      /**
       *
       * @param l
       * @param m
       * @param theta
       * @param phi
       * @return
       */
      double ImYlm(int l, int m, double theta, double phi) const;
      /**
       *
       * @param l
       * @param m
       * @param x
       * @param y
       * @param z
       * @return imaginary part of Ylm, uses Cartesian coordinates
       */
      double ImYlm(int l, int m, double x, double y, double z) const;
      double DeltaJ(double aJot1, double aJot2, double aJot) const;
      /**
       *
       * @param aJot1
       * @param aEm1
       * @param aJot2
       * @param aEm2
       * @param aJot
       * @param aEm
       * @return Clebsch-Gordan coefficients
       */
      double ClebschGordan(double aJot1, double aEm1, double aJot2, double aEm2, double aJot, double aEm) const;
      double WignerSymbol(double aJot1, double aEm1, double aJot2, double aEm2, double aJot, double aEm) const;
      ClassDef(YlmMath, 1)
    };
  }  // namespace Sh
}  // namespace Hal

#endif /* HAL_FEATURES_MATH_SHMATH_H_ */
