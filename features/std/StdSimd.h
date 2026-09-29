/*
 * StdSimd.h
 *
 *  Created on: 21 wrz 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_STD_STDSIMD_H_
#define HAL_FEATURES_STD_STDSIMD_H_

/**
 * temporary interface for SIMD in C++17 up to 23
 */

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <numeric>
#include <type_traits>


#if defined(HAL_SIMD_FORCE_SCALAR)

#define HAL_SIMD_BACKEND_SCALAR

#elif defined(HAL_SIMD_FORCE_EXPERIMENTAL)

#define HAL_SIMD_BACKEND_EXPERIMENTAL

#elif defined(HAL_SIMD_FORCE_STD)

#define HAL_SIMD_BACKEND_STD

#elif defined(__has_include)

#if __has_include(<simd>) && (__cplusplus >= 202302L)

#define HAL_SIMD_BACKEND_STD

#elif __has_include(<experimental/simd>)

#define HAL_SIMD_BACKEND_EXPERIMENTAL

#else

#define HAL_SIMD_BACKEND_SCALAR

#endif

#else

#define HAL_SIMD_BACKEND_SCALAR

#endif


#if defined(HAL_SIMD_BACKEND_STD)

#include <simd>

#endif


#if defined(HAL_SIMD_BACKEND_EXPERIMENTAL)

#include <experimental/simd>

#endif


namespace Hal {
  namespace Simd {

    // ============================================================================
    // STD SIMD
    // ============================================================================

#if defined(HAL_SIMD_BACKEND_STD)


    template<typename T>
    using Vec = std::native_simd<T>;

    template<typename T>
    using Mask = std::native_simd_mask<T>;

    template<typename T>
    inline constexpr std::size_t size_v = Vec<T>::size();


    // ----------------------------------------------------------------------------
    // construction
    // ----------------------------------------------------------------------------

    template<typename T>
    inline Vec<T> set(T value) {
      return Vec<T>(value);
    }

    template<typename T>
    inline Vec<T> zero() {
      return Vec<T>(T {});
    }


    // ----------------------------------------------------------------------------
    // load / store
    // ----------------------------------------------------------------------------

    template<typename T>
    inline Vec<T> load(const T* ptr) {
      Vec<T> result;
      result.copy_from(ptr, std::element_aligned);
      return result;
    }

    template<typename T>
    inline void store(const Vec<T>& value, T* ptr) {
      value.copy_to(ptr, std::element_aligned);
    }


    // ----------------------------------------------------------------------------
    // arithmetic
    // ----------------------------------------------------------------------------

    template<typename T>
    inline Vec<T> fmadd(const Vec<T>& a, const Vec<T>& b, const Vec<T>& c) {
      return std::fma(a, b, c);
    }


    // ----------------------------------------------------------------------------
    // math
    // ----------------------------------------------------------------------------

    template<typename T>
    inline Vec<T> abs(const Vec<T>& x) {
      return std::abs(x);
    }

    template<typename T>
    inline Vec<T> sqrt(const Vec<T>& x) {
      return std::sqrt(x);
    }

    template<typename T>
    inline Vec<T> exp(const Vec<T>& x) {
      return std::exp(x);
    }

    template<typename T>
    inline Vec<T> min(const Vec<T>& a, const Vec<T>& b) {
      return std::min(a, b);
    }

    template<typename T>
    inline Vec<T> max(const Vec<T>& a, const Vec<T>& b) {
      return std::max(a, b);
    }


    // ----------------------------------------------------------------------------
    // comparisons
    // ----------------------------------------------------------------------------

    template<typename T>
    inline Mask<T> equal(const Vec<T>& a, const Vec<T>& b) {
      return a == b;
    }

    template<typename T>
    inline Mask<T> not_equal(const Vec<T>& a, const Vec<T>& b) {
      return a != b;
    }

    template<typename T>
    inline Mask<T> less(const Vec<T>& a, const Vec<T>& b) {
      return a < b;
    }

    template<typename T>
    inline Mask<T> less_equal(const Vec<T>& a, const Vec<T>& b) {
      return a <= b;
    }

