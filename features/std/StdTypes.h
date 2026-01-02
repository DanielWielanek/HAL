/*
 * StdTypes.h
 *
 *  Created on: 16 gru 2025
 *      Author: daniel
 */

#ifndef HAL_FEATURES_STD_STDTYPES_H_
#define HAL_FEATURES_STD_STDTYPES_H_

#include <complex>

#include <TString.h>
// some enums used almost everywhere

namespace Hal {
  enum class EInfo { kDebugInfo = 0, kInfo = 1, kLowWarning = 2, kWarning = 3, kError = 4, kCriticalError = 5 };
  enum class EFormatType { kSim = 0, kReco = 1, kComplexReco = 2, kComplexSim = 3, kUnknown = 4 };

  enum class ERound { kSeparator, kPrefix };
  enum class ECutUpdate { kNo = -1, kEvent = 0, kTrack = 1, kTwoTrack = 2, kTwoTrackBackground = 3 };

  enum class EFormatDepth { kAll, kBuffered, kNonBuffered };

}  // namespace Hal

namespace Hal {
  namespace Std {
    template<typename T>
    struct Triplet {
      T x, y, z;
    };
    typedef std::complex<double> DComplex;
    /**
     * conver enum to "human readable" update ratio description
     * @param upd
     * @return
     */
    TString UpdateEnumToString(Hal::ECutUpdate upd);
  }  // namespace Std
}  // namespace Hal
#endif /* HAL_FEATURES_STD_STDTYPES_H_ */
