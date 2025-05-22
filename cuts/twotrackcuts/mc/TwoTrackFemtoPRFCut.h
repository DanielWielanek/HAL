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
    TwoTrackFemtoPRFCut();
    virtual Bool_t Pass(TwoTrack* pair);
    virtual Bool_t Init(Int_t task_id);
    static Int_t RStar() { return 1; };
    static Int_t KStar() { return 0; };
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
    TwoTrackFemtoPRFCutFull();
    virtual Bool_t Pass(TwoTrack* pair);
    virtual Bool_t Init(Int_t task_id);
    static Int_t RStarOut() { return 4; }
    static Int_t RStarSide() { return 5; }
    static Int_t RStarLong() { return 6; }
    static Int_t TStar() { return 7; }
    static Int_t RStar() { return 8; }
    static Int_t KStar() { return 9; }
    static Int_t KStarOut() { return 0; };
    static Int_t KStarSide() { return 1; };
    static Int_t KStarLong() { return 2; };
    static Int_t KStarEn() { return 3; };
    virtual ~TwoTrackFemtoPRFCutFull();
    ClassDef(TwoTrackFemtoPRFCutFull, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACKFEMTOPRFCUT_H_ */
