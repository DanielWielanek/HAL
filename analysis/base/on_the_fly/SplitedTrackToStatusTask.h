/*
 * SplitedTrackToStatusTask.h
 *
 *  Created on: 27 lis 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALSPLITEDTRACKTOSTATUSTASK_H_
#define HALSPLITEDTRACKTOSTATUSTASK_H_

#include "Array.h"
#include "EventAna.h"


/**
 * set track status based on reconstruction
 * -1 - MC track not registered, reconstructed track not matched with MC
 * 0 - track reconstructed one
 * 1 - track reconstructed more than once (or MC track assigned to other track)
 */
namespace Hal {
  class SplitedTrackToStatusTask : public EventAna {
    Array_1<Int_t> fArrayReco;
    Array_1<Int_t> fArraySim;

  protected:
    virtual Task::EInitFlag Init();
    virtual void ProcessEvent();

  public:
    SplitedTrackToStatusTask();
    virtual ~SplitedTrackToStatusTask();
    SplitedTrackToStatusTask(const SplitedTrackToStatusTask& other);
    SplitedTrackToStatusTask& operator=(const SplitedTrackToStatusTask& other);
    ClassDef(SplitedTrackToStatusTask, 1)
  };
}  // namespace Hal

#endif /* HALSPLITEDTRACKTOSTATUSTASK_H_ */
