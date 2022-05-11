/*
 * TrackBoosteMomentaCuts.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKBOOSTEMOMENTACUTS_H_
#define HALTRACKBOOSTEMOMENTACUTS_H_

#include "TrackComplexCut.h"

namespace Hal {

  class TrackBoostedPCut : public TrackCut {
  protected:
    TVector3 fBoost;

  public:
    TrackBoostedPCut();
    void SetBoostVector(TVector3 boost) { fBoost = boost; };
    virtual Bool_t Pass(Track* track);
    virtual ~TrackBoostedPCut() {};
    ClassDef(TrackBoostedPCut, 1)
  };

  class TrackBoostedKinematcisCut : public TrackCut {
  protected:
    TVector3 fBoost;

  public:
    TrackBoostedKinematcisCut();
    void SetBoostVector(TVector3 boost) { fBoost = boost; };
    static Int_t Px() { return 0; };
    static Int_t Py() { return 1; };
    static Int_t Pz() { return 2; };
    static Int_t Pt() { return 3; };
    static Int_t Eta() { return 4; };
    static Int_t Rapidity() { return 5; };
    static Int_t P() { return 6; }
    Bool_t Init(Int_t format_id = 0);
    virtual Bool_t Pass(Track* track);
    virtual ~TrackBoostedKinematcisCut();
    ClassDef(TrackBoostedKinematcisCut, 1)
  };

  class TrackBoostedDeltaPCut : public TrackCut {
  protected:
    TVector3 fBoost;

  public:
    TrackBoostedDeltaPCut();
    void SetBoostVector(TVector3 boost) { fBoost = boost; };
    Bool_t Init(Int_t format_id = 0);
    virtual Bool_t Pass(Track* track);
    virtual ~TrackBoostedDeltaPCut() {};
    ClassDef(TrackBoostedDeltaPCut, 1)
  };
}  // namespace Hal
#endif /* HALFEMTO_CUTS_TRACKCUTS_KINEMATICS_HALTRACKBOOSTEMOMENTACUTS_H_ */
