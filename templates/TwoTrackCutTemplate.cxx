/*
 * TwoTrackCutTemplates.cxx
 *
 *  Created on: 10 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "TwoTrackCutTemplate.h"

#include <Hal/DataFormatManager.h>
#include <Hal/Event.h>
#include <Hal/Package.h>
#include <Hal/TwoTrack.h>

namespace MyHal {

  /**
   * do not forget to set number of parameters in parent constructor !
   */
  TwoTrackCutTemplate::TwoTrackCutTemplate() : TwoTrackCut(1) {
    /**
     *  SetUnitName(name, unit, id) - for all parameters, optionally call SetMinAndMax(min, mas, id)
     *  */
  }

  TwoTrackCutTemplate::~TwoTrackCutTemplate() {}
  /**
   * this is optional method if you want to chek format
   * @param taskID
   * @return
   */
  Int_t TwoTrackCutTemplate::Init(Int_t taskID) {
    Hal::DataFormatManager* mngr = Hal::DataFormatManager::Instance();
    const Hal::Event* event      = mngr->GetFormat(taskID, Hal::EFormatDepth::kNonBuffered);
    if (dynamic_cast<Hal::Event*>(event)) return kTRUE;
    return TwoTrackCut::Init(taskID);
  }

  Bool_t TwoTrackCutTemplate::Pass(Hal::TwoTrack* pair) {
    switch (pair->GetPairType()) {
      case Hal::TwoTrack::PairType::kSignal: {
        /**
         * SetValue(val, id)
         */
      } break;
      case Hal::TwoTrack::PairType::kBackground: {
        /**
         * SetValue(val, id)
         */
      } break;
      case Hal::TwoTrack::PairType::kMixed: {
        /**
         * SetValue(val, id)
         */
      } break;
      case Hal::TwoTrack::PairType::kRotated: {
        /**
         * SetValue(val, id) - don't forget to swap Px and Py
         */
      } break;
      case Hal::TwoTrack::PairType::kHemishpere: {
        /**
         * SetValue(val,id) - don't forget to swap Px, Py, Pz
         */
      } break;
      case Hal::TwoTrack::PairType::kOther: {
        /**
         * SetValue
         */
      } break;
    }
    return Validate(); /**
    or force validate if done by hand */
  }

  Hal::Package* TwoTrackCutTemplate::Report() const {
    Hal::Package* pack = Hal::TwoTrackCut::Report();
    /* add some objects
    pack->AddObject
    */
    return pack;
  }
}  // namespace MyHal
