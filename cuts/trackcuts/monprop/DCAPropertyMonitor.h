/*
 * DCAPropertyMonitor.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_CUTS_TRACKCUTS_PROPERTIES_HALDCAPROPERTYMONITOR_H_
#define HALFEMTO_CUTS_TRACKCUTS_PROPERTIES_HALDCAPROPERTYMONITOR_H_

#include "PropertyMonitorXY.h"

namespace Hal {

  class DCAPropertyMonitor : public TrackFieldMonitorXY {
  public:
    DCAPropertyMonitor(Bool_t complex = kFALSE);
    virtual CutMonitor* MakeCopy() const { return new DCAPropertyMonitor(*this); }
    virtual ~DCAPropertyMonitor();
    ClassDef(DCAPropertyMonitor, 1)
  };
}  // namespace Hal

#endif /* HALFEMTO_CUTS_TRACKCUTS_PROPERTIES_HALDCAPROPERTYMONITOR_H_ */
