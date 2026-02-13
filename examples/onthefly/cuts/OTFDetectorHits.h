/*
 * DetectorHits.h
 *
 *  Created on: 13 lut 2026
 *      Author: daniel
 */

#ifndef HAL_EXAMPLES_ONTHEFLY_CUTS_OTFDETECTORHITS_H_
#define HAL_EXAMPLES_ONTHEFLY_CUTS_OTFDETECTORHITS_H_

#include "TrackExpCut.h"


namespace HalOTF {

  class DetectorHits : public Hal::TrackExpCut {
  public:
    enum ValID { Unseen = 0, Seen = 1, GoodSeen = 2 };
    DetectorHits(Int_t par);
    virtual Bool_t Init(Int_t taskId);
    virtual ~DetectorHits() {};
    virtual std::vector<std::pair<TString, Double_t>> GetBinLabels(Int_t par = 0) const;
    ClassDef(DetectorHits, 1)
  };

  class DetectorHitsA : public DetectorHits {
  public:
    DetectorHitsA();
    virtual ~DetectorHitsA() {};
    virtual Bool_t Pass(Hal::Track* track);
    ClassDef(DetectorHitsA, 1)
  };

  class DetectorHitsB : public DetectorHits {
  public:
    DetectorHitsB();
    virtual ~DetectorHitsB() {};
    virtual Bool_t Pass(Hal::Track* track);
    ClassDef(DetectorHitsB, 1)
  };

  class DetectorHitsC : public DetectorHits {
  public:
    DetectorHitsC();
    virtual ~DetectorHitsC() {};
    virtual Bool_t Pass(Hal::Track* track);
    ClassDef(DetectorHitsC, 1)
  };

  class DetectorHitsABCSum : public DetectorHits {
  public:
    DetectorHitsABCSum();
    virtual ~DetectorHitsABCSum() {};
    virtual std::vector<std::pair<TString, Double_t>> GetBinLabels(Int_t par = 0) const;
    virtual Bool_t Pass(Hal::Track* track);
    ClassDef(DetectorHitsABCSum, 1)
  };


} /* namespace HalOTF */

#endif /* HAL_EXAMPLES_ONTHEFLY_CUTS_OTFDETECTORHITS_H_ */
