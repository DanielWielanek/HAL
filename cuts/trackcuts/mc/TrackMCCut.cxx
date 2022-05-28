/*
 * TrackMCCut.cxx
 *
 *  Created on: 5 mar 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackMCCut.h"

#include "DataFormatManager.h"
#include "McTrack.h"

namespace Hal {

  TrackMCCut::TrackMCCut(const Int_t size) : TrackCut(size) {}

  Bool_t TrackMCCut::Init(Int_t task_id) {
    if (TrackCut::Init() == kFALSE) return kFALSE;
    if (FormatInhertis("Hal::McEvent", task_id)) { return kTRUE; }
    return kFALSE;
  }

  TrackMCCut::~TrackMCCut() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
