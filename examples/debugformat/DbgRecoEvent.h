/*
 * DbgRecoEvent.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_DBGRECOEVENT_H_
#define HAL_EXAMPLES_ONTHEFLY_DBGRECOEVENT_H_

#include "ExpEvent.h"
#include "ExpTrack.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>

namespace HalDbg {
  class RecoEvent : public Hal::ExpEvent {
  public:
    RecoEvent();
    Hal::EventInterface* CreateInterface() const;
    virtual void Update(Hal::EventInterface* interface);
    virtual Bool_t ExistInTree() const;
    virtual TString GetFormatName() const { return "DbgRecoEvent"; };
    virtual ~RecoEvent() {};
    ClassDef(RecoEvent, 1)
  };

  class RecoTrack : public Hal::ExpTrack {
    Int_t fMatchIdx;

  public:
    RecoTrack() : fMatchIdx(-1) {};
    Int_t GetMatch() const { return fMatchIdx; };
    void SetMatch(Int_t idx) { fMatchIdx = idx; };
    virtual ~RecoTrack() {};
    ClassDef(RecoTrack, 1);
  };

}  // namespace HalDbg
#endif /* HAL_EXAMPLES_ONTHEFLY_DBGRECOEVENT_H_ */
