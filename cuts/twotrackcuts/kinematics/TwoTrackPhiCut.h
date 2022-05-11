/*
 * TwoTrackPhiCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTWOTRACKPHICUT_H_
#define HALTWOTRACKPHICUT_H_
#include "TwoTrackCut.h"
/**
 * check pair azimuthal angle
 */
namespace Hal {
  class TwoTrackPhiCut : public TwoTrackCut {
  public:
    TwoTrackPhiCut();
    virtual Bool_t Pass(TwoTrack* pair);
    virtual ~TwoTrackPhiCut();
    ClassDef(TwoTrackPhiCut, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACKPHICUT_H_ */
