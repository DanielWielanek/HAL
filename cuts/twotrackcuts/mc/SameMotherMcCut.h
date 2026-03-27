/*
 * SameMotherCut.h
 *
 *  Created on: 2 kwi 2024
 *      Author: daniel
 */

#ifndef HAL_CUTS_TWOTRACKCUTS_MC_SAMEMOTHERMCCUT_H_
#define HAL_CUTS_TWOTRACKCUTS_MC_SAMEMOTHERMCCUT_H_

#include "TwoTrackMcCut.h"
namespace Hal {

  class SameMotherMcCut : public TwoTrackMcCut {
  public:
    SameMotherMcCut();
    virtual Bool_t Pass(TwoTrack* pair);
    virtual ~SameMotherMcCut() {};
    ClassDef(SameMotherMcCut, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_TWOTRACKCUTS_MC_SAMEMOTHERMCCUT_H_ */
