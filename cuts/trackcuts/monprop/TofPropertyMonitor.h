/*
 * TofPropertyMonitor.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_CUTS_TRACKCUTS_PROPERTIES_HALTOFPROPERTYMONITOR_H_
#define HALFEMTO_CUTS_TRACKCUTS_PROPERTIES_HALTOFPROPERTYMONITOR_H_

#include "PropertyMonitorXY.h"

namespace Hal {

  class TofM2Monitor : public TrackFieldMonitorXY {
  public:
    TofM2Monitor();
    virtual ~TofM2Monitor() {};
    ClassDef(TofM2Monitor, 1)
  };

  class TofBetaMonitor : public TrackFieldMonitorXY {
  public:
    TofBetaMonitor();
    virtual ~TofBetaMonitor() {};
    ClassDef(TofBetaMonitor, 1)
  };

}  // namespace Hal

#endif /* HALFEMTO_CUTS_TRACKCUTS_PROPERTIES_HALTOFPROPERTYMONITOR_H_ */
