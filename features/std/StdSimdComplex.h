/*
 * StdSimdComplex.h
 *
 *  Created on: 21 wrz 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_STD_STDSIMDCOMPLEX_H_
#define HAL_FEATURES_STD_STDSIMDCOMPLEX_H_

#include <TObject.h>

#include "StdSimd.h"

namespace Hal {

  namespace Simd {

    template<typename T>
    class Complex {
    public:
      T fRe;
      T fIm;

      Complex() = default;

      Complex(const T& re, const T& im) : fRe(re), fIm(im) {}

      // np. Complex<Float_v>(2.0f)
      explicit Complex(const T& re) : fRe(re), fIm(T(0)) {}

      T& re() { return fRe; }
      const T& re() const { return fRe; }

      T& im() { return fIm; }
      const T& im() const { return fIm; }

      Complex& operator+=(const Complex& other) {
        fRe += other.fRe;
        fIm += other.fIm;
        return *this;
      }

      Complex& operator-=(const Complex& other) {
        fRe -= other.fRe;
        fIm -= other.fIm;
        return *this;
      }

      Complex& operator*=(const Complex& other) {
        const T re = fRe * other.fRe - fIm * other.fIm;
        const T im = fRe * other.fIm + fIm * other.fRe;

        fRe = re;
        fIm = im;
        return *this;
      }

      Complex& operator/=(const Complex& other) {
        const T den = other.fRe * other.fRe + other.fIm * other.fIm;

        const T re = (fRe * other.fRe + fIm * other.fIm) / den;
        const T im = (fIm * other.fRe - fRe * other.fIm) / den;

        fRe = re;
        fIm = im;
        return *this;
      }

      Complex operator*(const T& other) const { return {fRe * other, fIm * other}; }
      Complex operator/(const T& other) const { return {fRe / other, fIm / other}; }

      Complex operator-() const { return {-fRe, -fIm}; }
      Complex conj() const { return {fRe, -fIm}; }
      virtual ~Complex() {};
      // ClassDef(Complex, 0)
    };


    template<typename T>
    Complex<T> operator+(Complex<T> a, const Complex<T>& b) {
      a += b;
      return a;
    }

    template<typename T>
    Complex<T> operator-(Complex<T> a, const Complex<T>& b) {
      a -= b;
      return a;
    }

    template<typename T>
    Complex<T> operator*(Complex<T> a, const Complex<T>& b) {
      a *= b;
      return a;
    }

    template<typename T>
    Complex<T> operator/(Complex<T> a, const Complex<T>& b) {
      a /= b;
      return a;
    }

  }  // namespace Simd


  typedef Simd::Complex<Simd::Vec<float>> FComplex_v;
  typedef Simd::Complex<Simd::Vec<double>> DComplex_v;

} /* namespace Hal */

#endif /* HAL_FEATURES_STD_STDSIMDCOMPLEX_H_ */
