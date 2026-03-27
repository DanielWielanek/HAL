/*
 * TrackBasicKinematicsCut.h
 *
 *  Created on: 27 mar 2026
 *      Author: daniel
 */

#ifndef HAL_CUTS_TRACKCUTS_KINEMATICS_TRACKBASICKINEMATICSCUT_H_
#define HAL_CUTS_TRACKCUTS_KINEMATICS_TRACKBASICKINEMATICSCUT_H_

#include "TrackCut.h"


namespace Hal {

  class TrackPtYCut : public TrackCut {
  public:
    TrackPtYCut();
    void SetPt(Double_t lo, Double_t hi) { SetMinMax(lo, hi, 0); }
    void SetY(Double_t lo, Double_t hi) { SetMinMax(lo, hi, 1); }
    virtual Bool_t Pass(Track* track);
    virtual ~TrackPtYCut() {};
    ClassDef(TrackPtYCut, 1)
  };

  class TrackPtYAssumedCut : public TrackCut {
    Double_t fMass2   = {0};
    Int_t fAssumedPid = {0};

  public:
    TrackPtYAssumedCut();
    void SetAssumedPid(Int_t pid) { fAssumedPid = pid; }
    void SetPt(Double_t lo, Double_t hi) { SetMinMax(lo, hi, 0); }
    void SetY(Double_t lo, Double_t hi) { SetMinMax(lo, hi, 1); }
    virtual Bool_t Init(Int_t taskId);
    virtual Bool_t Pass(Track* track);
    virtual ~TrackPtYAssumedCut() {};
    ClassDef(TrackPtYAssumedCut, 1)
  };

  class TrackPtEtaCut : public TrackCut {
  public:
    TrackPtEtaCut();
    void SetPt(Double_t lo, Double_t hi) { SetMinMax(lo, hi, 0); }
    void SetEta(Double_t lo, Double_t hi) { SetMinMax(lo, hi, 1); }
    virtual Bool_t Pass(Track* track);
    virtual ~TrackPtEtaCut() {};
    ClassDef(TrackPtEtaCut, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_TRACKCUTS_KINEMATICS_TRACKBASICKINEMATICSCUT_H_ */
