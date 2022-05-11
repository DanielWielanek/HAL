/*
 * PairDeltaQinvCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALPAIRDELTAQINVCUT_H_
#define HALPAIRDELTAQINVCUT_H_

#include "TwoTrackCut.h"

namespace Hal {
  class Track;

  class PairDeltaQinvCut : public TwoTrackCut {
    Double_t fMass1, fMass2;
    Double_t Q(Track* tr1, Track* tr2) const;

  public:
    PairDeltaQinvCut();
    virtual Bool_t Pass(TwoTrack* pair);
    static Int_t Relative() { return 0; }
    static Int_t Absolute() { return 1; };
    Bool_t Init(Int_t task_id);
    void SetMass(Double_t m) { fMass1 = fMass2 = m; };
    void SetMass(Double_t m1, Double_t m2) {
      fMass1 = m1;
      fMass2 = m2;
    };
    virtual ~PairDeltaQinvCut();
    ClassDef(PairDeltaQinvCut, 1)
  };
}  // namespace Hal

#endif /* HALPAIRDELTAQINVCUT_H_ */
