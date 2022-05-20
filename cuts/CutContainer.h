/*
 * CutContainer.h
 *
 *  Created on: 06-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALCUTCONTAINER_H_
#define HALCUTCONTAINER_H_

#include <TObjArray.h>
#include <TString.h>

#include "Cut.h"
#include "CutCollection.h"
#include "CutMonitor.h"
#include "Std.h"

//#define SHOW_CUT_INIT
/**
 * class for storing all cuts and cut monitors used in analysis
 */
namespace Hal {
  class CutContainer : public TObject {
    Bool_t fInit;
    Int_t fSize;
    // TObjArray *fTempCutContainers;
    TObjArray** fTempCutMonitors;  //[fSize]
    TObjArray** fCutContainers;    //[fSize]
    void VerifyOrder(TObjArray* obj);
    void ExtractComplexMonitor(CutMonitor* mon, TString& opt);
    void MakeComplexAxis(CutMonitor* mon, Int_t axis, Int_t opt);
    Bool_t ExtractRegExp(const Cut& cut, Option_t* opt);
    Bool_t ExtrackRegExp2(const Cut& cut, Option_t* opt);
    Bool_t ExtractRegExp(const CutMonitor& cut, Option_t* opt);
    Bool_t ExtractRegExp2(const CutMonitor& cut, Option_t* opt);
    Bool_t CheckTwoTracksOptions(const CutMonitor& cutmon, Option_t* opt);
    Bool_t CheckTwoTracksOptions(const Cut& cutmon, Option_t* opt);

  protected:
    inline TObjArray* GetCutContainer(ECutUpdate upd) const { return (TObjArray*) fCutContainers[static_cast<Int_t>(upd)]; };

