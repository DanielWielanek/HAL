/*
 * TrackDeltaMomCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKDELTAMOMCUT_H_
#define HALTRACKDELTAMOMCUT_H_

#include "TrackDeltaMomentumCut.h"
namespace Hal{
class TrackDeltaPtCut : public TrackDeltaMomCut {
public:
  TrackDeltaPtCut();
  Bool_t Pass(Track* track);
  virtual ~TrackDeltaPtCut();
  ClassDef(TrackDeltaPtCut, 1)
};

class TrackDeltaPCut : public TrackDeltaMomCut {
public:
  TrackDeltaPCut();
  Bool_t Pass(Track* track);
  virtual ~TrackDeltaPCut();
  ClassDef(TrackDeltaPCut, 1)
};

class TrackDeltaPzCut : public TrackDeltaMomCut {
public:
  TrackDeltaPzCut();
  Bool_t Pass(Track* track);
  virtual ~TrackDeltaPzCut();
  ClassDef(TrackDeltaPzCut, 1)
};

class TrackDeltaPhiCut : public TrackDeltaMomCut {
public:
  TrackDeltaPhiCut();
  Bool_t Pass(Track* track);
  virtual ~TrackDeltaPhiCut();
  ClassDef(TrackDeltaPhiCut, 1)
};
class TrackDeltaThetaCut : public TrackDeltaMomCut {
public:
  TrackDeltaThetaCut();
  Bool_t Pass(Track* track);
  virtual ~TrackDeltaThetaCut();
  ClassDef(TrackDeltaThetaCut, 1)
};

class TrackDeltaVectPtCut : public TrackDeltaVectMomCut {
public:
  TrackDeltaVectPtCut();
  Bool_t Pass(Track* track);
  virtual ~TrackDeltaVectPtCut();
  ClassDef(TrackDeltaVectPtCut, 1)
};

class TrackDeltaVectPCut : public TrackDeltaVectMomCut {
public:
  TrackDeltaVectPCut();
  Bool_t Pass(Track* track);
  virtual ~TrackDeltaVectPCut();
  ClassDef(TrackDeltaVectPCut, 1)
};

class TrackDeltaVectPhiCut : public TrackDeltaVectMomCut {
public:
  TrackDeltaVectPhiCut();
  Bool_t Pass(Track* track);
  virtual ~TrackDeltaVectPhiCut();
  ClassDef(TrackDeltaVectPhiCut, 1)
};

class TrackDeltaVectThetaCut : public TrackDeltaVectMomCut {
public:
  TrackDeltaVectThetaCut();
  Bool_t Pass(Track* track);
  virtual ~TrackDeltaVectThetaCut();
  ClassDef(TrackDeltaVectThetaCut, 1)
};
}
#endif /* HALTRACKDELTAMOMCUT_H_ */
