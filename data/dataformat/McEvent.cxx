/*
 * MCEvent.cxx
 *
 *  Created on: 1 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "McEvent.h"
#include "DataFormat.h"
#include "EventInterfaceAdvanced.h"
#include "McTrack.h"
#include "McTrackInterface.h"
namespace Hal {
  McEvent::McEvent() : Event(), fB(0) {}

  McEvent::McEvent(TString track_classname, TString v0_classname) : Event(track_classname, v0_classname), fB(0) {}

  void McEvent::ShallowCopyEvent(Event* event) {
    Event::ShallowCopyEvent(event);
    if (event->InheritsFrom("Hal::McEvent")) fB = ((McEvent*) event)->GetImpactParameter();
  }

  McEvent::~McEvent() {}

  McEvent::McEvent(const McEvent& other) : Event(other) { fB = other.fB; }

  void McEvent::ShallowCopyTracks(Event* event) {
    fTracks->Clear();
    McEvent* mc_event = (McEvent*) event;
    fTotalTracksNo    = mc_event->fTracks->GetEntriesFast();
    for (int i = 0; i < fTotalTracksNo; i++) {
      McTrack* to   = (McTrack*) fTracks->ConstructedAt(i);
      McTrack* from = (McTrack*) mc_event->fTracks->UncheckedAt(i);
      to->ResetTrack(i, this);
      to->CopyData(from);
    }
  }

  void McEvent::Update() {
    Event::Update();
    EventInterfaceAdvanced* source = dynamic_cast<EventInterfaceAdvanced*>(fSource);
    if (source) {
      McTrackInterface* interface = (McTrackInterface*) source->GetTrackInterface();
      for (int i = 0; i < fTotalTracksNo; i++) {
        McTrack* tr = (McTrack*) GetTrack(i);
        tr->SetPdg(interface->GetPdg());
        tr->SetStartPosition(interface->GetStartX(), interface->GetStartY(), interface->GetStartZ(), interface->GetStartT());
        tr->SetFreezoutPosition(interface->GetFreezX(), interface->GetFreezY(), interface->GetFreezZ(), interface->GetFreezT());
      }
      delete interface;
    }
  }

  Float_t McEvent::GetFieldVal(Int_t fieldID) const {
    switch (fieldID) {
      case DataFieldID::EMcEvent::kB: return GetImpactParameter(); break;
      default: return Event::GetFieldVal(fieldID); break;
    }
  }

  TString McEvent::GetFieldName(Int_t fieldID) const {
    switch (fieldID) {
      case DataFieldID::EMcEvent::kB: return "B [fm]"; break;
      default: return Event::GetFieldName(fieldID); break;
    }
  }
}  // namespace Hal
