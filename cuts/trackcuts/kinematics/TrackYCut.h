/*
 * TrackYCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKYCUT_H_
#define HALTRACKYCUT_H_
#include "TrackCut.h"

namespace Hal {
  /**
   * check rapidity of track
   */
  class TrackYCut : public TrackCut {
  public:
    TrackYCut();
    virtual Bool_t Pass(Track* track);
    virtual ~TrackYCut();
    ClassDef(TrackYCut, 1)
  };

  /**
   * check rapidity of track - but user must set rapidity by hand
   */
  class TrackYAssumedCut : public TrackCut {
    Double_t fMass;
    Int_t fPdg;

  public:
    TrackYAssumedCut() : TrackCut(1), fMass(-1), fPdg(0) { SetUnitName("y [AU]"); };
    Bool_t Pass(Track* track);
    Bool_t Init(Int_t taskId);
    void SetPid(Int_t pid);
    Package* Report() const;
    virtual ~TrackYAssumedCut() {};
    ClassDef(TrackYAssumedCut, 1);
  };

}  // namespace Hal

#endif /* HALTRACKYCUT_H_ */