    template<typename T>
    inline Mask<T> greater(const Vec<T>& a, const Vec<T>& b) {
      return a > b;
    }

    template<typename T>
    inline Mask<T> greater_equal(const Vec<T>& a, const Vec<T>& b) {
      return a >= b;
    }


    // ----------------------------------------------------------------------------
    // mask
    // ----------------------------------------------------------------------------

    template<typename T>
    inline bool any(const Mask<T>& mask) {
      for (std::size_t i = 0; i < size_v<T>; ++i)
        if (mask[i]) return true;

      return false;
    }

    template<typename T>
    inline bool all(const Mask<T>& mask) {
      for (std::size_t i = 0; i < size_v<T>; ++i)
        if (!mask[i]) return false;

      return true;
    }


    // ----------------------------------------------------------------------------
    // reduction
    // ----------------------------------------------------------------------------

    template<typename T>
    inline T reduce_add(const Vec<T>& x) {
      T result {};

      for (std::size_t i = 0; i < size_v<T>; ++i)
        result += x[i];

      return result;
    }

    template<typename T>
    inline T reduce_min(const Vec<T>& x) {
      T result = x[0];

      for (std::size_t i = 1; i < size_v<T>; ++i)
        result = std::min(result, x[i]);

      return result;
    }

    template<typename T>
    inline T reduce_max(const Vec<T>& x) {
      T result = x[0];

      for (std::size_t i = 1; i < size_v<T>; ++i)
        result = std::max(result, x[i]);

      return result;
    }


    // ============================================================================
    // EXPERIMENTAL SIMD
    // ============================================================================

#elif defined(HAL_SIMD_BACKEND_EXPERIMENTAL)


    template<typename T>
    using Vec = std::experimental::native_simd<T>;

    template<typename T>
    using Mask = std::experimental::native_simd_mask<T>;

    template<typename T>
    inline constexpr std::size_t size_v = Vec<T>::size();


    // ----------------------------------------------------------------------------
    // construction
    // ----------------------------------------------------------------------------

    template<typename T>
    inline Vec<T> set(T value) {
      return Vec<T>(value);
    }

    template<typename T>
    inline Vec<T> zero() {
      return Vec<T>(T {});
    }


    // ----------------------------------------------------------------------------
    // load / store
    // ----------------------------------------------------------------------------

    template<typename T>
    inline Vec<T> load(const T* ptr) {
      Vec<T> result;
      result.copy_from(ptr, std::experimental::element_aligned);

      return result;
    }

    template<typename T>
    inline void store(const Vec<T>& value, T* ptr) {
      value.copy_to(ptr, std::experimental::element_aligned);
    }


    // ----------------------------------------------------------------------------
    // arithmetic
    // ----------------------------------------------------------------------------

    template<typename T>
    inline Vec<T> fmadd(const Vec<T>& a, const Vec<T>& b, const Vec<T>& c) {
      return std::experimental::fma(a, b, c);
    }


    // ----------------------------------------------------------------------------
    // math
    // ----------------------------------------------------------------------------

    template<typename T>
    inline Vec<T> abs(const Vec<T>& x) {
      return std::experimental::abs(x);
    }

    template<typename T>
    inline Vec<T> sqrt(const Vec<T>& x) {
      return std::experimental::sqrt(x);
    }

    template<typename T>
    inline Vec<T> exp(const Vec<T>& x) {
      return std::experimental::exp(x);
    }

    template<typename T>
    inline Vec<T> min(const Vec<T>& a, const Vec<T>& b) {
      return std::experimental::min(a, b);
    }

    template<typename T>
    inline Vec<T> max(const Vec<T>& a, const Vec<T>& b) {
      return std::experimental::max(a, b);
    }


    // ----------------------------------------------------------------------------
    // comparisons
    // ----------------------------------------------------------------------------

    template<typename T>
    inline Mask<T> equal(const Vec<T>& a, const Vec<T>& b) {
      return a == b;
    }

    template<typename T>
    inline Mask<T> not_equal(const Vec<T>& a, const Vec<T>& b) {
      return a != b;
    }

    template<typename T>
    inline Mask<T> less(const Vec<T>& a, const Vec<T>& b) {
      return a < b;
    }

