/*
 * TrackMcCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKMCCUT_H_
#define HALTRACKMCCUT_H_

#include "TrackCut.h"

namespace Hal {

  class TrackMcCut : public TrackCut {
  public:
    TrackMcCut(const Int_t size = 1);
    virtual Bool_t Init(Int_t task_id);
    virtual ~TrackMcCut();
    ClassDef(TrackMcCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKMCCUT_H_ */
