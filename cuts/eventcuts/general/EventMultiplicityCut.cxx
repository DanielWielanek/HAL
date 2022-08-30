/*
 * EventMultiplicityCut.cxx
 *
 *  Created on: 28-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "EventMultiplicityCut.h"

#include "Event.h"
#include "Package.h"
#include "Parameter.h"

namespace Hal {
  EventMultiplicityCut::EventMultiplicityCut() : EventCut(1) { SetUnitName("Multiplicity [N_{particles}]"); }

  EventMultiplicityCut::~EventMultiplicityCut() {
    // TODO Auto-generated destructor stub
  }

  Bool_t EventMultiplicityCut::Pass(Event* event) {
    SetValue(event->GetTotalTrackNo());
    return Validate();
  }

  EventMultiChargedCut::EventMultiChargedCut() : EventCut(1), fScale(1), fMinEta(-1), fMaxEta(1), fRange(2), fPDG(NULL) {
    SetUnitName("dN_{ch}/d#eta");
  }

  Bool_t EventMultiChargedCut::Pass(Event* event) {
    Double_t charged_no = 0;
    for (int i = 0; i < event->GetTotalTrackNo(); i++) {
      Track* track = event->GetTrack(i);
      Double_t eta = track->GetMomentum().Eta();
      if (eta >= fMinEta && eta <= fMaxEta) {
        if (track->GetCharge() != 0) charged_no++;
      }
    }
    Double_t value = charged_no * fScale;  // *fScale/fRange
    SetValue(value);
    return Validate();
  }

  void EventMultiChargedCut::SetAveragingRange(Double_t min, Double_t max) {
    fMinEta = min;
    fMaxEta = max;
  }

  void EventMultiChargedCut::SetEfficiencyScale(Double_t scale) { fScale = scale; }

  Bool_t EventMultiChargedCut::Init(Int_t task_id) {
    Bool_t stat = EventCut::Init(task_id);
    fPDG        = TDatabasePDG::Instance();
    fRange      = fMaxEta - fMinEta;
    fScale      = fScale / fRange;
    return stat;
  }

  EventMultiChargedCut::~EventMultiChargedCut() {
    // TODO Auto-generated destructor stub
  }

  Package* EventMultiChargedCut::Report() const {
    Package* pack = EventCut::Report();
    pack->AddObject(new ParameterDouble("Av. Min Eta", fMinEta));
    pack->AddObject(new ParameterDouble("Av. Max Eta", fMaxEta));
    pack->AddObject(new ParameterDouble("Raw Scale", fScale * fRange));
    return pack;
  }
}  // namespace Hal
