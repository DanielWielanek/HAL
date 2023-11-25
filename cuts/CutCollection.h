/*
 * CutCollection.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALCUTSUBCONTAINER_H_
#define HALCUTSUBCONTAINER_H_

#include "Cut.h"

#include "Array.h"

#include <TObjArray.h>
#include <TString.h>
#include <vector>

#include "Std.h"

namespace Hal {
  class CutMonitor;
  class Event;
  class Track;
  class TwoTrack;

  /**
   * basic class for storing all cuts and cut monitors used for one group of
   * objects, called also collection or cut sub-container
   */
  class CutCollection : public TObject {
  private:
    TObjArray* fCutMonitors;
    TObjArray *fCuts, *fFastCuts;
    ULong64_t fPassedSlow, fFailedSlow, fPassedFast, fFailedFast;
    Int_t fFastCutsNo;
    Int_t fSlowCutsNo;
    Int_t fCutMonitorsNo;
    Array_2<Int_t>* fPreviousAddress;
    Array_2<Int_t>* fNextAddress;
    Array_1<Int_t>* fNextNo;
    Array_1<Int_t>* fPreviousNo;
    ECutUpdate fMode;  // mode event/ track. two track
    Bool_t fInit, fDummy;
    Int_t fCollectionID, fContainerSize, fStep;
    TObjArray** fCutContainerArr;  //[fContainerSize]
    Bool_t fNextUsed, fPrevUsed;
    /**
     * update cut monitors
     * @param val value to update - true if object pass, false otherwise
     * @param obj object used in analysis (for old-style cut monitors)
     */
    void Update(Bool_t val, TObject* obj);
    /**
     * initialize cut monitors by looking for monitored cuts
     * @param task_id
     */
    void AdvancedMonitorInitialization(Int_t task_id);
    /**
     *  look for cut that have the same address like address in other cut sub
     * container
     * @param adreess cut to check
     * @param input cut container to check
     * @return -1 if there is no cut with given adress, position of cut otherwise
     */
    Int_t MatchCut(Cut* address, CutCollection* input);
    /**
     * look for cuts with given names in collections with smaller update ratio
     * (tier) but linked with this cut container
     * @param cut name of cut to find
     * @return array of found cuts
     */
    TObjArray* LocateInLowerCollections(TString cut);
    /**
     * look for cuts with given name in this collection or look in lower
     * collections that are linked with this collection
     * @param classname name looked for monitor
     * @return array of found cuts
     */
    TObjArray* LocateCuts(TString classname);
    /**
     * check if cuts are compatibile
     * @param cut1 first cut to check
     * @param cut2 second cut to check
     * @return true if cuts have the same collection number and update ratio,
     * false otherwise
     */
    Bool_t CheckIfComptatiblie(Cut* cut1, Cut* cut2);
    /**
     *
     * @param cutname cut name (not cut-class name)
     * @return cut update ratio
     */
    ECutUpdate GetUpdateFromName(TString cutname) const;
    inline TObjArray* GetCutContainerArray(ECutUpdate upd) const {
      return fCutContainerArr[static_cast<Int_t>(upd)];
      ;
    }

  public:
    /**
     * Constructor used by CutCollection
     * @param container pointer to array with other sub-containers
     * @param cont_size size of cut-container array
     * @param update update ratio of this cut sub-container
     * @param collectionNo collection number of this cut sub-container
     */
    CutCollection(TObjArray** container, Int_t cont_size, ECutUpdate update, Int_t collectionNo);
    /**
     * default constructor for streamer
     */
    CutCollection();
    /**
     * create copy of this cut container that can be used by another task
     * @param container array of cut containers
     * @return
     */
    CutCollection* MakeNewCopy(TObjArray** container) const;
    /**
     * initialize this cut container
     * @param task_id of processed events
     */
    void Init(Int_t task_id);
    /**
     * not used
     */
    void PrintInfo() const;
    /**
     * set "step of actualization", normalny it's equal to one, this means that we
     * are going from events to tracks from tracks to pairs / background pairs
     * this should be set to 2 if we want to go from particles to three particle
     * system
     * @param step
     */
    void SetStep(Int_t step) { fStep = step; };
    /**
     * add cut to this collection
     * @param cut cut
     * @param opt option of adding (at this level only "fast" option can be used
     */
    void AddCut(Cut* cut, Option_t* opt = " ");
    /**
     * mark this cut container as dummy (all cuts ad cuts monitor are removed and
     * will not be used anymore
     */
    void MarkAsDummy();
    /**
     * add cut monitor
     * @param monitor - monitor to add
     */
    void AddCutMonitor(CutMonitor* monitor);
    /**
     * add link to higher collection
     * @param value collection number used for linking
     * @param background true if link point to background collections
     */
    void AddNextAddr(Int_t value, Bool_t background = kFALSE);
    /**
     * add link to lower collection
     * @param value collection number used for linking
     * @param background true if link point to background collections
     */
    void AddPreviousAddr(Int_t value, Bool_t background = kFALSE);
    /**
     * overwrite all objects in this class by object from copy, such objects are
     * not removed if copy is not deleted
     * @param copy original cut subconainer
     * @param copy_high_links if true links from to higher cut container (with
     * bigger update ratio) is also copied
     */
    void MakeDummyCopy(const CutCollection* copy, Bool_t copy_high_links = kTRUE);
    /**
     * compare to CutCollection class object
     * @param obj obj to compare
     * @return flag for comparison used by ROOT
     */
    Int_t Compare(const TObject* obj) const;
    /**
     *
     * @param opt "fast" if user want get values only for fast cuts, otherwise
     * value of slow cuts is returned
     * @return number of objects that failed cuts
     */
    ULong64_t GetFailed(Option_t* opt = "") const;
    /**
     *
     * @param opt "fast" if user want get values only for fast cuts, only for fast
     * cuts, otherwise value of slow cuts is returned
     * @return number of objects that passed cuts
     */
    ULong64_t GetPassed(Option_t* opt = "") const;
    /**
     *
     * @return number of "slow" cuts
     */
    Int_t GetCutNo() const { return fCuts->GetEntriesFast(); };
    /**
     *
     * @return collection number of this object
     */
    Int_t GetCollectionID() const { return fCollectionID; };
    /**
     *
     * @return number of "fast" cuts
     */
    Int_t GetFastCutNo() const { return fFastCuts->GetEntriesFast(); };
    /**
     *
     * @return number of cut monitors
     */
    Int_t GetCutMonitorNo() const { return fCutMonitors->GetEntriesFast(); };
    /**
     * check event cuts
     * @param obj event to check
     * @return true if object ok, false if object shoudn't pass cuts
     */
    Bool_t PassEvent(Event* obj);
    /**
     * check track cuts
     * @param obj track to check
     * @return true if object ok, false if object shoudn't pass cuts
     */
    Bool_t PassTrack(Track* obj);
    /**
     * check pair cuts
     * @param pair pair to check
     * @return true if object ok, false if object shoudn't pass cuts
     */
    Bool_t PassTwoTrack(TwoTrack* pair);
    /**
     *  for ROOT sorting algorithms
     * @return true
     */
    Bool_t IsSortable() const { return kTRUE; };
    /**
     * In some analysis like EventAnaChain it's possible to use subtasks. In
     * such case common cust subcontainer is marked as a "dummy". Cuts and cut
     * monitoris in such containers might belong for more than one task, so
     * modyfing them might lead to problems. Other values (like links) might be
     * modified.
     *
     * @return true if dummy
     */
    Bool_t IsDummy() const { return fDummy; };
    /**
     * return slow cut with given position
     * @param index cut position
     * @return slow cut
     */
    Cut* GetCut(Int_t index) const { return (Cut*) fCuts->UncheckedAt(index); };
    /**
     * return fast cut with given position
     * @param index cut position
     * @return fast cut
     */
    Cut* GetFastCut(Int_t index) const { return (Cut*) fFastCuts->UncheckedAt(index); };
    /**
     * return cut with given name
     * @param name cut name (name of class)
     * @return cut
     */
    Cut* FindCut(TString name) const;
    /**
     *
     * @param index cut monitor index
     * @return cut monitor
     */
    CutMonitor* GetCutMonitor(Int_t index) const;
    /**
     * copy this cut container with new collection number
     * @param new_collection collection number of copy
     * @return copy of this
     */
    CutCollection* Replicate(Int_t new_collection) const;
    /**
     *
     * @return number of links to lower collections
     */
    inline Int_t GetPrevNo() const { return fPreviousNo->Get(0); };
    /**
     *
     * @return number of links to higher collections
     */
    inline Int_t GetNextNo() const { return fNextNo->Get(0); };
    /**
     * return value of given link that points to lower sub-containers
     * @param index number of link
     * @return number of collection (lower sub-container linked with this)
     */
    inline Int_t GetPrevAddr(Int_t index) const { return fPreviousAddress->Get(0, index); };
    /**
     * return value of given link that points to higher sub-containers
     * @param index number of link
     * @return number of collection (higher sub-container linked with this)
     */
    inline Int_t GetNextAddr(Int_t index) const { return fNextAddress->Get(0, index); };
    /**
     *
     * @return total number for lower tiers sub-containers in background mode
     */
    inline Int_t GetPrevNoBackground() const { return fPreviousNo->Get(1); };
    /**
     *
     * @return total number for higher tiers sub-containers in background mode
     */
    inline Int_t GetNextNoBackround() const { return fNextNo->Get(1); };
    /**
     * return value of given link that points to lower sub-containers and are
     * background sub-containers
     * @param index number of link
     * @return number of collection (lower sub-container linked with this)
     */
    inline Int_t GetPrevAddBackround(Int_t index) const { return fPreviousAddress->Get(1, index); };
    /**
     * return value of given link that points to higher sub-containers that are
     * also background sub-containers
     * @param index number of link
     * @return number of collection (higher sub-container linked with this)
     */
    inline Int_t GetNextAddrBackround(Int_t index) const { return fNextAddress->Get(1, index); };
    /**
     *
     * @return report from this
     */
    virtual Package* Report() const;
    virtual ~CutCollection();
    ClassDef(CutCollection, 1)
  };
}  // namespace Hal
#endif /* HALCUTSUBCONTAINER_H_ */
