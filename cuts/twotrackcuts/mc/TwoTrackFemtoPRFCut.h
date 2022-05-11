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
}  // namespace Hal
#endif /* HALTWOTRACKFEMTOPRFCUT_H_ */
