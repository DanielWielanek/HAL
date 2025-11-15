/*
 * ToFBasicCut.h
 *
 *  Created on: 26 wrz 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKBASICTOFCUT_H_
#define HALTRACKBASICTOFCUT_H_

#include "TrackExpCut.h"
namespace Hal {
  class TrackBasicToFCut : public TrackExpCut {

  public:
    enum ParID { Mass2 = 0, Beta = 1, Flag = 2 };
    TrackBasicToFCut();
    Bool_t Init(Int_t formad_id);
    /**
     * accept only tracks with TOF signal (flag 1)
     */
    virtual void AcceptOnlyWithTof() { SetMinMax(1, Flag); };
    /**
     * accept only without TOF signal (flag 0)
     */
    virtual void AcceptOnlyWithoutTof() { SetMinMax(0, Flag); }
    virtual Bool_t Pass(Track* tr);
    virtual ~TrackBasicToFCut();
    ClassDef(TrackBasicToFCut, 1)
  };
}  // namespace Hal

#endif /* HALTRACKBASICTOFCUT_H_ */
