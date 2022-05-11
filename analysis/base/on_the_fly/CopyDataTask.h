/*
 * CopyDataTask.h
 *
 *  Created on: 06-11-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCOPYDATATASK_H_
#define HALCOPYDATATASK_H_

#include "Task.h"

namespace Hal {
  class Event;

  /**
   * task for copying data
   */
  class CopyDataTask : public Task {
    Event* fInputData;

  protected:
    /**
     * initialization
     * @return
     */
    virtual Task::EInitFlag Init();

  public:
    CopyDataTask();
    /**
     * set format of data to copy
     * @param event event with type of data that should be copied
     */
    virtual void SetFormat(Event* event);
    /**
     * function called for each event
     * @param opt not used
     */
    virtual void Exec(Option_t* opt);
    virtual ~CopyDataTask();
    ClassDef(CopyDataTask, 1)
  };
}  // namespace Hal

#endif /* HALCOPYDATATASK_H_ */