    template<typename T>
    inline Mask<T> less_equal(const Vec<T>& a, const Vec<T>& b) {
      return a <= b;
    }

    template<typename T>
    inline Mask<T> greater(const Vec<T>& a, const Vec<T>& b) {
      return a > b;
    }

    template<typename T>
    inline Mask<T> greater_equal(const Vec<T>& a, const Vec<T>& b) {
      return a >= b;
    }


    // ----------------------------------------------------------------------------
    // mask
    // ----------------------------------------------------------------------------

    template<typename T>
    inline bool any(const Mask<T>& mask) {
      for (std::size_t i = 0; i < size_v<T>; ++i)
        if (mask[i]) return true;

      return false;
    }

    template<typename T>
    inline bool all(const Mask<T>& mask) {
      for (std::size_t i = 0; i < size_v<T>; ++i)
        if (!mask[i]) return false;

      return true;
    }


    // ----------------------------------------------------------------------------
    // reduction
    // ----------------------------------------------------------------------------

    template<typename T>
    inline T reduce_add(const Vec<T>& x) {
      T result {};

      for (std::size_t i = 0; i < size_v<T>; ++i)
        result += x[i];

      return result;
    }

    template<typename T>
    inline T reduce_min(const Vec<T>& x) {
      T result = x[0];

      for (std::size_t i = 1; i < size_v<T>; ++i)
        result = std::min(result, x[i]);

      return result;
    }

    template<typename T>
    inline T reduce_max(const Vec<T>& x) {
      T result = x[0];

      for (std::size_t i = 1; i < size_v<T>; ++i)
        result = std::max(result, x[i]);

      return result;
    }


    // ============================================================================
    // SCALAR
    // ============================================================================

#else


    template<typename T>
    class Vec {
    public:
      using value_type = T;

      constexpr Vec() = default;

      constexpr Vec(T value) : fValue(value) {}

      constexpr T& operator[](std::size_t) { return fValue; }

      constexpr T operator[](std::size_t) const { return fValue; }

      constexpr Vec& operator+=(const Vec& other) {
        fValue += other.fValue;
        return *this;
      }

      constexpr Vec& operator-=(const Vec& other) {
        fValue -= other.fValue;
        return *this;
      }

      constexpr Vec& operator*=(const Vec& other) {
        fValue *= other.fValue;
        return *this;
      }

      constexpr Vec& operator/=(const Vec& other) {
        fValue /= other.fValue;
        return *this;
      }

      constexpr T value() const { return fValue; }

    private:
      T fValue {};
    };


    template<typename T>
    using Mask = bool;


    template<typename T>
    inline constexpr std::size_t size_v = 1;


    // ----------------------------------------------------------------------------
    // operators
    // ----------------------------------------------------------------------------

    template<typename T>
    constexpr Vec<T> operator+(Vec<T> a, const Vec<T>& b) {
      return a += b;
    }

    template<typename T>
    constexpr Vec<T> operator-(Vec<T> a, const Vec<T>& b) {
      return a -= b;
    }

    template<typename T>
    constexpr Vec<T> operator*(Vec<T> a, const Vec<T>& b) {
      return a *= b;
    }

    template<typename T>
    constexpr Vec<T> operator/(Vec<T> a, const Vec<T>& b) {
      return a /= b;
    }

    template<typename T>
    constexpr bool operator==(const Vec<T>& a, const Vec<T>& b) {
      return a.value() == b.value();
    }

    template<typename T>
    constexpr bool operator!=(const Vec<T>& a, const Vec<T>& b) {
      return a.value() != b.value();
    }

    template<typename T>
    constexpr bool operator<(const Vec<T>& a, const Vec<T>& b) {
      return a.value() < b.value();
    }

    template<typename T>
    constexpr bool operator<=(const Vec<T>& a, const Vec<T>& b) {
      return a.value() <= b.value();
    }

    template<typename T>
    constexpr bool operator>(const Vec<T>& a, const Vec<T>& b) {
      return a.value() > b.value();
    }

    template<typename T>
    constexpr bool operator>=(const Vec<T>& a, const Vec<T>& b) {
      return a.value() >= b.value();
    }


