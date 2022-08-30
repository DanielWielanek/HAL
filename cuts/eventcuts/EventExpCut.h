/*
 * EventExpCut.h
 *
 *  Created on: 30 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_CUTS_EVENTCUTS_DETECTOR_EVENTEXPCUT_H_
#define HAL_CUTS_EVENTCUTS_DETECTOR_EVENTEXPCUT_H_

#include "EventCut.h"
namespace Hal {

  class EventExpCut : public EventCut {
  public:
    EventExpCut(const Int_t size = 1);
    virtual Bool_t Init(Int_t format_id);
    virtual ~EventExpCut() {};
    ClassDef(EventExpCut, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_EVENTCUTS_DETECTOR_EVENTEXPCUT_H_ */
