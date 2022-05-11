/*
 * V0BasicCut.h
 *
 *  Created on: 05-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALV0CANDBASICCUT_H_
#define HALV0CANDBASICCUT_H_
/**
 * basic class for V0 selection
 */
#include "TwoTrackCut.h"

namespace Hal {
  class V0CandBasicCut : public TwoTrackCut {
  public:
    V0CandBasicCut();
    virtual Bool_t Pass(TwoTrack* pair);
    static Int_t DCA1to2() { return 0; };
    static Int_t DCAToPV() { return 1; };
    static Int_t PtArm() { return 2; };
    static Int_t AlphaArm() { return 3; };
    static Int_t CosAngle() { return 4; };
    static Int_t DecLength() { return 5; };
    static Int_t InvMass() { return 6; };
    virtual ~V0CandBasicCut();
    ClassDef(V0CandBasicCut, 1)
  };
}  // namespace Hal
#endif /* HALV0CANDBASICCUT_H_ */