    // ----------------------------------------------------------------------------
    // construction
    // ----------------------------------------------------------------------------

    template<typename T>
    constexpr Vec<T> set(T value) {
      return Vec<T>(value);
    }

    template<typename T>
    constexpr Vec<T> zero() {
      return Vec<T>(T {});
    }


    // ----------------------------------------------------------------------------
    // load / store
    // ----------------------------------------------------------------------------

    template<typename T>
    inline Vec<T> load(const T* ptr) {
      return Vec<T>(*ptr);
    }

    template<typename T>
    inline void store(const Vec<T>& value, T* ptr) {
      *ptr = value.value();
    }


    // ----------------------------------------------------------------------------
    // arithmetic
    // ----------------------------------------------------------------------------

    template<typename T>
    constexpr Vec<T> fmadd(const Vec<T>& a, const Vec<T>& b, const Vec<T>& c) {
      return a * b + c;
    }


    // ----------------------------------------------------------------------------
    // math
    // ----------------------------------------------------------------------------

    template<typename T>
    inline Vec<T> abs(const Vec<T>& x) {
      return Vec<T>(std::abs(x.value()));
    }

    template<typename T>
    inline Vec<T> sqrt(const Vec<T>& x) {
      return Vec<T>(std::sqrt(x.value()));
    }

    template<typename T>
    inline Vec<T> exp(const Vec<T>& x) {
      return Vec<T>(std::exp(x.value()));
    }

    template<typename T>
    constexpr Vec<T> min(const Vec<T>& a, const Vec<T>& b) {
      return Vec<T>(a.value() < b.value() ? a.value() : b.value());
    }

    template<typename T>
    constexpr Vec<T> max(const Vec<T>& a, const Vec<T>& b) {
      return Vec<T>(a.value() > b.value() ? a.value() : b.value());
    }


    // ----------------------------------------------------------------------------
    // comparisons
    // ----------------------------------------------------------------------------

    template<typename T>
    constexpr Mask<T> equal(const Vec<T>& a, const Vec<T>& b) {
      return a == b;
    }

    template<typename T>
    constexpr Mask<T> not_equal(const Vec<T>& a, const Vec<T>& b) {
      return a != b;
    }

    template<typename T>
    constexpr Mask<T> less(const Vec<T>& a, const Vec<T>& b) {
      return a < b;
    }

    template<typename T>
    constexpr Mask<T> less_equal(const Vec<T>& a, const Vec<T>& b) {
      return a <= b;
    }

    template<typename T>
    constexpr Mask<T> greater(const Vec<T>& a, const Vec<T>& b) {
      return a > b;
    }

    template<typename T>
    constexpr Mask<T> greater_equal(const Vec<T>& a, const Vec<T>& b) {
      return a >= b;
    }


    // ----------------------------------------------------------------------------
    // mask
    // ----------------------------------------------------------------------------

    inline constexpr bool any(bool mask) { return mask; }

    inline constexpr bool all(bool mask) { return mask; }


    // ----------------------------------------------------------------------------
    // reduction
    // ----------------------------------------------------------------------------

    template<typename T>
    constexpr T reduce_add(const Vec<T>& x) {
      return x.value();
    }

    template<typename T>
    constexpr T reduce_min(const Vec<T>& x) {
      return x.value();
    }

    template<typename T>
    constexpr T reduce_max(const Vec<T>& x) {
      return x.value();
    }


#endif


    // ============================================================================
    // Common API
    // ============================================================================

    template<typename T>
    inline constexpr std::size_t width() {
      return size_v<T>;
    }
    inline constexpr std::size_t Float_size() {
      // return 4;
      return size_v<float>;
    }
    inline constexpr std::size_t Double_size() { return size_v<double>; }
    inline constexpr std::size_t Int_size() { return size_v<int>; }
  }  // namespace Simd


  typedef Simd::Vec<float> Float_v;
  typedef Simd::Vec<int> Int_v;
  typedef Simd::Vec<double> Double_v;

}  // namespace Hal


#endif /* HAL_FEATURES_STD_STDSIMD_H_ */
