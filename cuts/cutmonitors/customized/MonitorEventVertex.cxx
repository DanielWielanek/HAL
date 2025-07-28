/*
 * MonitorVertex.cxx
 *
 *  Created on: 28 lip 2025
 *      Author: daniel
 */

#include "MonitorEventVertex.h"
#include "DataFormat.h"

namespace Hal {

  MonitorEventVertexXY::MonitorEventVertexXY() {
    fFieldId1 = Hal::DataFieldID::Event::EBasic::kVertexX;
    fFieldId2 = Hal::DataFieldID::Event::EBasic::kVertexY;
  }

  MonitorEventVertexRZ::MonitorEventVertexRZ() {
    fFieldId1 = Hal::DataFieldID::Event::EBasic::kVertexZ;
    fFieldId2 = Hal::DataFieldID::Event::EBasic::kVertexXY;
  }

} /* namespace Hal */
