/*
 * TrackCut.cpp
 *
 *  Created on: 21-06-2013
 *      Author: Daniel Wielanek
 */

#include "TrackCut.h"

#include <TString.h>

#include "CutOptions.h"
#include "Std.h"
#include "TrackComplexCut.h"
#include "TrackVirtualCut.h"

namespace Hal {
  TrackCut::TrackCut(const Int_t i) : Cut(i, ECutUpdate::kTrack, "N_{tracks}") {
    // fUnitName = " ";
  }

  TrackCut::~TrackCut() {
    // TODO Auto-generated destructor stub
  }

  Package* TrackCut::Report() const {
    Package* pack = Cut::Report();
    return pack;
  }
}  // namespace Hal
