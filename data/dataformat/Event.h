/*
 Get * Event.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALEVENT_H_
#define HALEVENT_H_

#include "EventInterface.h"
#include "Track.h"

#include <TClonesArray.h>
#include <TNamed.h>
#include <TObjArray.h>
#include <TString.h>

#include "Std.h"

class TDatabasePDG;
namespace Hal {
  class Package;
  class McEvent;
  class ComplexEvent;
  class SmearedEvent;
  /**
   * Abstract class for storing events. It contains representation of basic
   * paramters, and Track array. \warning You should not use base constructor
   * in analysis but construtor with TString parameter In this constructor yous
   * should specify type of tracks in this event.
   *
   *  Currently all
   * classes work in folllowing way: Event-derived classes is basic class that
   * store most important inforrmations. Getters/setters here are usually inlined
   * to improve performance (crutial if analysis call getters frequently).
   * Event also  might contains so called EventInterface .
   * EventInterface based classes are used to store and acces "real event" in
   * 'oryginal structure" from tree. Event interface is always present in "current
   * event". If you don;t want to store event interface in buffered events you can
   * call "DisableSource". In such case only data from Event-based events are
   * stored in memory. Another option of compression is calling Compress() method,
   * in such case only tracks that passed cuts  are stored..
   * @see EventInterface
   */
  class Event : public TNamed {
  private:
    friend class SmearedEvent;
    friend class ComplexEvent;
    friend class Track;

  protected:
    EventInterface* fSource;  //!
    TDatabasePDG* fPDG;       //!
    TClonesArray* fTracks;
    TClonesArray* fV0sHiddenInfo;
    TLorentzVector* fVertex;
    Double_t fPhi, fPhiError;
    Int_t fEventId;
    Int_t fTotalTracksNo;
    Int_t fTotalV0s;
    /**
     *
     * @param pdg
     * @return charge of particle
     */
    Double_t CalculateCharge(Int_t pdg) const;
    /**
     * copy into this event and source (if needed)
     * @param event event to copy
     */
    void CopyData(Event* event);
    /**
     * copy into this and compress event and source (if needed)
     * @param event event to copy
     * @param map map of tracks to copy
     * @param map_ids map to convert old ID's into new
     * @param map_size size of map
     */
    void CopyCompress(Event* event, Int_t* map, Int_t* mapID, Int_t map_size);
    /**
     * compress given clones array
     * @param array array of clones to compress
     * @param map_size size of map
     */
    void Compress(TClonesArray* array, Int_t* map, Int_t map_size);
    /**
     * delete source if present
     */
    void DeleteSource();
    /**
     * copy only track data from "event" into this
     * @param event event to copy
     */
    virtual void ShallowCopyTracks(Event* event);
    /**
     * copy only track data from "event" into this
     * @param event event to copy
     * @param index_map map with indexes (used to links with oryginal tracks or
     * parents
     * @param mapID - map of track indexes
     * @param map_size sie of map
     */
    virtual void ShallowCopyCompressTracks(Event* event, Int_t* map, Int_t* mapID, Int_t map_size);
    /**
     * copy only event data from "event" into this
     * @param event event to copy
     */
    virtual void ShallowCopyEvent(Event* event);
    /**
     * copy hidden v0s
     * @param event
     */
    virtual void ShallowCopyHiddenInfo(Event* event);
    /**
     *
     * @param n number of branches, names of branches
     * @return true if all branches are present in tree
     */
    Bool_t CheckBranches(Int_t n...) const;
    /**
     * constructor used by derived classes
     * @param track_class name of track class, e.g. if you have MagicEvent
     * class with MagicTrack  your constructor should look like
     * MagicEvent():Event("MagicTrack"){... .
     */
    Event(TString track_class, TString v0_class = "Hal::V0Track");

  public:
    /**
     * default constructor - should no be used
     */
    Event();
    /**
     * copy c-tor
     * @param other
     */
    Event(const Event& other);
    /**
     * make copy of event into this object, if map is specifed then build
     * "minievent"
     * @param event event to copy (will be the same class)
     * @param map map of tracks
     * @param map_size - map size
     */
    void Build(Event* event, Int_t* map = nullptr, Int_t* mapID = nullptr, Int_t map_size = 0);
    /**
     * register this in output branch
     */
    void Register(Bool_t write);
    /**
     * register source in output format
     */
    void RegisterSource(Bool_t write);
    /**
     * creates "compressed event" by removing tracks that are not present in map
     * @param map map that contains indexes of tracks that should be copied
     * @param map_size size of this map
     */
    void Compress(Int_t* map, Int_t map_size);
    /**
     *  update fields by using "source event", it's better to implement this
     * method due to improve performance. In such case you can use directly
     * getters/setters from oryginal structure instead of calling virtual methods
     * from EventInterface
     */
    virtual void Update();
    /**
     * used for clear (usually track array)
     * @param opt option of Clear
     */
    virtual void Clear(Option_t* opt = " ");
    /**
     * set new eventID
     * @param newID
     */
    inline void SetEventID(Int_t newID) { fEventId = newID; }
    /**
     *  set event reaction plane with error
     *  @param phi rection plane
     *  @param phi_error reaction plane error
     *  */
    inline void SetPhi(Double_t phi, Double_t phi_error = 0) {
      fPhi      = phi;
      fPhiError = phi_error;
    };
    /**
     * print basic info about event
     **/
    virtual void Print(Option_t* opt = "") const;
    /**
     * boost this event and all tracks inside
     * @param vx
     * @param vy
     * @param vz
     */
    virtual void Boost(Double_t vx, Double_t vy, Double_t vz);
    /**
     * link event with event in input tree, this event will be point to currently
     * processed event
     */
    virtual void LinkWithTree() { fSource->LinkWithTree(EventInterface::eMode::kRead); };
    /**
     * fill fake track with track data
     * @param i position of track in track array
     */
    inline Track* GetTrack(Int_t i) const { return (Track*) fTracks->UncheckedAt(i); };
    /**
     *
     * @return newly added track
     */
    inline Track* AddTrack() { return (Track*) fTracks->ConstructedAt(fTracks->GetEntriesFast()); }
    /**
     *
     * @return true if needed branches exist in tree
     */
    virtual Bool_t ExistInTree() const { return kFALSE; };
    /**
     * check if this buffered and "non-buffered" format are compatible
     * this function should be reimplemented only if user want to copy data from
     * other format
     * @param buffered return true if formats are compatible
     * @return
     */
    virtual Bool_t IsCompatible(const Event* non_buffered) const;
    /**
     * this function should be reimplemented only if hidden settings are used -
     * the settings that are used during calculation of event properties
     * @return true if this format have some hidden settings that doesn't depend
     * on data stored in tree
     */
    virtual Bool_t HasHiddenSettings() const { return kFALSE; };
    /**
     *
     * @return event number
     */
    inline Int_t GetEventID() const { return fEventId; };
    /**
     *
     * @return total number of tracks in event
     */
    inline Int_t GetTotalTrackNo() const { return fTotalTracksNo; };
    /**
     *
     * @return total number of registered vo
     */
    inline Int_t GetTotalV0No() const { return fTotalV0s; };
    /**
     *
     * @return reaction plane angle
     */
    inline Double_t GetPhi() const { return fPhi; };
    /**
     *
     * @return error of reaction plane
     */
    inline Double_t GetPhiError() const { return fPhiError; };
    /**
     *
     * @return vertex position in 4D
     */
    inline TLorentzVector* GetVertex() const { return fVertex; };
    /**
     *
     * @return format type
     */
    virtual EFormatType GetFormatType() const { return EFormatType::kReco; };
    /**
     *
     * @return pointer to original event
     */
    TObject* GetEventPointer() const;
    /**
     * return pointer to track with original class
     * \warning each format should have own unique name
     * @param index index of track in track array
     * @return track at given position
     */
    TObject* GetTrackPointer(Int_t index) const;
    /**
     *
     * @return name of current format, should be reimplemented only if
     * class name doesn't uniquely define format
     */
    virtual TString GetFormatName() const;
    /**
     *
     * @return allocated track compatible with current format
     */
    Track* GetNewTrack() const;
    /**
     * for allocation event should be reimpleneted only if default constructor +
     * CopyHiddenVariabels is not enough to create complete empty new event
     * @return new object of this class
     * for SetFormat()
     */
    virtual Event* GetNewEvent() const;
    /**
     *  create source - original structure of event,  by allocating memory
     *
     *  **/
    virtual void CreateSource() = 0;
    /**
     *
     * @return pointer to used interface
     */
    inline EventInterface* GetSource() const { return fSource; };
    /**
     * creates report about this event, this should be reimplented only if
     * hidden variables/ additional options are used in event
     * @return
     */
    virtual Package* Report() const;
    /**
     * copy settings of this event - but not values stored!
     * @param event
     */
    virtual void CopyHiddenSettings(const Event* /*event*/) {};
    /**
     * return track property by ID, this should be positve number larger than 200 (for ID outside of framework)
     * @param fieldID
     * @return
     */
    virtual Float_t GetFieldVal(Int_t fieldID) const;
    /**
     * return name of the track property by ID
     * @param fieldID
     * @return
     */
    virtual TString GetFieldName(Int_t fieldID) const;
    /**
     *
     * @return pointer to hidden info
     */
    inline TClonesArray* GetV0HiddenInfo() const { return fV0sHiddenInfo; };
    /**
     * default destructor
     */
    virtual ~Event();
    ClassDef(Event, 1)
  };
}  // namespace Hal
#endif /* HALEVENT_H_ */
