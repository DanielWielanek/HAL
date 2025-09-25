/*
 * TofPropertyMonitor.cxx
 *
 *  Created on: 5 cze 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TofPropertyMonitor.h"

#include "DataFormat.h"

namespace Hal {

  TofPropertyMonitor::TofPropertyMonitor() :
    TrackFieldMonitorXY(DataFieldID::Track::EBasic::kPq, DataFieldID::Track::EExp::kTofM2) {}

  TofPropertyMonitor::~TofPropertyMonitor() {}
}  // namespace Hal
