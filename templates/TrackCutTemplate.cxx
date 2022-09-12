/*
 * TrackCutTemplates.cxx
 *
 *  Created on: 10 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TrackCutTemplate.h"

#include <Hal/DataFormatManager.h>
#include <Hal/Event.h>
#include <Hal/Package.h>
#include <Hal/Track.h>
namespace MyHal {
  /**
   * do not forget to set number of parameters in parent constructor !
   */
  TrackCutTemplate::TrackCutTemplate() : TrackCut(1) {
    /**
     *  SetUnitName(name, unit, id) - for all parameters, optionally call SetMinAndMax(min, mas, id)
     *  */
  }

  TrackCutTemplate::~TrackCutTemplate() {}
  /**
   * this is optional method if you want to chek format
   * @param taskID
   * @return
   */
  Int_t TrackCutTemplate::Init(Int_t taskID) {
    Hal::DataFormatManager* mngr = Hal::DataFormatManager::Instance();
    const Hal::Event* event      = mngr->GetFormat(taskID, Hal::EFormatDepth::kNonBuffered);
    if (dynamic_cast<Hal::Event*>(event)) return kTRUE;
    return TrackCut::Init(taskID);
  }

  Bool_t TrackCutTemplate::Pass(Hal::Track* pair) {
    /**
     * SetValue(val, id)
     */
    return Validate(); /**
    or force validate if done by hand */
  }

  Hal::Package* TrackCutTemplate::Report() const {
    Hal::Package* pack = Hal::TrackCut::Report();
    /* add some objects
    pack->AddObject
    */
    return pack;
  }

}  // namespace MyHal