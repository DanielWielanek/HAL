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

namespace Hal{

class TofPropertyMonitor : public TrackFieldMonitorXY {
public:
  TofPropertyMonitor(Bool_t complex = kFALSE);
  virtual CutMonitor* MakeCopy() const { return new TofPropertyMonitor(*this); }
  virtual ~TofPropertyMonitor();
  ClassDef(TofPropertyMonitor, 1)
};
}

#endif /* HALFEMTO_CUTS_TRACKCUTS_PROPERTIES_HALTOFPROPERTYMONITOR_H_ */
