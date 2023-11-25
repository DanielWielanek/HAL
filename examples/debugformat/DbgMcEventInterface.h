/*
 * DbgMcEventInterface.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_DBGMCEVENTINTERFACE_H_
#define HAL_EXAMPLES_ONTHEFLY_DBGMCEVENTINTERFACE_H_

#include "EventInterface.h"
#include "DbgData.h"
namespace HalDbg {
  class McEventInterface : public Hal::EventInterface {
    Dbg::McEvent* fEvent;
    Bool_t fCanDelete;

  protected:
    virtual void ConnectToTreeInternal(Hal::EventInterface::eMode mode);

  public:
    McEventInterface();
    Dbg::McEvent* GetMcEvent() const { return fEvent; }
    virtual ~McEventInterface();
    ClassDef(McEventInterface, 1)
  };
}  // namespace HalDbg

#endif /* HAL_EXAMPLES_ONTHEFLY_DBGMCEVENTINTERFACE_H_ */
