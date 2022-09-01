/*
 * V0MassInvCut.h
 *
 *  Created on: 05-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_CUTS_TRACKCUTS_V0CUTS_HALV0MASSINVCUT_H_
#define HALFEMTO_CUTS_TRACKCUTS_V0CUTS_HALV0MASSINVCUT_H_

#include "TrackCut.h"

namespace Hal {
  class Package;
  class V0MassInvCut : public TrackCut {
    Int_t fPid1Hypo = {0};
    Int_t fPid2Hypo = {0};

    Double_t fM1 = {0}, fM2 = {0};

  public:
    V0MassInvCut();
    void SetDaughterPidHypo(Int_t posPdg, Int_t negPdg) {
      fPid1Hypo = posPdg;
      fPid2Hypo = negPdg;
    }
    virtual Bool_t Init(Int_t task_id);
    virtual Bool_t Pass(Track* track);
    virtual Package* Report() const;
    virtual ~V0MassInvCut() {};
    ClassDef(V0MassInvCut, 1)
  };

  class V0InvMassLambdaCut : public TrackCut {
  public:
    V0InvMassLambdaCut();
    virtual Bool_t Pass(Track* track);
    virtual ~V0InvMassLambdaCut() {};
    ClassDef(V0InvMassLambdaCut, 1)
  };

  class V0InvMassAntiLambdaCut : public TrackCut {
  public:
    V0InvMassAntiLambdaCut();
    virtual Bool_t Pass(Track* track);
    virtual ~V0InvMassAntiLambdaCut() {};
    ClassDef(V0InvMassAntiLambdaCut, 1)
  };
  class V0InvMassK0Cut : public TrackCut {
  public:
    V0InvMassK0Cut();
    virtual Bool_t Pass(Track* track);
    virtual ~V0InvMassK0Cut() {};
    ClassDef(V0InvMassK0Cut, 1)
  };

}  // namespace Hal


#endif /* HALFEMTO_CUTS_TRACKCUTS_V0CUTS_HALV0MASSINVCUT_H_ */
