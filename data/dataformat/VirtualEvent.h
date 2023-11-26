/*
 * EventVirtual.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALVIRTUALEVENT_H_
#define HALVIRTUALEVENT_H_

#include "Event.h"
#include "EventInterface.h"
#include "VirtualTrack.h"

#include <TClonesArray.h>
#include <TString.h>

namespace Hal {


  class VirtualEventInterface : public EventInterface {
  protected:
    virtual void ConnectToTreeInternal(eMode /*mode*/) {};

  public:
    VirtualEventInterface() {};
    virtual ~VirtualEventInterface() {};
    ClassDef(VirtualEventInterface, 1)
  };

  class VirtualEvent : public Event {
    Event* fVirtualEvent;

  public:
    VirtualEvent();
    VirtualEvent(const VirtualEvent& other);
    Hal::EventInterface* CreateInterface() const { return new VirtualEventInterface(); };
    void Update(Hal::EventInterface* interface);
    void RegisterInTree(TString prefix, Bool_t save = kTRUE);
    Bool_t ExistInTree() const;
    Track* GetTrackSafely(Int_t i) { return (Track*) fTracks->ConstructedAt(i); };
    Event* GetNewEvent() const { return new VirtualEvent(*this); };
    Track* GetNewTrack() const { return new VirtualTrack(); };
    virtual ~VirtualEvent();
    ClassDef(VirtualEvent, 1)
  };
}  // namespace Hal
#endif /* HALVIRTUALEVENT_H_ */
