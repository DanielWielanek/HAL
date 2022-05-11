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

namespace Hal{

DCAPropertyMonitor::DCAPropertyMonitor(Bool_t complex) :
  TrackFieldMonitorXY(
    (complex ? DataFieldID::ReStep + DataFieldID::EExpTrack::kDcaZ : DataFieldID::EExpTrack::kDcaZ),
    (complex ? DataFieldID::ReStep + DataFieldID::EExpTrack::kDcaXY : DataFieldID::EExpTrack::kDcaXY)) {}

DCAPropertyMonitor::~DCAPropertyMonitor() {}
}
