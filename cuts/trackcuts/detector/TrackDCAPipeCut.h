/*
 * TrackDCAPipeCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKDCAPIPECUT_H_
#define HALTRACKDCAPIPECUT_H_
#include "TrackExpCut.h"
namespace Hal {
  class TrackDCAPipeCut : public TrackExpCut {
  public:
    enum ParID { DCA = 0, DCAxy = 1, DCAz = 2 };
    TrackDCAPipeCut();
    virtual Bool_t Pass(Track* track);
    virtual Bool_t Init(Int_t id_format = 0);
    virtual ~TrackDCAPipeCut();
    ClassDef(TrackDCAPipeCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKDCAPIPECUT_H_ */
