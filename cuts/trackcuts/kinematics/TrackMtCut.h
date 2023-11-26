/*
 * TrackMtCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTRACKMTCUT_H_
#define HALTRACKMTCUT_H_

#include "TrackCut.h"

#include <TDatabasePDG.h>
/**
 * check transerse mass of track
 */

namespace Hal {

  class TrackMtCut : public TrackCut {
    Bool_t fMassFixed;
    Double_t fMass2;
    TDatabasePDG* fPdg;

  public:
    TrackMtCut();
    /**
     * copy constructor
     * @param cut cut to make copy form
     */
    TrackMtCut(const TrackMtCut& cut);
    /**
     * fix mass used to calculate Mt
     * @param mass mass of particle
     */
    void FixMass(Double_t mass);
    virtual Bool_t Pass(Track* track);
    virtual Bool_t Init(const Int_t task_id = 0);
    virtual Package* Report() const;
    virtual ~TrackMtCut();
    ClassDef(TrackMtCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKMTCUT_H_ */
