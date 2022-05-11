/*
 * EventPrimaryNoCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALEVENTPRIMARYTRACKSNOCUT_H_
#define HALEVENTPRIMARYTRACKSNOCUT_H_

#include "EventCut.h"

namespace Hal {
  class EventPrimaryTracksNoCut : public EventCut {
  public:
    EventPrimaryTracksNoCut();
    virtual Bool_t Pass(Event* event);
    virtual ~EventPrimaryTracksNoCut();
    ClassDef(EventPrimaryTracksNoCut, 1)
  };
}  // namespace Hal
#endif /* HALEVENTPRIMARYTRACKSNOCUT_H_ */
