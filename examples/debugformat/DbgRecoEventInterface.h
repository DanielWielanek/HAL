/*
 * DbgRecoEventInterface.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_DBGRECOEVENTINTERFACE_H_
#define HAL_EXAMPLES_ONTHEFLY_DBGRECOEVENTINTERFACE_H_

#include "DbgData.h"
#include "EventInterface.h"

namespace HalDbg {

  class RecoEventInterface : public Hal::EventInterface {
    Dbg::RecoEvent* fEvent;
    Bool_t fCanDelete;

  protected:
    void ConnectToTreeInternal(eMode mode);

  public:
    RecoEventInterface();
    Dbg::RecoEvent* GetEvent() const { return fEvent; }
    virtual ~RecoEventInterface();
    ClassDef(RecoEventInterface, 1)
  };
}  // namespace HalDbg

#endif /* HAL_EXAMPLES_ONTHEFLY_DBGRECOEVENTINTERFACE_H_ */
