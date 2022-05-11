/*
 * TrackTpcHitsCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKTPCHITCUT_H_
#define HALTRACKTPCHITCUT_H_

#include "TrackExpCut.h"
namespace Hal {
  class TrackTpcHitCut : public TrackExpCut {
  public:
    TrackTpcHitCut();
    virtual Bool_t Init(Int_t format_id = 0);
    virtual Bool_t Pass(Track* tr);
    virtual ~TrackTpcHitCut();
    ClassDef(TrackTpcHitCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKTPCHITCUT_H_ */
