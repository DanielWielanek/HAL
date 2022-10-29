/*
 * EventMultiplicityVertexCut.h
 *
 *  Created on: 29 paź 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_CUTS_EVENTCUTS_GENERAL_EVENTMULTIPLICITYZVERTEXCUT_H_
#define HAL_CUTS_EVENTCUTS_GENERAL_EVENTMULTIPLICITYZVERTEXCUT_H_

#include "EventCut.h"

namespace Hal {

  class EventMultiplicityZVertexCut : public EventCut {
  public:
    EventMultiplicityZVertexCut();
    virtual Bool_t Pass(Event* event);
    virtual ~EventMultiplicityZVertexCut();
    ClassDef(EventMultiplicityZVertexCut, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_EVENTCUTS_GENERAL_EVENTMULTIPLICITYZVERTEXCUT_H_ */
