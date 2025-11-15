/*
 * MonitorVertex.cxx
 *
 *  Created on: 28 lip 2025
 *      Author: daniel
 */

#include "MonitorEventVertex.h"
#include "DataFormat.h"

namespace Hal {

  MonitorEventVertexXY::MonitorEventVertexXY() :
    EventFieldMonitorXY(Hal::DataFieldID::Event::EBasic::kVertexX, Hal::DataFieldID::Event::EBasic::kVertexY) {}

  MonitorEventVertexRZ::MonitorEventVertexRZ() :
    EventFieldMonitorXY(Hal::DataFieldID::Event::EBasic::kVertexZ, Hal::DataFieldID::Event::EBasic::kVertexXY) {}

} /* namespace Hal */
