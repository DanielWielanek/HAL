/*
 * PidHypo.h
 *
 *  Created on: 14 lut 2026
 *      Author: daniel
 */

#ifndef HAL_EXAMPLES_ONTHEFLY_CUTS_OTFPIDHYPOCUT_H_
#define HAL_EXAMPLES_ONTHEFLY_CUTS_OTFPIDHYPOCUT_H_

#include "TrackExpCut.h"


namespace HalOTF {

  class PidHypoCut : public Hal::TrackExpCut {
  public:
    PidHypoCut();
    virtual Bool_t Pass(Hal::Track* track);
    virtual Bool_t Init(Int_t taskId);
    virtual ~PidHypoCut();
    ClassDef(PidHypoCut, 1)
  };

} /* namespace HalOTF */

#endif /* HAL_EXAMPLES_ONTHEFLY_CUTS_OTFPIDHYPOCUT_H_ */
