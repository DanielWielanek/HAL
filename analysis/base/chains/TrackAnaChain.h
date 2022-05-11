/*
 * TrackAnaChain.h
 *
 *  Created on: 26 sie 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef BASE_HALTRACKANACHAIN_H_
#define BASE_HALTRACKANACHAIN_H_

#include "TrackAna.h"
/**
 * Class for making analysis where track and event selection can be common. All
 * event and track cuts added to subtasks will be ingored, only cuts added
 * directly to chain will be used in this chain.
 */

namespace Hal {
  class TrackAnaChain : public TrackAna {
    Int_t fTaskNo;
    TrackAna** fTask;  //[fTaskNo]
    void GoToDir(TString name);

  protected:
    /**
     * remove subtask
     * @param no subtask number
     */
    virtual void RemoveTask(Int_t no);
    virtual void ProcessEvent();
    virtual Package* Report() const;
    /**
     * overwrite ana data holders with this data holders
     * @param ana
     */
    void LinkTask(TrackAna* ana) const;
    /**
     * disconnect ana data holders with this data holders
     * @param ana
     */
    void UnlinkTask(TrackAna* ana) const;
    /**
     * synchronize cut monitors and cuts
     * @param ana
     * @param end - if yes then links to higher cuts are copied - this value
     * should be true before ana->Init() and false later
     */
    void SynchronizeCutContainers(TrackAna* ana, Bool_t end) const;
    virtual Task::EInitFlag Init();

  public:
    TrackAnaChain();
    virtual void Exec(Option_t* opt);
    /**
     * add subtask to analysis
     * @param ana
     */
    virtual void AddAnalysis(TrackAna* ana);
    virtual void FinishTask();
    virtual ~TrackAnaChain();
    ClassDef(TrackAnaChain, 1)
  };
}  // namespace Hal
#endif /* BASE_HALTRACKANACHAIN_H_ */
