/*
 * OTFMcEventInterface.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_OTFMCEVENTINTERFACE_H_
#define HAL_EXAMPLES_ONTHEFLY_OTFMCEVENTINTERFACE_H_

#include "EventInterface.h"
#include "OTFData.h"
namespace HalOTF {
  class McEventInterface : public Hal::EventInterface {
    OTF::McEvent* fEvent;
    Bool_t fCanDelete;

  protected:
    virtual void ConnectToTreeInternal(Hal::EventInterface::eMode mode);

  public:
    McEventInterface();
    OTF::McEvent* GetMcEvent() const { return fEvent; }
    virtual ~McEventInterface();
    ClassDef(McEventInterface, 1)
  };
}  // namespace HalOTF

#endif /* HAL_EXAMPLES_ONTHEFLY_OTFMCEVENTINTERFACE_H_ */
