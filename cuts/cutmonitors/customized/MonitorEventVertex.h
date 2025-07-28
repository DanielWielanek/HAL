/*
 * MonitorVertex.h
 *
 *  Created on: 28 lip 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITOREVENTVERTEX_H_
#define HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITOREVENTVERTEX_H_

#include "CustomMonitors2D.h"

namespace Hal {

  class MonitorEventVertexXY : public CustomEventMonitors2D {
  public:
    MonitorEventVertexXY();
    virtual ~MonitorEventVertexXY() {};
    ClassDef(MonitorEventVertexXY, 1)
  };
  class MonitorEventVertexRZ : public CustomEventMonitors2D {
  public:
    MonitorEventVertexRZ();
    virtual ~MonitorEventVertexRZ() {};
    ClassDef(MonitorEventVertexRZ, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITOREVENTVERTEX_H_ */
