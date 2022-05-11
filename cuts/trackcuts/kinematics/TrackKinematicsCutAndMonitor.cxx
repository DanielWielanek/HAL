/*
 * TrackKinematicsCutAndMonitor.cxx
 *
 *  Created on: 7 maj 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "TrackKinematicsCutAndMonitor.h"
#include "TrackEtaCut.h"
#include "TrackPtCut.h"

namespace Hal {

  TrackKinematicsCutAndMonitor::TrackKinematicsCutAndMonitor() :
    fEtaAxis("TrackPtCut", 0, 100, 0, 4), fPtAxis("TrackEtaCut", 0, 100, -4, 4) {
    AddCut(TrackPtCut());
    AddCut(TrackEtaCut());
  }

  TrackKinematicsCutAndMonitor::~TrackKinematicsCutAndMonitor() {
    // TODO Auto-generated destructor stub
  }

  void TrackKinematicsCutAndMonitor::AddAllCutMonitorRequests() { AddCutMonitorRequest(fEtaAxis, fPtAxis); }
}  // namespace Hal
