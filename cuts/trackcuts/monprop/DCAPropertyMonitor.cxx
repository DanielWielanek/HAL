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

  DCAPropertyMonitor::DCAPropertyMonitor(Bool_t complex) :
    TrackFieldMonitorXY((complex ? DataFieldID::ReStep + DataFieldID::Track::EExp::kDcaZ : DataFieldID::Track::EExp::kDcaZ),
                        (complex ? DataFieldID::ReStep + DataFieldID::Track::EExp::kDcaXY : DataFieldID::Track::EExp::kDcaXY)) {}

  DCAPropertyMonitor::~DCAPropertyMonitor() {}
}  // namespace Hal
