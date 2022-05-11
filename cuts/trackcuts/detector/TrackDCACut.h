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
    TrackDCACut();
    virtual Bool_t Pass(Track* track);
    virtual Bool_t Init(Int_t id_format = 0);
    static Int_t DCA() { return 0; };
    static Int_t DCAxy() { return 1; };
    static Int_t DCAz() { return 2; };
    virtual ~TrackDCACut();
    ClassDef(TrackDCACut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKDCACUT_H_ */
