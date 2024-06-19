/*
 * XiTrack.cpp
 *
 *  Created on: 17 cze 2024
 *      Author: daniel
 */

#include "XiTrack.h"

namespace Hal {

  void XiTrack::CopyData(XiTrack* v) {
    fV0Id              = v->fV0Id;
    fTrackId           = v->fTrackId;
    fChargedId         = v->fChargedId;
    fAssumedPdg        = v->fAssumedPdg;
    fAssumedPdgNeu     = v->fAssumedPdgNeu;
    fAssumedPdgCharged = v->fAssumedPdgCharged;
    fDecay             = v->fDecay;
  }
}  // namespace Hal
