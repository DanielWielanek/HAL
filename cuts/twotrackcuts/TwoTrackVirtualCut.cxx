/*
 * TwoTrackVirtualCut.cpp
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TwoTrackVirtualCut.h"

#include "TwoTrack.h"
namespace Hal {
  TwoTrackVirtualCut::TwoTrackVirtualCut() : TwoTrackCut(1) { SetUnitName("Virtual Unit [VU]"); }

  TwoTrackVirtualCut::~TwoTrackVirtualCut() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
