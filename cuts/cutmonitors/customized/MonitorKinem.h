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
  /**
   * monitor to plot rapidity vs pt - reconstructed, requires complex format
   */
  class MonitorYPtReco : public TrackFieldMonitorXY {
  public:
    MonitorYPtReco();
    virtual ~MonitorYPtReco() {};
    ClassDef(MonitorYPtReco, 1)
  };
  /**
   * monitor to plot pseudorapidity vs pt - reconstructed, requires complex format
   */
  class MonitorEtaPtReco : public TrackFieldMonitorXY {
  public:
    MonitorEtaPtReco();
    virtual ~MonitorEtaPtReco() {};
    ClassDef(MonitorEtaPtReco, 1)
  };

  /**
   * monitor to plot rapidity vs pt - simulated, requires complex format
   */
  class MonitorYPtMc : public TrackFieldMonitorXY {
  public:
    MonitorYPtMc();
    virtual ~MonitorYPtMc() {};
    ClassDef(MonitorYPtMc, 1)
  };
  /**
   * monitor to plot pseudorapidity vs pt - simulated, requires complex format
   */
  class MonitorEtaPtMc : public TrackFieldMonitorXY {
  public:
    MonitorEtaPtMc();
    virtual ~MonitorEtaPtMc() {};
    ClassDef(MonitorEtaPtMc, 1)
  };
} /* namespace Hal */

#endif /* HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITORTRACKKINEM_H_ */
