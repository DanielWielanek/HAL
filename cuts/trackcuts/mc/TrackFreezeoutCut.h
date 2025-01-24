/*
 * TrackFreezeoutCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKFREEZEOUTCUT_H_
#define HALTRACKFREEZEOUTCUT_H_
#include "TrackMCCut.h"

#include "TrackCut.h"
/**
 * check freezout parameters, has four for X,Y,Z and T
 */
namespace Hal {

  class TrackFreezeoutCut : public TrackMCCut {
  public:
    TrackFreezeoutCut();
    Bool_t Pass(Track* track);
    virtual ~TrackFreezeoutCut();
    ClassDef(TrackFreezeoutCut, 1)
  };

  /**
   * check track freezout time
   */
  class TrackTFreezCut : public TrackMCCut {
  public:
    TrackTFreezCut();
    virtual Bool_t Pass(Track* track);
    virtual ~TrackTFreezCut();
    ClassDef(TrackTFreezCut, 1)
  };

  /**
   * check freezout tau for track
   */
  class TrackTauCut : public TrackMCCut {
  public:
    TrackTauCut();
    virtual Bool_t Pass(Track* track);
    virtual ~TrackTauCut();
    ClassDef(TrackTauCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKFREEZEOUTCUT_H_ */
