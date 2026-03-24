/*
 * OTFEfficiency.h
 *
 *  Created on: 11 lut 2026
 *      Author: daniel
 */

#ifndef HAL_EXAMPLES_ONTHEFLY_EXPERIMENT_OTFEFFICIENCY_H_
#define HAL_EXAMPLES_ONTHEFLY_EXPERIMENT_OTFEFFICIENCY_H_

#include <TH2.h>

#include "OTFExperimentSubTask.h"


namespace HalOTF {
  class RecoTrack;
  /**
   * simulate efficiency of detector modify number of hits for given detector:
   * 0 - particle not detected
   * 1 - particle detected
   * if not modified usually nhits = 2
   */
  class EfficientyKin : public ExperimentSubTask {
  protected:
    TH2D* fAcceptance = {nullptr};
    Char_t fDetType   = {'A'};
    /**
     * changes number of hits
     * @param tr track
     * @param flag new number of hits
     */
    void ChangeHits(HalOTF::RecoTrack* tr, Int_t flag) const;

  public:
    EfficientyKin(std::vector<int> vec = {0}) : ExperimentSubTask(vec) {};
    void SetHistogram(const TH2D& histo);
    void SetDetectorType(Char_t type) { fDetType = type; }
    virtual Bool_t Init();
    Hal::Package* Report() const;
    virtual ~EfficientyKin();
    ClassDef(EfficientyKin, 1)
  };


  class EfficiencyYPt : public EfficientyKin {
  protected:
    virtual void ProcessTrack(Hal::ComplexTrack* track);

  public:
    EfficiencyYPt(std::vector<int> vec = {0}) : EfficientyKin(vec) {};
    virtual ~EfficiencyYPt() {};
    ClassDef(EfficiencyYPt, 1)
  };

  class EfficiencyEtaPt : public EfficientyKin {
  protected:
    virtual void ProcessTrack(Hal::ComplexTrack* track);

  public:
    EfficiencyEtaPt(std::vector<int> vec = {0}) : EfficientyKin(vec) {};
    virtual ~EfficiencyEtaPt() {};
    ClassDef(EfficiencyEtaPt, 1)
  };
} /* namespace HalOTF */

#endif /* HAL_EXAMPLES_ONTHEFLY_EXPERIMENT_OTFEFFICIENCY_H_ */
