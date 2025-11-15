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
  MonitorTofM2::MonitorTofM2() :
    TrackFieldMonitorXY(Hal::DataFieldID::Track::EBasic::kPq, Hal::DataFieldID::Track::EExp::kTofM2) {
    SetXaxis(200, -2, 2);
    SetYaxis(200, -0.5, 2.5);
  }

  MonitorTofBeta::MonitorTofBeta() :
    TrackFieldMonitorXY(Hal::DataFieldID::Track::EBasic::kPq, Hal::DataFieldID::Track::EExp::kToFBeta) {
    SetXaxis(200, -2, 2);
    SetYaxis(200, -0.25, 1.25);
  }
}  // namespace Hal
