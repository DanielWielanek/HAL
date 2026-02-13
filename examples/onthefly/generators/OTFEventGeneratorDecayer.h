/*
 * OTFGeneratorDecayer.h
 *
 *  Created on: 18 mar 2025
 *      Author: daniel
 */

#ifndef HAL_EXAMPLES_ONTHEFLY_OTFGENERATORDECAYER_H_
#define HAL_EXAMPLES_ONTHEFLY_OTFGENERATORDECAYER_H_

#include <RtypesCore.h>

#include "Decay.h"
#include "OTFEventGenerator.h"

namespace HalOTF {
  /**
   * class that decays all particles in given event according to given channel
   */
  class EventGeneratorDecayer : public EventGenerator {
  protected:
    Hal::Decay* fDecayer = {nullptr};
    std::vector<Hal::McTrack*> fDaughters;
    virtual void GenerateEvent();

  public:
    EventGeneratorDecayer() {}
    void SetDecay(Hal::Decay decay);
    virtual Bool_t Init();
    virtual ~EventGeneratorDecayer();
    ClassDef(EventGeneratorDecayer, 1)
  };


} /* namespace HalOTF */

#endif /* HAL_EXAMPLES_ONTHEFLY_OTFGENERATORDECAYER_H_ */
