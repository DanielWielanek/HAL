/*
 * TwoTrackKtCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALTWOTRACKKTCUT_H_
#define HALTWOTRACKKTCUT_H_

#include "TwoTrackCut.h"
/**
 * check pair kt value
 */
namespace Hal {
  class TwoTrackKtCut : public TwoTrackCut {
  protected:
  public:
    TwoTrackKtCut();
    virtual Bool_t Pass(TwoTrack* pair);
    virtual ~TwoTrackKtCut();
    ClassDef(TwoTrackKtCut, 1)
  };
}  // namespace Hal

#endif /* HALKTCUT_H_ */
