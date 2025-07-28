/*
 * MonitorTof.cxx
 *
 *  Created on: 9 lip 2025
 *      Author: daniel
 */

#include "MonitorTof.h"

#include <RtypesCore.h>
#include <TH2.h>

#include "ComplexTrack.h"
#include "DataFormat.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "ExpTrack.h"
#include "Std.h"

namespace Hal {
  MonitorTofM2::MonitorTofM2() {
    SetXaxis(200, -2, 2);
    SetYaxis(200, -0.5, 2.5);
    fFieldId1 = Hal::DataFieldID::Track::EBasic::kPq;
    fFieldId2 = Hal::DataFieldID::Track::EExp::kTofM2;
  }

  MonitorTofBeta::MonitorTofBeta() {
    SetXaxis(200, -2, 2);
    SetYaxis(200, -0.25, 1.25);
    fFieldId1 = Hal::DataFieldID::Track::EBasic::kPq;
    fFieldId2 = Hal::DataFieldID::Track::EExp::kToFBeta;
  }
}  // namespace Hal
