/*
 * TrackOnlyPrimariesCut.h
 *
 *  Created on: 07-08-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKONLYPRIMARIESCUT_H_
#define HALTRACKONLYPRIMARIESCUT_H_
#include "TrackCut.h"
/**
 * check if track is primary
 */
namespace Hal {
  class TrackOnlyPrimariesCut : public TrackCut {
  public:
    TrackOnlyPrimariesCut();
    void AcceptOnlyNonPrimaries() { SetMinAndMax(0); };
    void AcceptOnlyPrimaries() { SetMinAndMax(1); };
    void AcceptAll() { SetMinMax(0, 1); }
    virtual Bool_t Pass(Track* track);
    virtual ~TrackOnlyPrimariesCut();
    ClassDef(TrackOnlyPrimariesCut, 1)
  };
}  // namespace Hal

#endif /* HALTRACKONLYPRIMARIESCUT_H_ */
