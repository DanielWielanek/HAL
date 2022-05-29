/*
 * OTFComplexEvent.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_OTFCOMPLEXEVENT_H_
#define HAL_EXAMPLES_ONTHEFLY_OTFCOMPLEXEVENT_H_

#include "ComplexEvent.h"

namespace HalOTF {
  class ComplexEvent : public Hal::ComplexEvent {
  public:
    ComplexEvent();
    virtual void Update();
    virtual TString GetFormatName() const { return "OTFComplexEvent"; };
    virtual ~ComplexEvent() {};
    ClassDef(ComplexEvent, 1);
  };
}  // namespace HalOTF
#endif /* HAL_EXAMPLES_ONTHEFLY_OTFCOMPLEXEVENT_H_ */
