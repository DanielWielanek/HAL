/*
 * BoostTask.h
 *
 *  Created on: 23 lip 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALBOOSTTASK_H_
#define HALBOOSTTASK_H_

#include "EventAna.h"

#include <Rtypes.h>
#include <RtypesCore.h>

namespace Hal {
  class EventInterface;
  class TrackInterface;

  /**
   * basic class for boosting  events, currently boost only oryginal event in tree
   */
  class BoostTask : public EventAna {
    Double_t fBoostVx;
    Double_t fBoostVy;
    Double_t fBoostVz;
    EventInterface* fEventInterface;
    TrackInterface* fTrackInterface;

  protected:
    Task::EInitFlag Init();
    virtual Package* Report() const;

  public:
    /**
     * default ctor
     */
    BoostTask();
    /**
     * constructor
     * @param vx x boost component
     * @param vy y boost component
     * @param vz z boost component
     */
    BoostTask(Double_t vx, Double_t vy, Double_t vz);
    /**
     * constructor
     * @param vz x boost component
     */
    BoostTask(Double_t vz);
    /**
     * set boost direction
     * @param vx x component of velocity
     * @param vy y component of velocity
     * @param vz z component of velocity
     * @param opt not used
     */
    void SetBoost(Double_t vx, Double_t vy, Double_t vz, Option_t* opt = "");
    /**
     *
     * @param vz  z component of velocity
     * @param opt not used
     */
    void SetBoost(Double_t vz, Option_t* opt = "");
    void Exec(Option_t* opt);
    virtual ~BoostTask();
    ClassDef(BoostTask, 1)
  };
}  // namespace Hal

#endif /* HALBOOSTTASK_H_ */
