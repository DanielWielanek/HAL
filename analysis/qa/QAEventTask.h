/*
 * QAEventTask.h
 *
 *  Created on: 05-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_ANALYSIS_QA_HALQAEVENTTASK_H_
#define HALFEMTO_ANALYSIS_QA_HALQAEVENTTASK_H_

#include "QAPlot.h"

#include "EventAna.h"

#include <TObjArray.h>


/**
 * task for simple QA for event, base on event ana task, therefore user
 * can select objects with cuts
 */

namespace Hal {
  class QAEventTask : public EventAna {
    TObjArray* fEventQA;
    QAPlot* fTempEventPlot;
    std::vector<TString> fEventColNames;

  protected:
    QAPlot* GetQAPlot(Int_t i) const { return (QAPlot*) fEventQA->UncheckedAt(i); };
    virtual Task::EInitFlag Init();
    virtual void ProcessEvent();
    virtual Package* Report() const;

  public:
    QAEventTask();
    void SetEventCollectionNames(const std::initializer_list<TString>& init);
    void SetQAPlot(const QAPlot& plot);
    virtual ~QAEventTask();
    QAEventTask(const QAEventTask& other);
    QAEventTask& operator=(const QAEventTask& other);
    ClassDef(QAEventTask, 1)
  };
}  // namespace Hal
#endif /* HALFEMTO_ANALYSIS_QA_HALQAEVENTTASK_H_ */
