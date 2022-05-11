/*
 * TrackPCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKPCUT_H_
#define HALTRACKPCUT_H_
#include "TrackCut.h"

namespace Hal{

class TrackPCut : public TrackCut {
public:
  TrackPCut();
  virtual Bool_t Pass(Track* track);
  virtual ~TrackPCut();
  ClassDef(TrackPCut, 1)
};

class TrackPSignedCut : public TrackCut {
public:
  TrackPSignedCut();
  virtual Bool_t Pass(Track* track);
  virtual ~TrackPSignedCut();
  ClassDef(TrackPSignedCut, 1)
};
}
#endif /* HALTRACKPCUT_H_ */
