/*
 * TwoTrackCut.cxx
 *
 *  Created on: 06-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TwoTrackCut.h"

#include "TwoTrack.h"

namespace Hal {
  TwoTrackCut::TwoTrackCut(const Int_t i) : Cut(i, ECutUpdate::kTwoTrack, "N_{pairs}") {}

  TwoTrackCut::~TwoTrackCut() {
    // TODO Auto-generated destructor stub
  }

  Package* TwoTrackCut::Report() const {
    Package* pack = Cut::Report();
    return pack;
  }
}  // namespace Hal
