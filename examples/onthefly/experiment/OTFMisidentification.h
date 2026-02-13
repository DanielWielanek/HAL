/*
 * OTFMisidentification.h
 *
 *  Created on: 13 lut 2026
 *      Author: daniel
 */

#ifndef HAL_EXAMPLES_ONTHEFLY_EXPERIMENT_OTFMISIDENTIFICATION_H_
#define HAL_EXAMPLES_ONTHEFLY_EXPERIMENT_OTFMISIDENTIFICATION_H_

#include <TH2.h>
#include <vector>

#include "OTFExperimentSubTask.h"


namespace HalOTF {
  /**
   * apply identification according to probablitiy historgram
   */
  class MisidentificationKin : public ExperimentSubTask {
  protected:
    std::vector<TH2D*> fHypos;
    std::vector<Int_t> fPids;
    Int_t GetNewPid(Double_t x, Double_t y) const;
    virtual void ProcessTrack(Hal::ComplexTrack* track) = 0;

  public:
    MisidentificationKin(std::vector<int> ids = {0}) : ExperimentSubTask(ids) {};
    /**
     * set probability histogram
     * @param h
     * @param pid
     */
    void AddPidHypo(const TH2D& h, Int_t pid);
    /**
     * normalize probabilities (don't worry will be called by init)
     */
    void Normalize();
    /**
     * add missindefined probabilities (by using sum of already set pid histograms - note they must be normalized)
     */
    void AddMisidenfiedAuto();
    virtual Bool_t Init();
    virtual ~MisidentificationKin();
    ClassDef(MisidentificationKin, 1)
  };
  /**
   * apply identification according to probablitiy historgram OX - rapidity, OY - pt
   */
  class MisidentificationYPt : public MisidentificationKin {
  protected:
    virtual void ProcessTrack(Hal::ComplexTrack* track);

  public:
    MisidentificationYPt(std::vector<int> ids = {0}) : MisidentificationKin(ids) {};
    virtual Bool_t Init() { return MisidentificationKin::Init(); };
    virtual ~MisidentificationYPt() {};
    ClassDef(MisidentificationYPt, 1)
  };
  /**
   * apply identification according to probablitiy historgram OX - eta, OY - pt
   */
  class MisidentificationEtaPt : public MisidentificationKin {
  protected:
    virtual void ProcessTrack(Hal::ComplexTrack* track);

  public:
    MisidentificationEtaPt(std::vector<int> ids = {0}) : MisidentificationKin(ids) {};
    virtual Bool_t Init() { return MisidentificationKin::Init(); };
    virtual ~MisidentificationEtaPt() {};
    ClassDef(MisidentificationEtaPt, 1)
  };

} /* namespace HalOTF */

#endif /* HAL_EXAMPLES_ONTHEFLY_EXPERIMENT_OTFMISIDENTIFICATION_H_ */
