/*
 * TwoTrackDeltaEtaCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef _HALTWOTRACKDELTAETACUT_H_
#define _HALTWOTRACKDELTAETACUT_H_
#include "TwoTrackCut.h"
/**
 * check delta eta value pass pair if values of cut are outside of given range
 */
namespace Hal {
  class TwoTrackDeltaEtaCut : public TwoTrackCut {
    Bool_t fAntiValidate;

  public:
    TwoTrackDeltaEtaCut();
    TwoTrackDeltaEtaCut(const TwoTrackDeltaEtaCut& other);
    virtual void StandardValidation() { fAntiValidate = kFALSE; };
    virtual Bool_t Pass(TwoTrack* pair);
    virtual ~TwoTrackDeltaEtaCut();
    virtual Package* Report() const;
    ClassDef(TwoTrackDeltaEtaCut, 1)
  };
}  // namespace Hal
#endif /* _HALTWOTRACKDELTAETACUT_H_ */
