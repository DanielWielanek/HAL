/*
 * OTFReaderDecay.h
 *
 *  Created on: 28 mar 2024
 *      Author: daniel
 */

#ifndef HAL_EXAMPLES_ONTHEFLY_READERDECAY_H_
#define HAL_EXAMPLES_ONTHEFLY_READERDECAY_H_
#include "Decay.h"
#include "OTFReader.h"

#include "OTFData.h"

#include <vector>

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TH2.h>

namespace HalOTF {
  class McEvent;
  class RecoEvent;
} /* namespace HalOTF */

namespace HalOTF {

  class ReaderDecay : public HalOTF::Reader {
  protected:
    Hal::Decay* fDecayer = {nullptr};
    std::vector<Hal::McTrack*> fDaughters;
    virtual void Decay();
    Bool_t fDebug = {kFALSE};
    virtual void GenerateEvent();

  public:
    ReaderDecay();
    void EnableDebug() { fDebug = kTRUE; }
    void SetDecay(Hal::Decay decay);
    virtual Hal::Task::EInitFlag Init();
    virtual ~ReaderDecay();
    ClassDef(ReaderDecay, 1)
  };
}  // namespace HalOTF

#endif /* HAL_EXAMPLES_ONTHEFLY_READERDECAY_H_ */
