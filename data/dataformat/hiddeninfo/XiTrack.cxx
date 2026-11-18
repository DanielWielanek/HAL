/*
 * XiTrack.cpp
 *
 *  Created on: 17 cze 2024
 *      Author: daniel
 */

#include "XiTrack.h"

namespace Hal {

  void XiTrack::CopyData(HiddenTrackInfo* vx) {
    HiddenTrackInfo::CopyData(vx);
    auto v              = (XiTrack*) vx;
    fAssumedPdg         = v->fAssumedPdg;
    fAssumedBachelorPdg = v->fAssumedBachelorPdg;
    fDecayPoint         = v->fDecayPoint;
  }

  void XiTrack2::CopyData(HiddenTrackInfo* other) {
    HiddenTrackInfo::CopyData(other);
    auto v        = (XiTrack2*) other;
    fAssumedV0Pdg = v->fAssumedV0Pdg;
  }


  void XiTrack3::CopyData(HiddenTrackInfo* other) {
    HiddenTrackInfo::CopyData(other);
    auto v                    = (XiTrack3*) other;
    fAssumedGranddaughter1pid = v->fAssumedGranddaughter1pid;
    fAssumedGranddaughter2pid = v->fAssumedGranddaughter2pid;
  }

}  // namespace Hal
