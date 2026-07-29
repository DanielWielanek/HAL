/*
 * DetectorHits.h
 *
 *  Created on: 13 lut 2026
 *      Author: daniel
 */

#ifndef HAL_EXAMPLES_ONTHEFLY_CUTS_OTFDETECTORHITSCUT_H_
#define HAL_EXAMPLES_ONTHEFLY_CUTS_OTFDETECTORHITSCUT_H_

#include "TrackExpCut.h"


namespace HalOTF {

  class DetectorHitsCut : public Hal::TrackExpCut {
  public:
    enum ValID { Unseen = 0, Seen = 1, GoodSeen = 2 };
    DetectorHitsCut(Int_t par);
    virtual Bool_t Init(Int_t taskId);
    virtual ~DetectorHitsCut() {};
    virtual std::vector<TString> GetAxisBinLabels(Int_t par = 0) const;
    ClassDef(DetectorHitsCut, 1)
  };

  class DetectorHitsACut : public DetectorHitsCut {
  public:
    DetectorHitsACut();
    virtual ~DetectorHitsACut() {};
    virtual Bool_t Pass(Hal::Track* track);
    ClassDef(DetectorHitsACut, 1)
  };

  class DetectorHitsBCut : public DetectorHitsCut {
  public:
    DetectorHitsBCut();
    virtual ~DetectorHitsBCut() {};
    virtual Bool_t Pass(Hal::Track* track);
    ClassDef(DetectorHitsBCut, 1)
  };

  class DetectorHitsCCut : public DetectorHitsCut {
  public:
    DetectorHitsCCut();
    virtual ~DetectorHitsCCut() {};
    virtual Bool_t Pass(Hal::Track* track);
    ClassDef(DetectorHitsCCut, 1)
  };

  class DetectorHitsABCSumCut : public DetectorHitsCut {
  public:
    DetectorHitsABCSumCut();
    virtual ~DetectorHitsABCSumCut() {};
    virtual std::vector<TString> GetAxisBinLabels(Int_t par = 0) const;
    virtual Bool_t Pass(Hal::Track* track);
    ClassDef(DetectorHitsABCSumCut, 1)
  };


} /* namespace HalOTF */

#endif /* HAL_EXAMPLES_ONTHEFLY_CUTS_OTFDETECTORHITSCUT_H_ */
