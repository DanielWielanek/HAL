/*
 * DbgComplexEvent.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "DbgComplexEvent.h"
#include "ComplexEventInterface.h"
#include "ComplexTrack.h"
#include "DbgMcEvent.h"
#include "DbgRecoEvent.h"
#include "DbgRecoEventInterface.h"

namespace HalDbg {
  ComplexEvent::ComplexEvent() : Hal::ComplexEvent(new HalDbg::RecoEvent(), new HalDbg::McEvent) {}

  void ComplexEvent::Update(Hal::EventInterface* interface) {
    Hal::ComplexEventInterface* source = (Hal::ComplexEventInterface*) interface;
    fImgEvent->Update(source->GetImag());
    fRealEvent->Update(source->GetReal());
    Hal::Event::ShallowCopyEvent(fRealEvent);
    fEventId = fRealEvent->GetEventID();
    fTracks->Clear();
    fTotalTracksNo = fRealEvent->GetTotalTrackNo();
    fTracks->ExpandCreateFast(fTotalTracksNo);
    for (Int_t i = 0; i < fTotalTracksNo; i++) {
      Hal::ComplexTrack* track = (Hal::ComplexTrack*) fTracks->UncheckedAt(i);
      track->ResetTrack(i, this);
      track->SetRealTrack(fRealEvent->GetTrack(i));
      track->Hal::Track::CopyData(fRealEvent->GetTrack(i));
      Int_t match = ((RecoTrack*) fRealEvent->GetTrack(i))->GetMatch();

      if (match < 0) {
        track->SetImgTrack(nullptr);
      } else {
        track->SetImgTrack(fImgEvent->GetTrack(match));
        track->SetMatchID(match);
      }
    }
  }

}  // namespace HalDbg
