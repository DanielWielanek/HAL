/*
 * EventCutTempates.h
 *
 *  Created on: 10 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_TEMPLATES_EVENTCUTTEMPATE_H_
#define HAL_TEMPLATES_EVENTCUTTEMPATE_H_

#include <Hal/EventCut.h>

namespace MyHal {
  class Hal::Package;
  class EventCutTempate : public Hal::EventCut {
  public:
    EventCutTempate();
    Int_t Init(Int_t taskID);
    Bool_t Pass(Hal::Event* pair);
    Hal::Package* Report() const;
    virtual ~EventCutTempate();
    ClassDef(EventCutTempate, 1)
  };

}  // namespace MyHal

#endif /* HAL_TEMPLATES_EVENTCUTTEMPATE_H_ */
