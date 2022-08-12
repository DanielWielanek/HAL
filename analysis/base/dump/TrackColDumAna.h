/*
 * TrackColDumAna.h
 *
 *  Created on: 12 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_BASE_DUMP_TRACKCOLDUMANA_H_
#define HAL_ANALYSIS_BASE_DUMP_TRACKCOLDUMANA_H_

#include "TrackAna.h"
/**
 * class that store hal events with original format but with preselected particles
 * it mean that only track that passed cuts are stored with Status = Status +1000*Track Col ID
 */
namespace Hal {

  class TrackColDumAna : public TrackAna {
    Event* fNewEvent = {nullptr};

  protected:
    virtual void ProcessEvent();

  public:
    TrackColDumAna();
    virtual ~TrackColDumAna();
    TrackColDumAna(const TrackColDumAna& other);
    TrackColDumAna& operator=(const TrackColDumAna& other);
    virtual Task::EInitFlag Init();
    ClassDef(TrackColDumAna, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_BASE_DUMP_TRACKCOLDUMANA_H_ */
