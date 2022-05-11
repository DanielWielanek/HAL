/*
 * TwoTrackAnaChain.h
 *
 *  Created on: 26 sie 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTWOTRACKANACHAIN_H_
#define HALTWOTRACKANACHAIN_H_

#include "TwoTrackAna.h"

/**
 * class for making pair analysis with the same  event, track and pair
 * selection. Any cuts added to subtask will be ignored only cuts added directly
 * to this class will be used in subtasks in chain.
 */

namespace Hal {
  class TwoTrackAnaChain : public TwoTrackAna {
    Int_t fTaskNo;
    TwoTrackAna** fTask;  //[fTaskNo]
  protected:
    void RemoveTask(Int_t no);
    void ProcessEvent();
    void ProcessPair();
    void ProcessPair2();
    void ProcessPair_Mixed();
    void ProcessPair2_Mixed();
    void ProcessPair_Perfect();
    void ProcessPair2_Perfect();
    void ProcessPair_Rotated();
    void ProcessPair2_Rotated();
    void ProcessPair_Hemisphere();
    void ProcessPair2_Hemisphere();
    void ProcessPair_ChargedId();
    void ProcessPair_ChargedNId();
    void ProcessPair_Charged2();
    void ProcessPair2_Charged2();
    void ProcessPair_Charged3();
    void ProcessPair2_Charged3();
    void ProcessTrack();
    virtual void FinishEventNonIdentical();
    virtual void FinishEventIdentical();
    virtual Package* Report() const;
    /**
     * overwrite ana data holders with this data holders
     * @param ana
     */
    virtual void LinkTask(TwoTrackAna* ana) const;
    /**
     * disconnect ana data holders with this data holders
     * @param ana
     */
    virtual void UnlinkTask(TwoTrackAna* ana) const;
    /**
     * synchronize cut monitors and cuts
     * @param ana
     * @param end - if yes then links to higher cuts are linked - this value
     * should be true before ana->Init() and false later
     */
    virtual void SynchronizeCutContainers(TwoTrackAna* ana, Bool_t end) const;
    virtual Task::EInitFlag Init();

  public:
    TwoTrackAnaChain();
    /**
     * default constructor
     * @param use_background true if background must be used
     */
    TwoTrackAnaChain(Bool_t use_background);
    /**
     * add subtask to chain
     * @param ana
     */
    virtual void AddAnalysis(TwoTrackAna* ana);
    virtual void FinishTask();
    virtual ~TwoTrackAnaChain();
    ClassDef(TwoTrackAnaChain, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACKANACHAIN_H_ */
