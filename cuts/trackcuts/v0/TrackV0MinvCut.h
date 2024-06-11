/*
 * TrackV0MinvCut.h
 *
 *  Created on: 10 cze 2024
 *      Author: daniel
 */

#ifndef HAL_CUTS_TRACKCUTS_V0_TRACKV0MINVCUT_H_
#define HAL_CUTS_TRACKCUTS_V0_TRACKV0MINVCUT_H_

#include "TrackV0Cut.h"

namespace Hal {

  class TrackV0MinvCut : public TrackV0Cut {
  protected:
    Int_t fPid1 = {0}, fPid2 = {0};
    Double_t fM1 = {-1.0}, fM2 = {-1.0};
    virtual Bool_t PassV0(V0Track* tr);

  public:
    TrackV0MinvCut();
    void SetDaughersPid(Int_t pos, Int_t neg);
    Bool_t Init(Int_t taskid);
    virtual ~TrackV0MinvCut() {};
    ClassDef(TrackV0MinvCut, 1)
  };
  class TrackV0InvMassLambdaCut : public TrackV0Cut {
  protected:
    virtual Bool_t PassV0(V0Track* tr);

  public:
    TrackV0InvMassLambdaCut();
    virtual ~TrackV0InvMassLambdaCut() {};
    ClassDef(TrackV0InvMassLambdaCut, 1)
  };

  class TrackV0InvMassAntiLambdaCut : public TrackV0Cut {
  protected:
    virtual Bool_t PassV0(V0Track* tr);

  public:
    TrackV0InvMassAntiLambdaCut();
    virtual ~TrackV0InvMassAntiLambdaCut() {};
    ClassDef(TrackV0InvMassAntiLambdaCut, 1)
  };

  class TrackV0InvMassK0Cut : public TrackV0Cut {
  protected:
    virtual Bool_t PassV0(V0Track* tr);

  public:
    TrackV0InvMassK0Cut();
    virtual ~TrackV0InvMassK0Cut() {};
    ClassDef(TrackV0InvMassK0Cut, 1)
  };
} /* namespace Hal */

#endif /* HAL_CUTS_TRACKCUTS_V0_TRACKV0MINVCUT_H_ */
