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

  TofPropertyMonitor::TofPropertyMonitor(Bool_t complex) :
    TrackFieldMonitorXY((complex ? DataFieldID::ReStep + DataFieldID::Track::EBasic::kPq : DataFieldID::Track::EBasic::kPq),
                        (complex ? DataFieldID::ReStep + DataFieldID::Track::EExp::kTofM2 : DataFieldID::Track::EExp::kTofM2)) {}

  TofPropertyMonitor::~TofPropertyMonitor() {}
}  // namespace Hal
