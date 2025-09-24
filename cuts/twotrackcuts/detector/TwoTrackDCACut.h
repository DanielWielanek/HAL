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
    enum ParID { DCA = 0, DCAxy = 1, DCAz = 2 };
    TwoTrackDCACut();
    virtual Bool_t Pass(TwoTrack* pair);
    virtual ~TwoTrackDCACut();
    ClassDef(TwoTrackDCACut, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACKDCACUT_H_ */
