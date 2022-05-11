/*
 * TrackDeltaCut.h
 *
 *  Created on: 20 gru 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKDELTACUT_H_
#define HALTRACKDELTACUT_H_

#include "TrackCut.h"

#include <TLorentzVector.h>
namespace Hal{
class TrackDeltaMomentumCut : public TrackCut {
protected:
  Track* fTrackRe;
  Track* fTrackIm;

public:
  TrackDeltaMomentumCut(Int_t params = 1);
  static Int_t P() { return 0; }
  static Int_t Pt() { return 1; };
  static Int_t Phi() { return 2; }
  static Int_t Theta() { return 3; };
  static Int_t Pz() { return 4; }
  Bool_t Init(Int_t format_id = 0);
  virtual ~TrackDeltaMomentumCut();
  ClassDef(TrackDeltaMomentumCut, 1)
};
/**
 * standard cut for analysis of differences betwen reconstructed and MC momenta
 * using differences between given values e.g pt_reco-pt_sim
 */
class TrackDeltaMomCut : public TrackDeltaMomentumCut {
protected:
  /**
   * fill momentum fields
   * @param track
   * @return true if there is imaginary part
   */
  Bool_t GetMom(Track* track);

public:
  TrackDeltaMomCut(Int_t params = 5);
  virtual Bool_t Pass(Track* track);
  virtual ~TrackDeltaMomCut();
  ClassDef(TrackDeltaMomCut, 1)
};
/**
 * standard cut for analysis of differences betwen reconstructed and MC momenta
 * by using vector difference eg. (vec(preco)-vect(psim)).pt
 */

class TrackDeltaVectMomCut : public TrackDeltaMomentumCut {
protected:
  TLorentzVector fDiff;
  /**
   * fill momentu fields
   * @param track
   * @return true if there is imaginary part
   */
  Bool_t GetMom(Track* track);

public:
  TrackDeltaVectMomCut(Int_t params = 5);
  virtual Bool_t Pass(Track* track);
  virtual ~TrackDeltaVectMomCut();
  ClassDef(TrackDeltaVectMomCut, 1)
};
}
#endif /* HALTRACKDELTACUT_H_ */
