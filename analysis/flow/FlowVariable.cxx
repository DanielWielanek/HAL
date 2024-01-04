/*
 * FlowVariable.cxx
 *
 *  Created on: 19-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FlowVariable.h"

#include "DataFormatManager.h"
#include "Event.h"
#include "Track.h"


namespace Hal {
  FlowVariable::FlowVariable(TString axisName, TString axisUnit) : fName(axisName), fUnit(axisUnit) {}

  TString FlowVariable::GetAxisName() { return fName; }

  Double_t FlowVariable::GetVariable(Track* /*p*/) { return 0.; }

  FlowVariable::~FlowVariable() {
    // TODO Auto-generated destructor stub
  }

  TString FlowVariable::GetAxisUnit() { return fUnit; }

  FlowVirtualVariable::FlowVirtualVariable() : FlowVariable("virtual", "virtual") {}

  FlowVirtualVariable::~FlowVirtualVariable() {}

  FlowTrackFieldVariable::FlowTrackFieldVariable(Int_t fieldId) : fFieldId(fieldId) {}

  Double_t FlowTrackFieldVariable::GetVariable(Track* p) { return p->GetFieldVal(fFieldId); }

  Bool_t FlowTrackFieldVariable::Init(Int_t taskId) {
    auto dfm           = Hal::DataFormatManager::Instance();
    const Event* event = dfm->GetFormat(taskId, EFormatDepth::kNonBuffered);
    Track* track       = event->GetNewTrack();
    TString fieldName  = track->GetFieldName(fFieldId);
    if (fieldName == "[]") return kFALSE;
    SetFieldName(Hal::Std::RemoveUnits(fieldName));
    SetFieldUnit(Hal::Std::GetUnits(fieldName));
    delete track;
    return kTRUE;
  }

}  // namespace Hal
