/*
 * TrackFlagCuts.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKFLAGCUTS_H_
#define HALTRACKFLAGCUTS_H_

#include "TrackCut.h"
namespace Hal {
  class Track;

  class TrackPrimaryFlagCut : public TrackCut {
  public:
    TrackPrimaryFlagCut() : TrackCut(1) { SetUnitName("PrimaryFlag [AU]"); };
    static Int_t Primary() { return 1; };
    static Int_t NonPrimary() { return 0; };
    virtual Bool_t Pass(Track* track);
    virtual ~TrackPrimaryFlagCut() {};
    ClassDef(TrackPrimaryFlagCut, 1)
  };

  class TrackV0FlagCut : public TrackCut {
  public:
    TrackV0FlagCut() : TrackCut(1) { SetUnitName("V0Flag [AU]"); };
    static Int_t BadV0() { return 1; };
    static Int_t GoodV0() { return 2; };
    static Int_t NonV0() { return 0; }
    virtual Bool_t Pass(Track* track);
    virtual ~TrackV0FlagCut() {};
    ClassDef(TrackV0FlagCut, 1)
  };


  class TrackGoodSecondaryFlagCut : public TrackCut {
  public:
    TrackGoodSecondaryFlagCut() : TrackCut(1) { SetUnitName("MotherFlag [AU]"); };
    static Int_t HasMother() { return 1; };
    static Int_t IsOrphan() { return 0; }
    virtual Bool_t Pass(Track* track);
    virtual ~TrackGoodSecondaryFlagCut() {};
    ClassDef(TrackGoodSecondaryFlagCut, 1)
  };

  class TrackGlobalFlagCut : public TrackCut {
  public:
    TrackGlobalFlagCut() : TrackCut(1) { SetUnitName("GlobalFlag [AU]"); };
    static Int_t GlobalTrack() { return 1; }
    static Int_t NonGlobalTrack() { return 0; }
    virtual Bool_t Pass(Track* track);
    virtual ~TrackGlobalFlagCut() {};
    ClassDef(TrackGlobalFlagCut, 1)
  };
}  // namespace Hal
#endif /* HALFEMTO_CUTS_TRACKCUTS_HALTRACKFLAGCUTS_H_ */
