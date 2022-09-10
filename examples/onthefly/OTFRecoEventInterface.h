/*
 * OTFRecoEventInterface.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_OTFRECOEVENTINTERFACE_H_
#define HAL_EXAMPLES_ONTHEFLY_OTFRECOEVENTINTERFACE_H_

#include "EventInterface.h"
#include "OTFData.h"

namespace HalOTF {

  class RecoEventInterface : public Hal::EventInterface {
    OTF::RecoEvent* fEvent;
    Bool_t fCanDelete;

  protected:
    void ConnectToTreeInternal(eMode mode);

  public:
    RecoEventInterface();
    OTF::RecoEvent* GetEvent() const { return fEvent; }
    virtual ~RecoEventInterface();
    ClassDef(RecoEventInterface, 1)
  };
}  // namespace HalOTF

#endif /* HAL_EXAMPLES_ONTHEFLY_OTFRECOEVENTINTERFACE_H_ */
