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
    TwoTrackLCMSCut();
    virtual Bool_t Pass(TwoTrack* pair);
    void SetMass(Double_t m) { fM = m * m; };
    static Int_t Rout() { return 0; };
    static Int_t Rside() { return 1; };
    static Int_t Rlong() { return 2; };
    virtual ~TwoTrackLCMSCut();
    ClassDef(TwoTrackLCMSCut, 1)
  };
}  // namespace Hal

#endif /* KINEMATICS_HALTWOTRACKLCMSCUT_H_ */
