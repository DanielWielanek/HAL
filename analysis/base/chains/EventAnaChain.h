/*
 * EventAnaChain.h
 *
 *  Created on: 26 sie 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALEVENTANACHAIN_H_
#define HALEVENTANACHAIN_H_

#include "EventAna.h"
/**
 * Class for making analysis that require the same event selection algorithm.
 * Any event cuts in added classes "subtasks" will be ignored, only EventCuts
 * added directly to this class will be used. Classes that require track
 * buffering (like TwoTrack ) cannot be used as subtasks.
 */
namespace Hal {
  class EventAnaChain : public EventAna {
    Int_t fTaskNo;
    EventAna** fTask;  //[fTaskNo]
    void GoToDir(TString name);

  protected:
    /**
     * remove task from chain
     * @param no
     */
    virtual void RemoveTask(Int_t no);
    virtual void ProcessEvent();
    virtual Package* Report() const;
    /**
     * overwrite ana data holders with this data holders
     * @param ana
     */
    virtual void LinkTask(EventAna* ana) const;
    /**
     * disconnect ana data holders with this data holders
     * @param ana
     */
    virtual void UnlinkTask(EventAna* ana) const;
    /**
     * synchronize cut monitors and cuts
     * @param ana
     * @param end - if yes then links to higher cuts are linked - this value
     * should be true before ana->Init() and false later
     */
    virtual void SynchronizeCutContainers(EventAna* ana, Bool_t end) const;
    virtual Task::EInitFlag Init();

  public:
    EventAnaChain();
    virtual void Exec(Option_t* opt);
    /**
     * add subtask
     * @param ana subtask
     */
    virtual void AddAnalysis(EventAna* ana);
    virtual void FinishTask();
    virtual ~EventAnaChain();
    ClassDef(EventAnaChain, 1)
  };
}  // namespace Hal
#endif /* HALEVENTANACHAIN_H_ */
