/*
 * TrackPhiCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKPHICUT_H_
#define HALTRACKPHICUT_H_
#include "TrackCut.h"
/**
 * check track azimuthal angle
 */
namespace Hal{

class TrackPhiCut : public TrackCut {
  Bool_t fRound;

public:
  TrackPhiCut();
  virtual Bool_t Init(Int_t task_id);
  virtual Bool_t Pass(Track* track);
  virtual ~TrackPhiCut();
  ClassDef(TrackPhiCut, 1)
};
}
#endif /* HALTRACKPHICUT_H_ */
