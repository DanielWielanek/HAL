/*
 * EventVertexCut.cxx
 *
 *  Created on: 4 cze 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "EventVertexCut.h"

#include "DataFormatManager.h"
#include "ExpEvent.h"
#include <TLorentzVector.h>

namespace Hal {
  EventVertexCut::EventVertexCut() : EventCut(2) {
    SetUnitName("Vertex R_{t} [cm]", Rt());
    SetUnitName("Vertex Z [cm]", Z());
  }

  Bool_t EventVertexCut::Pass(Event* event) {
    Event* ev                 = (Event*) event;
    const TLorentzVector* pos = ev->GetVertex();
    Double_t x                = pos->X();
    Double_t y                = pos->Y();
    SetValue(TMath::Sqrt(x * x + y * y), Rt());
    SetValue(pos->Z(), Z());
    return Validate();
  }

  Bool_t EventVertexCut::Init(Int_t /*format_id*/) { return kTRUE; }

  EventVertexCut::~EventVertexCut() {}
}  // namespace Hal
