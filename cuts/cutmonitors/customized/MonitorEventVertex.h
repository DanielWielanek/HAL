/*
 * MonitorVertex.h
 *
 *  Created on: 28 lip 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITOREVENTVERTEX_H_
#define HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITOREVENTVERTEX_H_

#include "PropertyMonitorXY.h"

namespace Hal {

  class MonitorEventVertexXY : public EventFieldMonitorXY {
  public:
    MonitorEventVertexXY();
    virtual ~MonitorEventVertexXY() {};
    ClassDef(MonitorEventVertexXY, 1)
  };
  class MonitorEventVertexRZ : public EventFieldMonitorXY {
  public:
    MonitorEventVertexRZ();
    virtual ~MonitorEventVertexRZ() {};
    ClassDef(MonitorEventVertexRZ, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITOREVENTVERTEX_H_ */
