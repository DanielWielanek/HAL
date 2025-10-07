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
    enum ValID { Primary = 1, NonPrimary = 0 };
    TrackPrimaryFlagCut() : TrackCut(1) { SetUnitName("PrimaryFlag [AU]"); };
    virtual Bool_t Pass(Track* track);
    virtual std::vector<std::pair<TString, Double_t>> GetBinLabels(Int_t par = 0) const;
    virtual ~TrackPrimaryFlagCut() {};
    ClassDef(TrackPrimaryFlagCut, 1)
  };

  class TrackV0FlagCut : public TrackCut {
  public:
    enum ValID { BadV0 = 1, GoodV0 = 2, NonV0 = 0 };
    TrackV0FlagCut() : TrackCut(1) { SetUnitName("V0Flag [AU]"); };
    virtual std::vector<std::pair<TString, Double_t>> GetBinLabels(Int_t par = 0) const;
    virtual Bool_t Pass(Track* track);
    virtual ~TrackV0FlagCut() {};
    ClassDef(TrackV0FlagCut, 1)
  };


  class TrackGoodSecondaryFlagCut : public TrackCut {
  public:
    enum ValID { HasMother = 1, IsOrphan = 0 };
    TrackGoodSecondaryFlagCut() : TrackCut(1) { SetUnitName("MotherFlag [AU]"); };
    virtual std::vector<std::pair<TString, Double_t>> GetBinLabels(Int_t par = 0) const;
    virtual Bool_t Pass(Track* track);
    virtual ~TrackGoodSecondaryFlagCut() {};
    ClassDef(TrackGoodSecondaryFlagCut, 1)
  };

  class TrackGlobalFlagCut : public TrackCut {
  public:
    enum ValID { GlobalTrack = 1, NonGlobalTrack = 0 };
    TrackGlobalFlagCut() : TrackCut(1) { SetUnitName("GlobalFlag [AU]"); };
    virtual std::vector<std::pair<TString, Double_t>> GetBinLabels(Int_t par = 0) const;
    virtual Bool_t Pass(Track* track);
    virtual ~TrackGlobalFlagCut() {};
    ClassDef(TrackGlobalFlagCut, 1)
  };
}  // namespace Hal
#endif /* HALFEMTO_CUTS_TRACKCUTS_HALTRACKFLAGCUTS_H_ */
