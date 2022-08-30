/*
 * EventImpactParameterCut.cxx
 *
 *  Created on: 15-11-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "EventImpactParameterCut.h"

#include "Cout.h"
#include "DataFormatManager.h"
#include "McEvent.h"

namespace Hal {
  EventImpactParameterCut::EventImpactParameterCut() : EventMcCut(1) { SetUnitName("Impact Parameter [fm]"); }

  Bool_t EventImpactParameterCut::Pass(Event* event) {
    SetValue(((McEvent*) event)->GetImpactParameter());
    return Validate();
  }

  EventImpactParameterCut::~EventImpactParameterCut() {}

  EventImpactCentralityCut::EventImpactCentralityCut() : EventMcCut(1), fBMax(-1), fAlpha(0) { SetUnitName("Centrality [%]"); }

  void EventImpactCentralityCut::SetBMax(Double_t bmax) { fBMax = bmax; }

  Bool_t EventImpactCentralityCut::Pass(Event* event) {
    SetValue(ImpactToCentrality(((McEvent*) event)->GetImpactParameter()));
    return Validate();
  }

  Double_t EventImpactCentralityCut::ImpactToCentrality(const Double_t impact) { return impact * impact * fAlpha; }

  Bool_t EventImpactCentralityCut::Init(Int_t task_id) {
    Bool_t stat     = EventCut::Init(task_id);
    const Event* ev = DataFormatManager::Instance()->GetFormat(task_id);
    if (!ev->InheritsFrom("Hal::McEvent")) { return kFALSE; }
    if (fBMax < 0) {
      Cout::PrintInfo("Max impact  parameter in EventImpactCentralityCut is "
                      "<0 setting to 0",
                      EInfo::kLowWarning);
      fBMax  = 0;
      fAlpha = 1.0;
    } else {
      fAlpha = 100.0 / (fBMax * fBMax);
    }
    return stat;
  }

  EventImpactCentralityCut::~EventImpactCentralityCut() {}
}  // namespace Hal
