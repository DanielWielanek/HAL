/*
 * TrackNullCut.cxx
 *
 *  Created on: 12 lis 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackNullCut.h"

#include "ComplexEvent.h"
#include "ComplexTrack.h"
#include "Cut.h"
#include "DataFormatManager.h"
#include "Event.h"

#include <RtypesCore.h>

namespace Hal {
  TrackNullCut::TrackNullCut() : TrackCut(1) { SetUnitName("ComplexVarStatus [AU]"); }

  Bool_t TrackNullCut::Pass(Track* track) {
    ComplexTrack* particle = (ComplexTrack*) track;
    if (particle->GetImgTrack() == nullptr) {
      SetValue(0);
    } else {
      SetValue(1);
    }
    return Validate();
  }

  TrackNullCut::~TrackNullCut() {}

  Bool_t TrackNullCut::Init(Int_t task_id) {
    DataFormatManager* manager = DataFormatManager::Instance();
    const Event* event         = manager->GetFormat(task_id);
    if (dynamic_cast<const ComplexEvent*>(event)) return kTRUE;
    return kFALSE;
  }
}  // namespace Hal
