/*
 * TrackKinematicsCutAndMonitor.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_CUTS_TRACKCUTS_KINEMATICS_HALTRACKKINEMATICSCUTANDMONITOR_H_
#define HALFEMTO_CUTS_TRACKCUTS_KINEMATICS_HALTRACKKINEMATICSCUTANDMONITOR_H_

/**
 * example of usage cut and cut monitors
 */
#include "CutsAndMonitors.h"
#include "TrackCut.h"

namespace Hal {
  class TrackKinematicsCutAndMonitor : public CutsAndMonitors {
    CutMonAxisConf fEtaAxis;
    CutMonAxisConf fPtAxis;

  protected:
    void AddAllCutMonitorRequests();

  public:
    TrackKinematicsCutAndMonitor();
    TrackCut* GetPtCut() const { return static_cast<TrackCut*>(CutAt(0)); };
    TrackCut* GetEtaCut() const { return static_cast<TrackCut*>(CutAt(1)); };
    void SetCutMonPtAxis(Int_t bins, Double_t min, Double_t max) { fPtAxis.SetAxis(bins, min, max); };
    void SetCutMonEtaAxis(Int_t bins, Double_t min, Double_t max) { fEtaAxis.SetAxis(bins, min, max); };
    virtual ~TrackKinematicsCutAndMonitor();
    ClassDef(TrackKinematicsCutAndMonitor, 1)
  };
}  // namespace Hal

#endif /* HALFEMTO_CUTS_TRACKCUTS_KINEMATICS_HALTRACKKINEMATICSCUTANDMONITOR_H_ */
