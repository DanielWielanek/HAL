/*
 * PairEtaPtCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_CUTS_TWOTRACKCUTS_KINEMATICS_HALPAIRETAPTCUT_H_
#define HALFEMTO_CUTS_TWOTRACKCUTS_KINEMATICS_HALPAIRETAPTCUT_H_

#include "TwoTrackCut.h"
namespace Hal {
  class PairEtaPtCut : public TwoTrackCut {
  public:
    PairEtaPtCut();
    Bool_t Pass(TwoTrack* pair);
    static Int_t Pt() { return 1; };
    static Int_t Eta() { return 0; };
    virtual ~PairEtaPtCut();
    ClassDef(PairEtaPtCut, 1)
  };
}  // namespace Hal

#endif /* HALFEMTO_CUTS_TWOTRACKCUTS_KINEMATICS_HALPAIRETAPTCUT_H_ */
