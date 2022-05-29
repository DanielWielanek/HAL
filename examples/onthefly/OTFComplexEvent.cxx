/*
 * OTFComplexEvent.cxx
 *
 *  Created on: 28 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "OTFComplexEvent.h"
#include "ComplexTrack.h"
#include "OTFMcEvent.h"
#include "OTFRecoEvent.h"
#include "OTFRecoEventInterface.h"

namespace HalOTF {
  ComplexEvent::ComplexEvent() : Hal::ComplexEvent(new HalOTF::RecoEvent(), new HalOTF::McEvent) {}

  void ComplexEvent::Update() {
    fImgEvent->Update();
    fRealEvent->Update();
    Hal::Event::ShallowCopyEvent(fRealEvent);
    fTracks->Clear();
    fTotalTracksNo = fRealEvent->GetTotalTrackNo();
    fTracks->ExpandCreateFast(fTotalTracksNo);
    RecoEventInterface* s = (RecoEventInterface*) fSource;
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

}  // namespace HalOTF
