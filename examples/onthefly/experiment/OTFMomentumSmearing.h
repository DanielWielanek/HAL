/*
 * OTFMomentumSmearing.h
 *
 *  Created on: 13 lut 2026
 *      Author: daniel
 */

#ifndef HAL_EXAMPLES_ONTHEFLY_EXPERIMENT_OTFMOMENTUMSMEARING_H_
#define HAL_EXAMPLES_ONTHEFLY_EXPERIMENT_OTFMOMENTUMSMEARING_H_

#include <vector>

#include "OTFExperimentSubTask.h"

class TLorentzVector;
namespace Hal {
  class Track;
  class LorentzSmearing;
} /* namespace Hal */

class TH2D;

class TF1;


namespace HalOTF {
  /**
   * smears momenta according to given algo
   */
  class MomentumSmearing : public ExperimentSubTask {
    Hal::LorentzSmearing* fSmearing = {nullptr};

  protected:
    TLorentzVector GetNewMomenta(Hal::Track* track) const;
    virtual void ProcessTrack(Hal::ComplexTrack* track);

  public:
    MomentumSmearing(std::vector<int> vec = {0}) : ExperimentSubTask(vec) {};
    /**
     * set smearing algorithm
     * @param algo
     */
    void SetSmearingAlgo(const Hal::LorentzSmearing& algo);
    virtual Bool_t Init();
    virtual ~MomentumSmearing();
    ClassDef(MomentumSmearing, 1)
  };
} /* namespace HalOTF */

#endif /* HAL_EXAMPLES_ONTHEFLY_EXPERIMENT_OTFMOMENTUMSMEARING_H_ */
