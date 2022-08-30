/*
 * EventPtMaxCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALEVENTPTMAXCUT_H_
#define HALEVENTPTMAXCUT_H_

#include "EventCut.h"
/**
 * check biggest track pt in event
 */

namespace Hal {
  class EventPtMaxCut : public EventCut {
  public:
    EventPtMaxCut();
    virtual Bool_t Pass(Event* event);
    virtual Bool_t Init(Int_t task_id = 0);
    virtual ~EventPtMaxCut();
    ClassDef(EventPtMaxCut, 1)
  };
}  // namespace Hal
#endif /* HALEVENTPTMAXCUT_H_ */
