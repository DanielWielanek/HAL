/*
 * EventRunCut.h
 *
 *  Created on: 30 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_CUTS_EVENTCUTS_DETECTOR_EVENTRUNCUT_H_
#define HAL_CUTS_EVENTCUTS_DETECTOR_EVENTRUNCUT_H_

#include "EventExpCut.h"

#include <TString.h>

#include <initializer_list>
#include <vector>

/**
 * event that pass good runs only or reject bad runs only,
 * this depends on mode, default mode is rejecting listed runs
 */
namespace Hal {

  class EventRunCut : public EventExpCut {
  protected:
    TString fListFile;
    TString fLabel;
    std::vector<int> fRuns;
    enum class EMode { kGood, kBad };
    EMode fMode;

  public:
    EventRunCut();
    void SetRunList(std::initializer_list<int> list);
    void SetModeGoodRuns() { fMode = EMode::kGood; }
    void SetModeBadRuns() { fMode = EMode::kBad; }
    void SetDataLabel(TString label) { fLabel = label; }
    void SetInputFile(TString name) { fListFile = name; }
    virtual Bool_t Init(Int_t format_id);
    virtual Bool_t Pass(Event* event);
    virtual void Print(Option_t* opt = "") const;
    virtual Package* Report() const;
    virtual ~EventRunCut() {};
    ClassDef(EventRunCut, 1)
  };


}  // namespace Hal

#endif /* HAL_CUTS_EVENTCUTS_DETECTOR_EVENTRUNCUT_H_ */
