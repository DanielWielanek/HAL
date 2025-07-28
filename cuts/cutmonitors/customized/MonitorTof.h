/*
 * MonitorTof.h
 *
 *  Created on: 9 lip 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITORTOF_H_
#define HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITORTOF_H_

#include "CustomMonitors2D.h"

namespace Hal {
  class MonitorTofM2 : public CustomTrackMonitors2D {

  public:
    MonitorTofM2();
    virtual ~MonitorTofM2() {};
    ClassDef(MonitorTofM2, 1)
  };
  class MonitorTofBeta : public CustomTrackMonitors2D {
  public:
    MonitorTofBeta();
    virtual ~MonitorTofBeta() {};
    ClassDef(MonitorTofBeta, 1)
  };
}  // namespace Hal

#endif /* HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITORTOF_H_ */
