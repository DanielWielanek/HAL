/*
 * EventPtMaxCut.cxx
 *
 *  Created on: 11-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "EventPtMaxCut.h"

#include "Event.h"

namespace Hal {
  EventPtMaxCut::EventPtMaxCut() : EventCut(1) { SetUnitName("Max p_{t} [GeV/c]"); }

  Bool_t EventPtMaxCut::Init(Int_t task_id) {
    Bool_t stat = EventCut::Init(task_id);
    return stat;
  }

  EventPtMaxCut::~EventPtMaxCut() {}

  Bool_t EventPtMaxCut::Pass(Event* event) {
    Double_t pt_max = 0;
    for (int i = 0; i < event->GetTotalTrackNo(); i++) {
      Track* track = event->GetTrack(i);
      Double_t px  = track->GetPx();
      Double_t py  = track->GetPy();
      if ((px * px + py * py) > pt_max) { pt_max = px * px + py * py; }
    }
    SetValue(TMath::Sqrt(pt_max), 0);
    return Validate();
  }
}  // namespace Hal
