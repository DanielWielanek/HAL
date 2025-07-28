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

  MonitorDcaRZ::MonitorDcaRZ() {
    SetXaxis(200, -2, 2);
    SetYaxis(200, 0, 4);
    fFieldId1 = Hal::DataFieldID::Track::EExp::kDcaZ;
    fFieldId2 = Hal::DataFieldID::Track::EExp::kDcaXY;
  }

  MonitorDcaXY::MonitorDcaXY() {
    SetXaxis(200, -2, 2);
    SetYaxis(200, -2, 2);
    fFieldId1 = Hal::DataFieldID::Track::EExp::kDcaX;
    fFieldId2 = Hal::DataFieldID::Track::EExp::kDcaY;
  }

}  // namespace Hal
