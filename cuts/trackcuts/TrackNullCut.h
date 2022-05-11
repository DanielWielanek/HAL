/*
 * TrackNullCut.h
 *
 *  Created on: 12 lis 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_CUTS_TRACKCUTS_HALTRACKNULLCUT_H_
#define HALFEMTO_CUTS_TRACKCUTS_HALTRACKNULLCUT_H_

#include "TrackCut.h"

/**
 * cut that might be used for accepting/rejecting only particles with/withotut imaginary part
 */
namespace Hal {
  class TrackNullCut : public TrackCut {
  public:
    TrackNullCut();
    /**
     * do not use this cut
     */
    void Deactivate() { SetMinMax(0, 1); };
    /**
     * accept only particles with imaginary part
     */
    void AcceptOnlyMatched() { SetMinAndMax(1); };
    /**
     * accept only particles without imaginary part
     */
    void AcceptOnlyNotMatched() { SetMinAndMax(0); };
    Bool_t Pass(Track* track);
    virtual Bool_t Init(Int_t task_id);
    virtual ~TrackNullCut();
    ClassDef(TrackNullCut, 1)
  };
}  // namespace Hal


#endif /* HALFEMTO_CUTS_TRACKCUTS_HALTRACKNULLCUT_H_ */
