/*
 * OTFExperimentSubTask.h
 *
 *  Created on: 11 lut 2026
 *      Author: daniel
 */

#ifndef HAL_EXAMPLES_ONTHEFLY_EXPERIMENT_OTFEXPERIMENTSUBTASK_H_
#define HAL_EXAMPLES_ONTHEFLY_EXPERIMENT_OTFEXPERIMENTSUBTASK_H_

#include <RtypesCore.h>
#include <TObject.h>
#include <vector>

namespace Hal {
  class ComplexEvent;
  class ComplexTrack;
  class Package;
} /* namespace Hal */


namespace HalOTF {
  /**
   * class used to evalulate detector perfomance by modifying data
   */
  class ExperimentSubTask : public TObject {
    std::vector<int> fGeneratorIds;
    Bool_t fAll = {kFALSE};

  protected:
    enum class eMode { kSim, kReco };
    eMode fMode                                         = eMode::kSim;
    virtual void ProcessTrack(Hal::ComplexTrack* track) = 0;

  public:
    /**
     * Experiment subtask
     * @param ids ids of generators that are modified if -1 apply to all generators
     */
    ExperimentSubTask(std::vector<int> ids = {-1});
    /**
     * modify properties according to simulation data
     */
    void CorrectionOnSim() { fMode = eMode::kSim; }
    /**
     * modify properties according to reconstructed data
     */
    void CorrectionOnReco() { fMode = eMode::kReco; }
    /**
     *
     * @return
     */
    std::vector<int> GetGeneratorsId() const { return fGeneratorIds; }
    /**
     *
     * @return true if successfully initialized
     */
    virtual Bool_t Init() = 0;
    virtual void Exec(Hal::ComplexEvent* event);
    /**
     * make report (currently not used)
     * @return
     */
    virtual Hal::Package* Report() const;
    virtual ~ExperimentSubTask() {};
    ClassDef(ExperimentSubTask, 1)
  };

} /* namespace HalOTF */

#endif /* HAL_EXAMPLES_ONTHEFLY_EXPERIMENT_OTFEXPERIMENTSUBTASK_H_ */
