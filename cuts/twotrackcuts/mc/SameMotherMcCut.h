/*
 * SameMotherCut.h
 *
 *  Created on: 2 kwi 2024
 *      Author: daniel
 */

#ifndef HAL_CUTS_TWOTRACKCUTS_MC_SAMEMOTHERMCCUT_H_
#define HAL_CUTS_TWOTRACKCUTS_MC_SAMEMOTHERMCCUT_H_

#include "TwoTrackCut.h"
namespace Hal {

  class SameMotherMcCut : public TwoTrackCut {
  public:
    SameMotherMcCut();
    virtual Bool_t Pass(TwoTrack* pair);
    virtual ~SameMotherMcCut() {};
    ClassDef(SameMotherMcCut, 1)

      virtual Bool_t Init(Int_t);
  };

} /* namespace Hal */

#endif /* HAL_CUTS_TWOTRACKCUTS_MC_SAMEMOTHERMCCUT_H_ */
