/*
 * EventSmeared.cxx
 *
 *  Created on: 20 lis 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "SmearedEvent.h"
#include "ComplexTrack.h"


#include <TClonesArray.h>
#include <TObjArray.h>
#include <TString.h>

#include "Event.h"
#include "McTrack.h"
#include "VirtualEvent.h"

namespace Hal {
  SmearedEvent::SmearedEvent() : ComplexEvent("SmearedTrack", NULL, NULL), fSmearing(kFALSE), fRealMC(kFALSE) {}

  void SmearedEvent::Clear(Option_t* opt) { ComplexEvent::Clear(opt); }

  TString SmearedEvent::GetFormatName() const { return fRealEvent->GetFormatName() + "_smeared"; }

  SmearedEvent::SmearedEvent(Event* event) :
    ComplexEvent("SmearedTrack", event->GetNewEvent(), new VirtualEvent()), fSmearing(kFALSE) {
    if (event->InheritsFrom("Hal::MCEvent")) {
      fRealMC = kTRUE;
    } else {
      fRealMC = kFALSE;
    }
  }

  SmearedEvent::SmearedEvent(const SmearedEvent& other) :
    ComplexEvent(other), fSmearing(other.fSmearing), fRealMC(other.fRealMC) {}

  void SmearedEvent::Update() {
    if (fSmearing) {  // we need to update real event and copy data to imaginary
                      // event
      fRealEvent->Update();
      CopyData(fRealEvent);
      fImgEvent->Clear();
      fImgEvent->ShallowCopyEvent(fRealEvent);
      fTracks->Clear();
      if (fRealEvent->GetTotalTrackNo()) {
        fTotalTracksNo = fRealEvent->GetTotalTrackNo();
        for (int i = 0; i < fTotalTracksNo; i++) {
          Track* tr        = (Track*) fRealEvent->fTracks->UncheckedAt(i);
          ComplexTrack* to = (ComplexTrack*) fTracks->ConstructedAt(i);
          if (fRealMC) {
            ((McTrack*) to)->McTrack::CopyData(tr);
          } else {
            to->Track::CopyData(tr);
          }
          to->SetImgTrack((Track*) ((VirtualEvent*) fImgEvent)->GetTrackSafely(i));
          to->SetRealTrack(tr);
        }
      }
    } else {  // each event is separately updated
      ComplexEvent::Update();
    }
  }

  void SmearedEvent::LinkWithTree() {
    if (fImgEvent->ExistInTree()) {
      fImgEvent->LinkWithTree();
    } else {
      //	fImgEvent->RegisterInTree(kFALSE);
    }
    fRealEvent->LinkWithTree();
  }

  SmearedEvent::~SmearedEvent() {}
}  // namespace Hal
