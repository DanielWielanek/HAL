/*
 * DCAPropertyMonitor.cxx
 *
 *  Created on: 5 cze 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "DCAPropertyMonitor.h"

#include "DataFormat.h"

namespace Hal {

  DCAPropertyMonitor::DCAPropertyMonitor() :
    TrackFieldMonitorXY(DataFieldID::Track::EExp::kDcaZ, DataFieldID::Track::EExp::kDcaXY) {}

  DCAPropertyMonitor::~DCAPropertyMonitor() {}
}  // namespace Hal
