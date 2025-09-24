/*
 * TrackDCACut.h
 *
 *  Created on: 27 lut 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKDCACUT_H_
#define HALTRACKDCACUT_H_

#include "TrackCut.h"

namespace Hal {
  class TrackDCACut : public TrackCut {
  public:
    enum ParID { DCA = 0, DCAxy = 1, DCAz = 2 };
    TrackDCACut();
    virtual Bool_t Pass(Track* track);
    virtual Bool_t Init(Int_t id_format = 0);
    virtual ~TrackDCACut();
    ClassDef(TrackDCACut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKDCACUT_H_ */
