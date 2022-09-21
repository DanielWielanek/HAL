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
#include "Package.h"
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

  //========================= Z vertex  cut ===========================================

  EventVertexZCut::EventVertexZCut() : EventCut(1) { SetUnitName("V_{z} [cm]"); }

  Bool_t EventVertexZCut::Pass(Event* event) {
    SetValue(event->GetVertex()->Z());
    return Validate();
  }
  //=========================XYZ vertex cut========================================

  EventVertexXYZCut::EventVertexXYZCut() : EventCut(4), fShift(TVector3(0, 0, 0)) {
    SetUnitName("V_{xy} [cm]", Rt());
    SetUnitName("V_{x} [cm]", X());
    SetUnitName("V_{y} [cm]", Y());
    SetUnitName("V_{z} [cm]", Z());
  }

  void EventVertexXYZCut::SetShift(const TVector3& vec) { fShift = vec; }

  Bool_t EventVertexXYZCut::Pass(Event* event) {
    Double_t x = event->GetVertex()->X() - fShift.X();
    Double_t y = event->GetVertex()->Y() - fShift.Y();
    Double_t z = event->GetVertex()->Z() - fShift.Z();
    SetValue(TMath::Sqrt(x * x + y * y), Rt());
    SetValue(x, X());
    SetValue(y, Y());
    SetValue(z, Z());
    return Validate();
  }

  Package* EventVertexXYZCut::Report() const {
    Package* report = EventCut::Report();
    report->AddObject(fShift.Clone());
    return report;
  }

}  // namespace Hal
