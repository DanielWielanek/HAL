/*
 * PidHypo.cpp
 *
 *  Created on: 14 lut 2026
 *      Author: daniel
 */

#include <RtypesCore.h>

#include "Cut.h"
#include "DataFormatManager.h"
#include "ExpTrack.h"
#include "OTFPidHypoCut.h"
#include "OTFRecoEvent.h"
#include "StdTypes.h"

namespace HalOTF {

  PidHypoCut::PidHypoCut() : Hal::TrackExpCut(1) { SetUnitName("PidHypo [AU]"); }

  Bool_t PidHypoCut::Init(Int_t taskId) {
    const Hal::Event* ev = Hal::DataFormatManager::Instance()->GetFormat(taskId, Hal::EFormatDepth::kNonBuffered);
    if (dynamic_cast<const HalOTF::RecoEvent*>(ev)) { return kTRUE; }
    return kTRUE;
  }

  Bool_t PidHypoCut::Pass(Hal::Track* track) {
    auto tr = (HalOTF::RecoTrack*) track;
    SetValue(tr->GetPidHypo());
    return Validate();
  }

  PidHypoCut::~PidHypoCut() {}


} /* namespace HalOTF */
