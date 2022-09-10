/*
 * Reader.h
 *
 *  Created on: 24 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_BASE_READER_H_
#define HAL_ANALYSIS_BASE_READER_H_

#include "Task.h"

/**
 * class for reading data, creates the "HalEvent" branch with event with given format
 * that to this other tasks can use "direct access" that avoid neccessity of conversion format from
 * tree to Hal::Event based
 */
namespace Hal {
  class Event;
  class EventInterface;
  class Reader : public Task {
    UInt_t fProcessedEvents;
    Event* fEvent;
    EventInterface* fInterface;

  public:
    Reader();
    virtual Task::EInitFlag Init();
    virtual void Exec(Option_t* opt);
    /**
     * set format
     * @param format any class that inherit from NIcaEventAna and ican be read
     * from currently processed file
     */
    virtual void SetFormat(Event* format);
    virtual void FinishTask();
    virtual ~Reader();
    ClassDef(Reader, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_BASE_READER_H_ */
