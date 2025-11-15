/*
 * MonitorTrackKinem.h
 *
 *  Created on: 23 sty 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITORTRACKKINEM_H_
#define HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITORTRACKKINEM_H_

#include "PropertyMonitorXY.h"

namespace Hal {
  /**
   * monitor to plot rapidity vs pt
   */
  class MonitorYPt : public TrackFieldMonitorXY {
  public:
    MonitorYPt();
    virtual ~MonitorYPt() {};
    ClassDef(MonitorYPt, 1)
  };
  /**
   * monitor to plot pseudorapidity vs pt
   */
  class MonitorEtaPt : public TrackFieldMonitorXY {
  public:
    MonitorEtaPt();
    virtual ~MonitorEtaPt() {};
    ClassDef(MonitorEtaPt, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITORTRACKKINEM_H_ */
