/*
 * StdTypes.cxx
 *
 *  Created on: 16 gru 2025
 *      Author: daniel
 */

#include "StdTypes.h"


namespace Hal {
  namespace Std {
    TString UpdateEnumToString(Hal::ECutUpdate upd) {
      switch (upd) {
        case Hal::ECutUpdate::kNo: return ""; break;
        case Hal::ECutUpdate::kEvent: return "Event"; break;
        case Hal::ECutUpdate::kTrack: return "Track"; break;
        case Hal::ECutUpdate::kTwoTrack: return "TwoTrack"; break;
        case Hal::ECutUpdate::kTwoTrackBackground: return "TwoTrackBackground"; break;
        default: return ""; break;
      }
    }
  }  // namespace Std
}  // namespace Hal
