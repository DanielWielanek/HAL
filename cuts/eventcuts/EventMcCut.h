/*
 * EventMcCut.h
 *
 *  Created on: 30 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_CUTS_EVENTCUTS_EVENTMCCUT_H_
#define HAL_CUTS_EVENTCUTS_EVENTMCCUT_H_

#include "EventCut.h"

/**
 * base class for all mc-event based cuts, require to pass mc-event
 */
namespace Hal {

  class EventMcCut : public EventCut {
  public:
    EventMcCut(const Int_t size = 1);
    virtual Bool_t Init(Int_t format_id);
    virtual ~EventMcCut() {};
    ClassDef(EventMcCut, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_EVENTCUTS_EVENTMCCUT_H_ */
