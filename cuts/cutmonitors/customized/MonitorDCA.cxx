/*
 * MonitorDCA.cxx
 *
 *  Created on: 18 lip 2025
 *      Author: daniel
 */

#include "MonitorDCA.h"

#include <TH2D.h>

#include "ComplexTrack.h"
#include "DataFormat.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "ExpTrack.h"


namespace Hal {

  MonitorDcaRZ::MonitorDcaRZ() :
    TrackFieldMonitorXY(Hal::DataFieldID::Track::EExp::kDcaZ, Hal::DataFieldID::Track::EExp::kDcaXY) {
    SetXaxis(200, -2, 2);
    SetYaxis(200, 0, 4);
  }

  MonitorDcaXY::MonitorDcaXY() : TrackFieldMonitorXY(Hal::DataFieldID::Track::EExp::kDcaX, Hal::DataFieldID::Track::EExp::kDcaY) {
    SetXaxis(200, -2, 2);
    SetYaxis(200, -2, 2);
  }

}  // namespace Hal
