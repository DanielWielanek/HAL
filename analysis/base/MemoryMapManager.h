/*
 * MemoryMapManager.h
 *
 *  Created on: 28-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef HALMEMORYMAPMANAGER_H_
#define HALMEMORYMAPMANAGER_H_


#include "Array.h"
#include "CompressionMap.h"
#include "Event.h"

namespace Hal {
  class IOManager;
  class CutContainer;
  class MiniEventTask;
  /**
   * class for storing  fake events
   */
  class EventArray : public TObject {
  public:
    /**
     * size of array
     */
    Int_t fSize;
    /**
     * array of events
     */
    Event** fArray;  //[fSize]
    /**
     * default ctor
     */
    EventArray(Int_t task_id, Int_t size = 1);
    /**
     * return pointer ot fake event
     * @param size position of event into array
     * @return
     */
    inline Event* At(const Int_t size) { return fArray[size]; };
    /**
     * default d-tor
     */
    virtual ~EventArray() {
      for (int i = 0; i < fSize; i++)
        delete fArray[i];
      delete[] fArray;
    };
    ClassDef(EventArray, 1)
  };
  /**
   * class for managing buffered events
   */
  class MemoryMapManager : public TObject {
    friend class MiniEventTask;

  private:
    Bool_t fUseCompression;
    Bool_t fDirectAcces;
    Int_t fTrackMapSize, fFormatID;
    Int_t fEventCollectionsNo, fTrackCollectionsNo, fMixSize;
    Int_t* fEventToTrackNo;  //[fEventCollectionsNo]
    Int_t* fCounter;         //[fEventCollectionsNo]
    Int_t* fTotalTracks;     //[fEventCollectionsNo]
    Bool_t* fReadyToMix;     //[fEventCollectionsNo]
    EventArray** fEvents;    //[fEventCollectionsNo]
    EventInterface* fInterface;
    //"true" collections x mix_size
    CompressionMap fCompression;
    Int_t fMaxTrackCollectionNo;
    Event* fCurrentEvent;  // pointer to processed event
    Array_4<Int_t>* fTrackMap;
    Array_3<Int_t>* fTrackCounter;
    void ReloadMap(Int_t size);
    void CalculateCompressedMap(Int_t eventCol);
    Int_t GetTrackCollectionsNo(Int_t eventCol) const;
    Int_t GetCounter(Int_t eventCol, Int_t trackCol) const;
    /**
     * reset track maps for all track collections for given event
     * @param eventCol event collection
     * @param counter position of event in buffer
     */
    void ResetTrackMaps(Int_t eventCol, Int_t counter);
    /**
     * reset track maps
     * @param eventCol event collection
     * @param trackCol track collection
     * @param counter event position in buffer
     */
    void ResetTrackMaps(Int_t eventCol, Int_t trackCol, Int_t counter);

  public:
    /**
     * empty constructor (not used)
     */
    MemoryMapManager();
    /**
     * default constructor
     * @param const pointer to CutContainer with cuts used in analysis
     */
    MemoryMapManager(CutContainer* cont);
    /**
     * return pointer to current event
     * @param update - when true update event by values from tree
     */
    Event* GetTemporaryEvent();
    /**
     *  call update on this event
     * @param event
     */
    void ManualUpdate(Event* event);
    /**
     * set size of mixing buffer
     * @param mix_size mixing buffer size
     */
    void SetMixSize(Int_t mix_size);
    /**
     *
     * @return number of track in currently processed event (stored in tree)
     */
    Int_t GetTemporaryTotalTracksNo() const;
    /**
     * initialize this manager
     * @param event_factor - numer of artificial multiplication of event triggers
     * @param task_id data format (and  task id)
     * @param compress - if true then evets will be compressed
     * @param direct  -name of branch if direct access is used
     * is performed
     */
    void Init(Int_t event_factor, Int_t task_id, Bool_t compress, std::vector<TString> direct);
    /**
     * add track to map
     * @param eventCol even collection number
     * @param trackCol track collection number
     * @param index position of added track in track array
     */
    void AddTrackToMapTrack(Int_t eventCol, Int_t trackCol, Int_t index);
    /**
     * buffer event with memory
     * @param collection collection number of event
     */
    void BufferEvent(Int_t collection);
    /**
     * return processed event (from buffer)
     * @param collection - event collection
     * @return event
     */
    Event* GetEvent(Int_t collection) const;
    /**
     * fill track from buffered event (last buffered)
     * @param eventCol event collection number
     * @param trackCol track collection number
     * @param index position in map
     * @return track
     */
    Track* GetTrack(Int_t eventCol, Int_t trackCol, Int_t index) const;
    /**
     *
     * @param trackCol track collection
     * @param index number
     * @return track from temporary event via map
     */
    Track* GetTemporaryTrack(Int_t eventCol, Int_t trackCol, Int_t index) const;
    /**
     * get track from buffered event (last buffered)
     * @param eventCol event collection no
     * @param index position in track array
     * @return track
     */
    Track* GetRawTrack(Int_t eventCol, Int_t index) const;
    /**
     * @param collection event collection number
     * @param counter position in event buffer
     * @return event
     */
    Event* GetEvent(Int_t collection, Int_t counter) const;
    /**
     * return track from event
     * @param eventCol event collection number
     * @param trackCol track collection number
     * @param counter position of event in buffer
     * @param index position of particle in map
     * @return track from event
     */
    Track* GetTrack(Int_t eventCol, Int_t trackCol, Int_t counter, Int_t index) const;
    /**
     * return raw track from event
     * @param eventCol event collection number
     * @param counter position of event in buffer
     * @param index position of particle in track array
     * @return raw track (without using maps)
     */
    Track* GetRawTrack(Int_t eventCol, Int_t counter, Int_t index) const;
    /**
     * reject last event from buffer
     * @param eventCol event collection of rejected event
     */
    void RejectLastEvent(Int_t eventCol);
    /**
     * reset buffer, track maps and event buffer become empty
     * @param collection event collection number
     */
    void ResetEventCollection(Int_t collection);
    /**
     * clear event stored in tree
     */
    void ClearEvent();
    /**
     * Prepare maps of tracks for event that potentially will be buffered
     * @param collection
     */
    void PrepareMaps(Int_t collection);
    /**
     * print map of tracks
     */
    void PrintMap() const;
    /**
     *
     * @param eventCol event collection number
     * @return position of currently processed (or last buffered) event in array
     */
    Int_t GetCounter(Int_t eventCol) const;
    /**
     * return number of track in latest buffered event (or current)
     * @param eventCol event collection no
     * @param trackCol track collection no
     * @return number of tracks in map
     */
    Int_t GetTracksNo(Int_t eventCol, Int_t trackCol) const;
    /**
     *
     * @param eventCol event collection no
     * @param trackCol track collection no
     * @param counter event position
     * @return number of tracks
     */
    Int_t GetTracksNo(Int_t eventCol, Int_t trackCol, Int_t counter) const;
    /**
     *
     * @param eventCol event collection number
     * @return  number of tracks in current (or latest buffered) event
     */
    Int_t GetRawTracksNo(Int_t eventCol) const;
    /**
     * check if can mix events
     * @param collection event collection no
     * @return true if buffer is full
     */
    Bool_t IsReadyToMixing(Int_t collection) const;
    virtual ~MemoryMapManager();
    ClassDef(MemoryMapManager, 1)
  };
}  // namespace Hal
#endif /* HALMEMORYMAPMANAGER_H_ */
