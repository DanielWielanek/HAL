/*
 * MonitorDCA.h
 *
 *  Created on: 18 lip 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITORDCA_H_
#define HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITORDCA_H_

#include "CustomMonitors2D.h"

namespace Hal {

  class MonitorDcaRZ : public CustomTrackMonitors2D {

  public:
    MonitorDcaRZ();
    virtual ~MonitorDcaRZ() {};
    ClassDef(MonitorDcaRZ, 1)
  };
  class MonitorDcaXY : public CustomTrackMonitors2D {

  public:
    MonitorDcaXY();
    virtual ~MonitorDcaXY() {};
    ClassDef(MonitorDcaXY, 1)
  };
} /* namespace Hal */

#endif /* HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITORDCA_H_ */
