/*
 * OnTheFlyMcEvent.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_DBGMCEVENT_H_
#define HAL_EXAMPLES_ONTHEFLY_DBGMCEVENT_H_

#include "McEvent.h"
#include "McTrack.h"

namespace HalDbg {
  class McEvent : public Hal::McEvent {
  public:
    McEvent();
    Hal::EventInterface* CreateInterface() const;
    virtual void Update(Hal::EventInterface* interface);
    virtual Bool_t ExistInTree() const;
    virtual TString GetFormatName() const { return "DbgMcEvent"; };
    virtual ~McEvent();
    ClassDef(McEvent, 1)
  };

  class McTrack : public Hal::McTrack {
  public:
    McTrack() {};
    virtual ~McTrack() {};
    ClassDef(McTrack, 1)
  };

}  // namespace HalDbg
#endif /* HAL_EXAMPLES_ONTHEFLY_DBGMCEVENT_H_ */
