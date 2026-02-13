/*
 * OTFRecoEvent.h
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_EXAMPLES_ONTHEFLY_OTFRECOEVENT_H_
#define HAL_EXAMPLES_ONTHEFLY_OTFRECOEVENT_H_

#include "ExpEvent.h"
#include "ExpTrack.h"

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>

namespace HalOTF {
  class RecoEvent : public Hal::ExpEvent {
  public:
    RecoEvent();
    Hal::EventInterface* CreateInterface() const;
    virtual void Update(Hal::EventInterface* interface);
    virtual Bool_t ExistInTree() const;
    virtual TString GetFormatName() const { return "OTFRecoEvent"; };
    virtual ~RecoEvent() {};
    ClassDef(RecoEvent, 1)
  };

  class RecoTrack : public Hal::ExpTrack {
    Int_t fMatchIdx;
    Int_t fGeneratorId = {-1};
    Int_t fPidHypo     = {-1};
    Int_t fNHitsA      = {0};
    Int_t fNHitsB      = {0};
    Int_t fNHitsC      = {0};

  public:
    RecoTrack() : fMatchIdx(-1) {};
    Int_t GetMatch() const { return fMatchIdx; };
    Int_t GetGeneratorId() const { return fGeneratorId; }
    Int_t GetPidHypo() const { return fPidHypo; }
    Int_t GetNHitsA() const { return fNHitsA; }
    Int_t GetNHitsB() const { return fNHitsB; }
    Int_t GetNHitsC() const { return fNHitsC; }
    void SetNHitsA(Int_t hits) { fNHitsA = hits; };
    void SetNHitsB(Int_t hits) { fNHitsB = hits; };
    void SetNHitsC(Int_t hits) { fNHitsC = hits; };
    void SetGeneratorId(Int_t id) { fGeneratorId = id; }
    void SetMatch(Int_t idx) { fMatchIdx = idx; };
    void SetPidHypo(Int_t pid) { fPidHypo = pid; }
    virtual ~RecoTrack() {};
    ClassDef(RecoTrack, 1);
  };

}  // namespace HalOTF
#endif /* HAL_EXAMPLES_ONTHEFLY_OTFRECOEVENT_H_ */
