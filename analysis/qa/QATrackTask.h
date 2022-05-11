/*
 * QATrackTask.h
 *
 *  Created on: 14 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_ANALYSIS_QA_HALQATRACKTASK_H_
#define HALFEMTO_ANALYSIS_QA_HALQATRACKTASK_H_

#include "QAPlot.h"

#include "TrackAna.h"

/**
 * class for QA plots, works similar to the standard TrackAnalysis
 */

namespace Hal {
  class QATrackTask : public TrackAna {
    TObjArray* fEventQA;
    QAPlot* fTempEventPlot;
    TObjArray* fTrackQA;
    QAPlot* fTempTrackPlot;
    std::vector<TString> fEventColNames;
    std::vector<TString> fTrackColNames;

  protected:
    virtual Task::EInitFlag Init();
    virtual void ProcessEvent();
    virtual void ProcessTrack();
    virtual void LinkCollections();
    virtual Package* Report() const;
    QAPlot* GetEventQAPlot(Int_t i) const { return (QAPlot*) fEventQA->UncheckedAt(i); };
    QAPlot* GetTrackQAPlot(Int_t i) const { return (QAPlot*) fTrackQA->UncheckedAt(i); };

  public:
    QATrackTask();
    void SetEventCollectionNames(const std::initializer_list<TString>& init);
    void SetTrackCollectionNames(const std::initializer_list<TString>& init);
    void SetQAPlot(const QAPlot& plot);
    virtual ~QATrackTask();
    QATrackTask(const QATrackTask& other);
    QATrackTask& operator=(const QATrackTask& other);
    ClassDef(QATrackTask, 1)
  };
}  // namespace Hal
#endif /* HALFEMTO_ANALYSIS_QA_HALQATRACKTASK_H_ */