  public:
    /**
     * for linking policy
     */
    enum class ELinkPolicy { kOneToMany, kEqual, kReplicateLast, kReplicateFirst, kAnyToAny };
    /**
     * default constructor that shouldn't be used
     */
    CutContainer();
    /**
     * copy constructor
     * @param cont
     */
    CutContainer(const CutContainer& cont);
    /**
     * proper default constructor
     * @param tiers_no the highest update ratio of cuts used in analysis
     */
    CutContainer(ECutUpdate tiers_no);
    /**
     * initialize container
     * @param task_id id of used format
     */
    void Init(const Int_t task_id = 0);
    /**
     * add cut to this
     * @param cut cut to add
     * @param opt option of adding cut
     */
    void AddCut(const Cut& cut, Option_t* opt = " ");
    /**
     * Currently not used
     */
    void InitReport() const;
    /**
     * add cut monitor to this
     * @param monitor monitor to add
     * @param opt option of adding
     */
    void AddMonitor(const CutMonitor& monitor, Option_t* opt = " ");
    /**
     * link collections
     * @param opt_low update ratio for linking from
     * @param in_low collection number for linking from
     * @param opt_high update ratio for linking to
     * @param in_high collection number for linking to
     */
    void LinkCollections(ECutUpdate opt_low, Int_t in_low, ECutUpdate opt_high, Int_t in_high);
    /**
     * copy cut sub-container with given collection to new sub-container and add
     * to this cut cutnaier
     * @param type update ration (tier) of sub-container that should be copied
     * @param collection_no number collection number of subcontainer that should
     * be copied
     * @param new_collection_no number of collection of new sub-container
     * @param option currently not used
     */
    void ReplicateCollection(ECutUpdate type, Int_t collection_no, Int_t new_collection_no, Option_t* option = "");
    /**
     * remove given sub-container (collection) from this container
     * @param update update ration of removed sub-container
     * @param collection collection number of removed sub-container
     */
    void RemoveCollection(ECutUpdate update, Int_t collection);
    /**
     * copy cut containers with given collection from other cut container to this
     * cut container, those copies are marked as "dummy" so they contains pointers
     * to cuts and cuts monitors from other instead of copies, method useful for
     * making "chain analysis"
     * @param update specify witch cuts and cut monitors will be copied
     * @param other cut monitor that is source of cuts for copying
     * @param copy_link - copy links between different layers of cuts
     */
    void MakeDummyCopies(ECutUpdate update, CutContainer* other, Bool_t copy_link);
    /**
     * add virtual cut for given collection
     */
    void AddVirtualCut(ECutUpdate update, Int_t col);
    /**
     * return event collections no for given update ratio
     */
    Int_t GetCollectionsNo(ECutUpdate update) const;
    /**
     *
     * @return number of event collections in this
     */
    Int_t GetEventCollectionsNo() const;
    /**
     *
     * @return number of track collections in this
     */
    Int_t GetTrackCollectionsNo() const;
    /**
     *
     * @return number of two-track collections in this
     */
    Int_t GetTwoTrackCollectionsNo() const;
    /**
     *
     * @return number of background two-track collections in this
     */
    Int_t GetTwoTrackCollectionsBackgroundNo() const;
    /**
     *
     */
    Bool_t LinkCollections(ECutUpdate first, ECutUpdate last, ELinkPolicy policy);
    /**
     *
     * @param collection collection no
     * @return given event collection
     */
    inline CutCollection* GetEventCollection(Int_t collection) const {
      return (CutCollection*) fCutContainers[static_cast<Int_t>(ECutUpdate::kEvent)]->UncheckedAt(collection);
      ;
    };
    /**
     *
     * @param collection collection no
     * @return given track collection
     */
    inline CutCollection* GetTrackCollection(Int_t collection) const {
      return (CutCollection*) fCutContainers[static_cast<Int_t>(ECutUpdate::kTrack)]->UncheckedAt(collection);
    };
    /**
     *
     * @param collection collection no
     * @return given two-track collection
     */
    inline CutCollection* GetTwoTrackCollection(Int_t collection) const {
      return (CutCollection*) fCutContainers[static_cast<Int_t>(ECutUpdate::kTwoTrack)]->UncheckedAt(collection);
    };
    /**
     *
     * @param collection collection no
     * @return given background two-track collection
     */
    inline CutCollection* GetTwoTrackBackgroundCollection(Int_t collection) const {
      return (CutCollection*) fCutContainers[static_cast<Int_t>(ECutUpdate::kTwoTrackBackground)]->UncheckedAt(collection);
    };
    /**
     *
     * @return size of cut container (highest tier number/update ratio)
     */
    inline Int_t GetSize() const { return fSize; };
    /**
     * check event cuts
     * @param event event to check
     * @param collection event collection number used for checking
     * @return true if passed false otherwise
     */
    inline Bool_t PassEvent(Event* event, const Int_t collection) {
      return ((CutCollection*) fCutContainers[static_cast<Int_t>(ECutUpdate::kEvent)]->UncheckedAt(collection))
        ->PassEvent(event);
    }
    /**
     * check track cuts
     * @param track track to check
     * @param collection track collection number used for checking
     * @return true if passed false otherwise
     */
    inline Bool_t PassTrack(Track* track, const Int_t collection) {
      return ((CutCollection*) fCutContainers[static_cast<Int_t>(ECutUpdate::kTrack)]->UncheckedAt(collection))
        ->PassTrack(track);
    };
    /**
     * check pair cuts
     * @param pair pair pair of tracks  to check
     * @param collection pair collection number used for checking
     * @return true if passed false otherwise
     */
    inline Bool_t PassSignalPair(TwoTrack* pair, const Int_t collection) {
      return ((CutCollection*) fCutContainers[static_cast<Int_t>(ECutUpdate::kTwoTrack)]->UncheckedAt(collection))
        ->PassTwoTrack(pair);
    };
    /**
     * check pair cuts
     * @param pair pair pair of tracks  to check
     * @param collection pair collection number used for checking
     * @return true if passed false otherwise
     */
    inline Bool_t PassBackgroundPair(TwoTrack* pair, const Int_t collection) {
      return ((CutCollection*) fCutContainers[static_cast<Int_t>(ECutUpdate::kTwoTrackBackground)]->UncheckedAt(collection))
        ->PassTwoTrack(pair);
    };
    /**
     *
     * @return report from this cut container
     */
    virtual Package* Report() const;
    virtual ~CutContainer();
    ClassDef(CutContainer, 1)
  };
}  // namespace Hal
#endif /* HALCUTCONTAINER_H_ */
