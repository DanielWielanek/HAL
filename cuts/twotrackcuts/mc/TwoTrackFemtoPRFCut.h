/*
 * TwoTrackFemtoPRFCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTWOTRACKFEMTOPRFCUT_H_
#define HALTWOTRACKFEMTOPRFCUT_H_

#include "TwoTrackCut.h"

#include <TLorentzVector.h>
namespace Hal {
  /**
   * check k* and rstar
   */
  class TwoTrackFemtoPRFCut : public TwoTrackCut {
    TLorentzVector fP1, fP2, fX1, fX2;
    void Compute();

  public:
    enum ParID { RStar = 1, KStar = 0 };
    TwoTrackFemtoPRFCut();
    virtual Bool_t Pass(TwoTrack* pair);
    virtual Bool_t Init(Int_t task_id);
    virtual ~TwoTrackFemtoPRFCut();
    ClassDef(TwoTrackFemtoPRFCut, 1)
  };
  /**
   * full information - 8 parameters
   */
  class TwoTrackFemtoPRFCutFull : public TwoTrackCut {
    TLorentzVector fP1, fP2, fX1, fX2;
    void Compute();

  public:
    enum ParID {
      RStarOut  = 4,
      RStarSide = 5,
      RStarLong = 6,
      TStar     = 7,
      RStar     = 8,
      KStar     = 9,
      KStarOut  = 0,
      KStarSide = 1,
      KStarLong = 2,
      KStarEn   = 3
    };
    TwoTrackFemtoPRFCutFull();
    virtual Bool_t Pass(TwoTrack* pair);
    virtual Bool_t Init(Int_t task_id);
    virtual ~TwoTrackFemtoPRFCutFull();
    ClassDef(TwoTrackFemtoPRFCutFull, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACKFEMTOPRFCUT_H_ */
