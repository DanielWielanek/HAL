/*
 * DbgComplexEvent.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_DBGCOMPLEXEVENT_H_
#define HAL_EXAMPLES_ONTHEFLY_DBGCOMPLEXEVENT_H_

#include "ComplexEvent.h"

namespace HalDbg {
  class ComplexEvent : public Hal::ComplexEvent {
  public:
    ComplexEvent();
    virtual void Update(Hal::EventInterface* interface);
    virtual TString GetFormatName() const { return "DbgComplexEvent"; };
    virtual ~ComplexEvent() {};
    ClassDef(ComplexEvent, 1);
  };
}  // namespace HalDbg
#endif /* HAL_EXAMPLES_ONTHEFLY_DBGCOMPLEXEVENT_H_ */
