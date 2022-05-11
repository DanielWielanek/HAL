/*
 * TrackStartPositionCut.cxx
 *
 *  Created on: 4 lip 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackStartPositionCut.h"

#include "McTrack.h"
#include "DataFormatManager.h"

namespace Hal{

TrackStartPositionCut::TrackStartPositionCut() : TrackMCCut(4) {
  SetUnitName("StartX [cm]", 0);
  SetUnitName("StartY [cm]", 1);
  SetUnitName("StartZ [cm]", 2);
  SetUnitName("StartY [ns]", 3);
}

Bool_t TrackStartPositionCut::Pass(Track* track) {
  McTrack* mc = (McTrack*) track;
  SetValue(mc->GetStartPosition().X(), 0);
  SetValue(mc->GetStartPosition().Y(), 1);
  SetValue(mc->GetStartPosition().Z(), 2);
  SetValue(mc->GetStartPosition().T(), 3);
  return Validate();
}

TrackStartPositionCut::~TrackStartPositionCut() {}
}
