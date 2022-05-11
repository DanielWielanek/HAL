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
    TrackDCAPipeCut();
    virtual Bool_t Pass(Track* track);
    virtual Bool_t Init(Int_t id_format = 0);
    static Int_t DCA() { return 0; };
    static Int_t DCAxy() { return 1; };
    static Int_t DCAz() { return 2; };
    virtual ~TrackDCAPipeCut();
    ClassDef(TrackDCAPipeCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKDCAPIPECUT_H_ */
