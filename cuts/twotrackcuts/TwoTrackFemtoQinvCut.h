/*
 * TwoTrackQinvCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTWOTRACKFEMTOQINVCUT_H_
#define HALTWOTRACKFEMTOQINVCUT_H_

#include "TwoTrackCut.h"
namespace Hal {
  class TwoTrackFemtoQinvCut : public TwoTrackCut {
    Double_t fMass2;

  public:
    TwoTrackFemtoQinvCut(Double_t mass = 0.13957);
    Bool_t Pass(TwoTrack* pair);
    Package* Report() const;
    virtual ~TwoTrackFemtoQinvCut();
    ClassDef(TwoTrackFemtoQinvCut, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACKFEMTOQINVCUT_H_ */
