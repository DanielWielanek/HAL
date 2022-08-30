/*
 * EventPhiCut.h
 *
 *  Created on: 04-05-2022
 *      Author: wielanek
 */

#ifndef HALEVENTPHICUT_H_
#define HALEVENTPHICUT_H_

#include "EventCut.h"
/**
 * check reaction plane angle
 */

namespace Hal {
  class EventPhiCut : public EventCut {
  protected:
  public:
    EventPhiCut();
    virtual Bool_t Pass(Event* event);
    virtual ~EventPhiCut();
    ClassDef(EventPhiCut, 1)
  };
}  // namespace Hal

#endif /* HALEVENTPHICUT_H_ */
