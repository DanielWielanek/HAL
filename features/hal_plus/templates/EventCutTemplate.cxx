/*
 *Generated by hal-cmake
 */
#include "EventCutTemplate.h"

#include <Hal/DataFormatManager.h>
#include <Hal/Event.h>
#include <Hal/Package.h>

namespace MyHal {

  /**
   * do not forget to set number of parameters in parent constructor !
   */
  EventCutTemplate::EventCutTemplate() : EventCut(1) {
    /**
     *  SetUnitName(name, unit, id) - for all parameters, optionally call SetMinAndMax(min, mas, id)
     *  */
  }

  EventCutTemplate::~EventCutTemplate() {}
  /**
   * this is optional method if you want to chek format
   * @param taskID
   * @return
   */
  Int_t EventCutTemplate::Init(Int_t taskID) {
    Hal::DataFormatManager* mngr = Hal::DataFormatManager::Instance();
    const Hal::Event* event      = mngr->GetFormat(taskID, Hal::EFormatDepth::kNonBuffered);
    if (dynamic_cast<Hal::Event*>(event)) return kTRUE;
    return EventCutTempate::Init(taskID);
  }

  Bool_t EventCutTemplate::Pass(Hal::Event* pair) {
    /**
     * SetValue(val, id)
     */
    return Validate(); /**
    or force validate if done by hand */
  }

  Hal::Package* EventCutTemplate::Report() const {
    Hal::Package* pack = Hal::EventCut::Report();
    /* add some objects
    pack->AddObject
    */
    return pack;
  }

}  // namespace MyHal