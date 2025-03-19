/*
 * OTFReaderDecay.h
 *
 *  Created on: 28 mar 2024
 *      Author: daniel
 */

#ifndef HAL_EXAMPLES_ONTHEFLY_GENERATORDECAY_H_
#define HAL_EXAMPLES_ONTHEFLY_GENERATORDECAY_H_
#include "Decay.h"
#include "OTFReader.h"

#include "OTFData.h"

#include <vector>

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH2.h>

#include "OTFEventGenerator.h"

namespace HalOTF {
  class McEvent;
  class RecoEvent;
} /* namespace HalOTF */

namespace HalOTF {
  /**
   *  class that represents a single type of particles that decays into something
   */
  class EventGeneratorDecay : public HalOTF::EventGenerator {
  protected:
    Hal::Decay* fDecayer = {nullptr};
    std::vector<Hal::McTrack*> fDaughters;
    virtual void Decay();
    Bool_t fDebug = {kFALSE};
    virtual void GenerateEvent();

  public:
    EventGeneratorDecay();
    void EnableDebug() { fDebug = kTRUE; }
    void SetDecay(Hal::Decay decay);
    virtual Bool_t Init();
    virtual ~EventGeneratorDecay();
    ClassDef(EventGeneratorDecay, 1)
  };
}  // namespace HalOTF

#endif /* HAL_EXAMPLES_ONTHEFLY_READERDECAY_H_ */
