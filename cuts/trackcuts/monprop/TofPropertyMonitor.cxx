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

  TofM2Monitor::TofM2Monitor() : TrackFieldMonitorXY(DataFieldID::Track::EBasic::kPq, DataFieldID::Track::EExp::kTofM2) {}

  TofBetaMonitor::TofBetaMonitor() : TrackFieldMonitorXY(DataFieldID::Track::EBasic::kPq, DataFieldID::Track::EExp::kToFBeta) {}

}  // namespace Hal
