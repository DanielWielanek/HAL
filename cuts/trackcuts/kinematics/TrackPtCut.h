/*
 * TrackPtCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTRACKPTCUT_H_
#define HALTRACKPTCUT_H_

#include "TrackCut.h"
/**
 * check track transverse momentum
 */
namespace Hal{

class TrackPtCut : public TrackCut {
protected:
public:
  TrackPtCut();
  virtual Bool_t Pass(Track* track);
  virtual ~TrackPtCut();
  ClassDef(TrackPtCut, 1)
};
}
#endif /* HALPTCUT_H_ */
