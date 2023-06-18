/*
 * EventRotate.h
 *
 *  Created on: 18 cze 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_BASE_ON_THE_FLY_EVENTROTATETASK_H_
#define HAL_ANALYSIS_BASE_ON_THE_FLY_EVENTROTATETASK_H_

#include "EventAna.h"

/**
 * randomly rotates event
 * NOTE: Rotates only in Z-plane!
 */
namespace Hal {
  class EventRotateTask : public EventAna {
    Double_t fPhiMin;
    Double_t fPhiMax;

  protected:
    virtual void ProcessEvent();
    virtual Hal::Package* Report() const;
    virtual void CheckCutContainerCollections();

  public:
    EventRotateTask();
    EventRotateTask& operator=(const EventRotateTask& other);
    void SetRotationRange(Double_t lo, Double_t hi) {
      fPhiMin = lo;
      fPhiMax = hi;
    }
    EventRotateTask(const EventRotateTask& other);
    virtual ~EventRotateTask();
    ClassDef(EventRotateTask, 1)
  };
}  // namespace Hal
#endif /* HAL_ANALYSIS_BASE_ON_THE_FLY_EVENTROTATETASK_H_ */
