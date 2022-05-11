/*
 * EventVirtualCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALEVENTCUTVIRTUAL_H_
#define HALEVENTCUTVIRTUAL_H_

#include "EventCut.h"
/**
 * virtual cut that pass all events  and doesn't compute any values
 */
namespace Hal {
  class EventVirtualCut : public EventCut {
  public:
    /**
     * default constructor
     * @param i - number of monitored parameters
     */
    EventVirtualCut();
    virtual Bool_t Pass(Event* /*event*/) {
      ForcedUpdate(kTRUE);
      return kTRUE;
    };
    virtual ~EventVirtualCut();
    ClassDef(EventVirtualCut, 1)
  };
}  // namespace Hal

#endif /* HALEVENTCUTVIRTUAL_H_ */
