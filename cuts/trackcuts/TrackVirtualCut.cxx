/*
 * TrackVirtualCut.cxx
 *
 *  Created on: 07-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackVirtualCut.h"

#include "Track.h"
namespace Hal {
  TrackVirtualCut::TrackVirtualCut() : TrackCut(1) { SetUnitName("Virtual Unit [VU]"); }

  TrackVirtualCut::~TrackVirtualCut() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
