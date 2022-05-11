/*
 * TwoTrackDCACut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTWOTRACKDCACUT_H_
#define HALTWOTRACKDCACUT_H_

#include "TwoTrackCut.h"

namespace Hal {
  class TwoTrackDCACut : public TwoTrackCut {
  public:
    TwoTrackDCACut();
    virtual Bool_t Pass(TwoTrack* pair);
    static Int_t DCA() { return 0; };
    static Int_t DCAxy() { return 1; };
    static Int_t DCAz() { return 2; };
    virtual ~TwoTrackDCACut();
    ClassDef(TwoTrackDCACut, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACKDCACUT_H_ */
