/*
 * TwoTrackLCMSCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef KINEMATICS_HALTWOTRACKLCMSCUT_H_
#define KINEMATICS_HALTWOTRACKLCMSCUT_H_

#include "TwoTrackCut.h"

namespace Hal {
  class TwoTrackLCMSCut : public TwoTrackCut {
    Float_t fM;

  public:
    enum ParID { Rout = 0, Rside = 1, Rlong = 2 };
    TwoTrackLCMSCut();
    virtual Bool_t Pass(TwoTrack* pair);
    void SetMass(Double_t m) { fM = m * m; };
    virtual ~TwoTrackLCMSCut();
    ClassDef(TwoTrackLCMSCut, 1)
  };
}  // namespace Hal

#endif /* KINEMATICS_HALTWOTRACKLCMSCUT_H_ */
