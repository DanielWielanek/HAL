/*
 * OTFExperiment.h
 *
 *  Created on: 11 lut 2026
 *      Author: daniel
 */

#ifndef HAL_EXAMPLES_ONTHEFLY_OTFEXPERIMENT_H_
#define HAL_EXAMPLES_ONTHEFLY_OTFEXPERIMENT_H_

#include <RtypesCore.h>
#include <vector>

#include "OTFExperimentSubTask.h"
#include "Task.h"


namespace HalOTF {
  /**
   * class for performing simple simulation effects (acceptance, momentum smearing, pid misidentification
   */
  class Experiment : public Hal::Task {
    std::vector<ExperimentSubTask*> fSubTasks;
    Hal::ComplexEvent* fCurrentEvent = {nullptr};

  public:
    Experiment();
    virtual ~Experiment();
    virtual Hal::Task::EInitFlag Init();
    virtual void Exec(Option_t* option);
    virtual void FinishTask();
    ClassDef(Experiment, 1)
  };

}  // namespace HalOTF

#endif /* HAL_EXAMPLES_ONTHEFLY_OTFEXPERIMENT_H_ */
