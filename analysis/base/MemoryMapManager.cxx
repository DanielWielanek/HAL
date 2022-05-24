/*
 * MemoryMapManager.cxx
 *
 *  Created on: 28-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "MemoryMapManager.h"

#include "Cout.h"
#include "CutCollection.h"
#include "CutContainer.h"
#include "DataFormatManager.h"
#include "DataManager.h"
#include "Link.h"
#include "Std.h"
#include "Track.h"

#include <TMathBase.h>
#include <TString.h>
#include <iostream>


namespace Hal {
  MemoryMapManager::MemoryMapManager() :
    fUseCompression(kFALSE),
    fDirectAcces(kFALSE),
    fTrackMapSize(4000),
    fFormatID(0),
    fEventCollectionsNo(1),
    fTrackCollectionsNo(1),
    fMixSize(1),
    fEventToTrackNo(0),
    fCounter(nullptr),
    fTotalTracks(nullptr),
    fReadyToMix(nullptr),
    fEvents(nullptr),
    fSumMap(nullptr),
    fIndexMap(nullptr),
    fSumMapSize(0),
    fMaxTrackCollectionNo(0),
    fCurrentEvent(nullptr),
    fTrackMap(nullptr),
    fTrackCounter(nullptr) {}

  MemoryMapManager::MemoryMapManager(CutContainer* cont) : MemoryMapManager() {
    fEventCollectionsNo = cont->GetEventCollectionsNo();
    fTrackCollectionsNo = cont->GetTrackCollectionsNo();
    fEventToTrackNo     = new Int_t[fEventCollectionsNo];
    /*\
     * this stuff is needed to check if all track collections belong to given
     * event collections have some particles to avoid e.g. mixing empty kaon
     * events with full pion events in non-id femto this checking is provide if
     * call update counters (event_collection, kTRUE)
     */
    for (int i = 0; i < fEventCollectionsNo; i++) {
      fEventToTrackNo[i]    = cont->GetEventCollection(i)->GetNextNo();
      fMaxTrackCollectionNo = TMath::Max(fEventToTrackNo[i], fMaxTrackCollectionNo);
    }
  }

  void MemoryMapManager::BufferEvent(Int_t collection) {
    if (fUseCompression) {
      CalculateCompressedMap(collection);
      Event* uevent = fEvents[collection]->At(fCounter[collection]);
      uevent->Build(fCurrentEvent, fSumMap, fIndexMap, fSumMapSize);
      // we need to rebuild links
      for (int iTrack = 0; iTrack < uevent->GetTotalTrackNo(); iTrack++) {
        Track* track = uevent->GetTrack(iTrack);
        track->TranslateLinks(fIndexMap);
        // track->SetThisID(iTrack);
      }
      fTotalTracks[collection] = uevent->GetTotalTrackNo();
    } else {
      Event* uevent = fEvents[collection]->At(fCounter[collection]);
      uevent->Build(fCurrentEvent);
      fTotalTracks[collection] = uevent->GetTotalTrackNo();
    }
#ifdef MEMORYMAMANGER_COMPRESSION_DEBUG_
    std::cout << "MEMORY MANAGER DEBUG MAPS" << std::endl;
    std::cout << "ORYGINAL EVENT" << std::endl;
    Event* temp = GetTemporaryEvent();
    int total   = temp->GetTotalTrackNo();
    if (total > 50) total = 50;
    for (int i = 0; i < total; i++) {
      Track* tr = temp->GetTrack(i);
      std::cout << tr->GetId() << " " << tr->GetLinks()->GetLinks(0) << std::endl;
    }
    std::cout << "COPIED EVENT" << std::endl;
    temp  = fEvents[collection]->At(fCounter[collection]);
    total = temp->GetTotalTrackNo();
    if (total > 50) total = 50;
    for (int i = 0; i < total; i++) {
      Track* tr = temp->GetTrack(i);
      std::cout << tr->GetId() << " " << tr->GetLinks()->GetLinks(0) << std::endl;
    }
    std::cout << "==============" << std::endl;
#endif
  }
  //-----------------------------
  void MemoryMapManager::ReloadMap(Int_t size) {
    fTrackMap->MakeBigger(-1, -1, -1, size);
    delete[] fSumMap;
    delete[] fIndexMap;
    fSumMap       = new Int_t[size];
    fIndexMap     = new Int_t[size];
    fTrackMapSize = size;
#ifdef HAL_DEBUG
    Cout::PrintInfo("Reloading maps", EInfo::kLessInfo);
#endif
  }

  void MemoryMapManager::SetMixSize(Int_t mix_size) { fMixSize = mix_size; }

  Event* MemoryMapManager::GetTemporaryEvent() {
    if (!fDirectAcces) fCurrentEvent->Update();
    if (fCurrentEvent->GetTotalTrackNo() > fTrackMapSize) {
      for (int i = 0; i < fEventCollectionsNo; i++) {
        ReloadMap(fCurrentEvent->GetTotalTrackNo() * 1.2);
      }
    }
    return fCurrentEvent;
  }

  void MemoryMapManager::Init(Int_t task_id, Bool_t use_source, Bool_t compress, Bool_t direct) {
    fDirectAcces    = direct;
    fFormatID       = task_id;
    fUseCompression = compress;
    fEvents         = new EventArray*[fEventCollectionsNo];
    for (Int_t i = 0; i < fEventCollectionsNo; i++) {
      fEvents[i] = new EventArray(fFormatID, fMixSize);
    }
    if (use_source) {
      for (Int_t i = 0; i < fEventCollectionsNo; i++) {
        for (Int_t j = 0; j < fMixSize; j++) {
          fEvents[i]->fArray[j]->CreateSource();
        }
      }
    }
    DataFormatManager* dataManager = DataFormatManager::Instance();
    if (fDirectAcces == kFALSE) {
      fCurrentEvent = dataManager->GetEventFromTree(fFormatID);
    } else {
      fCurrentEvent = (Event*) DataManager::Instance()->GetObject(dataManager->GetFormat(fFormatID)->ClassName());
    }
    fTotalTracks  = new Int_t[fEventCollectionsNo];
    fSumMap       = new Int_t[fTrackMapSize];
    fIndexMap     = new Int_t[fTrackMapSize];
    fSumMapSize   = 0;
    fTrackMap     = new Array_4<Int_t>();
    fTrackCounter = new Array_3<Int_t>();
    fTrackCounter->MakeBigger(fEventCollectionsNo, fTrackCollectionsNo, fMixSize);
    fTrackMap->MakeBigger(fEventCollectionsNo, fTrackCollectionsNo, fMixSize, fTrackMapSize);
    fReadyToMix = new Bool_t[fEventCollectionsNo];
    for (int i = 0; i < fEventCollectionsNo; i++) {
      fReadyToMix[i] = kFALSE;
    }
    fCounter = new Int_t[fEventCollectionsNo];
    for (int i = 0; i < fEventCollectionsNo; i++) {
      fCounter[i] = -1;
    }
  }

  void MemoryMapManager::Init(Int_t event_factor, Int_t task_id, Bool_t use_source, Bool_t compress, Bool_t direct) {
    fDirectAcces    = direct;
    fFormatID       = task_id;
    fUseCompression = compress;

    Int_t* old_map      = fEventToTrackNo;
    Int_t realEventCol  = fEventCollectionsNo;
    fEventCollectionsNo = fEventCollectionsNo * event_factor;
    fEventToTrackNo     = new Int_t[fEventCollectionsNo];
    for (int i = 0; i < realEventCol; i++) {
      for (int j = 0; j < event_factor; j++) {
        fEventToTrackNo[j + i * event_factor] = old_map[i];
      }
      fMaxTrackCollectionNo = TMath::Max(fEventToTrackNo[i], fMaxTrackCollectionNo);
    }
    delete[] old_map;

    fEvents = new EventArray*[fEventCollectionsNo];
    for (Int_t i = 0; i < fEventCollectionsNo; i++) {
      fEvents[i] = new EventArray(fFormatID, fMixSize);
    }
    if (use_source) {
      for (Int_t i = 0; i < fEventCollectionsNo; i++) {
        for (Int_t j = 0; j < fMixSize; j++) {
          fEvents[i]->fArray[j]->CreateSource();
        }
      }
    }
    DataFormatManager* dataManager = DataFormatManager::Instance();
    if (fDirectAcces == kFALSE) {
      fCurrentEvent = dataManager->GetEventFromTree(fFormatID);
    } else {
      fCurrentEvent = (Event*) DataManager::Instance()->GetObject(dataManager->GetFormat(fFormatID)->ClassName());
    }
    fTotalTracks  = new Int_t[fEventCollectionsNo];
    fSumMap       = new Int_t[fTrackMapSize];
    fIndexMap     = new Int_t[fTrackMapSize];
    fSumMapSize   = 0;
    fTrackMap     = new Array_4<Int_t>();
    fTrackCounter = new Array_3<Int_t>();
    fTrackCounter->MakeBigger(fEventCollectionsNo, fTrackCollectionsNo, fMixSize);
    fTrackMap->MakeBigger(fEventCollectionsNo, fTrackCollectionsNo, fMixSize, fTrackMapSize);
    fReadyToMix = new Bool_t[fEventCollectionsNo];
    for (int i = 0; i < fEventCollectionsNo; i++) {
      fReadyToMix[i] = kFALSE;
    }
    fCounter = new Int_t[fEventCollectionsNo];
    for (int i = 0; i < fEventCollectionsNo; i++) {
      fCounter[i] = -1;
    }
  }

  Int_t MemoryMapManager::GetTracksNo(Int_t event_collection, Int_t track_collection) const {
    return fTrackCounter->Get(event_collection, track_collection, fCounter[event_collection]);
    // return fTrackCounter[event_collection][track_collection][
    // fCounter[event_collection] ];
  }

  Int_t MemoryMapManager::GetRawTracksNo(Int_t event_collection) const { return fTotalTracks[event_collection]; }

  Bool_t MemoryMapManager::IsReadyToMixing(Int_t collection) const { return fReadyToMix[collection]; }

  void MemoryMapManager::ResetEventCollection(Int_t collection) {
    fCounter[collection] = -1;
    for (int i = 0; i < fEventToTrackNo[collection]; i++) {
      for (int j = 0; j < fMixSize; j++) {
        fTrackCounter->Set(collection, i, j, 0);
      }
    }
    for (Int_t i = 0; i < fMixSize; i++)
      fEvents[collection]->At(i)->Clear("C");
    fReadyToMix[collection] = kFALSE;
  }

  void MemoryMapManager::AddTrackToMapTrack(Int_t event_collection, Int_t track_collection, Int_t index) {
    Int_t counter = fCounter[event_collection];
    fTrackMap->Set(event_collection,
                   track_collection,
                   counter,
                   fTrackCounter->IncrementAfter(event_collection, track_collection, counter),
                   index);
  }

  void MemoryMapManager::ResetTrackMaps(Int_t event_collection, Int_t counter) {
    for (int i = 0; i < fTrackCollectionsNo; i++)
      fTrackCounter->Set(event_collection, i, counter, 0);
  }

  void MemoryMapManager::ResetTrackMaps(Int_t event_collection, Int_t track_collection, Int_t counter) {
    // fTrackCounter[event_collection][track_collection][counter]=0;
    fTrackCounter->Set(event_collection, track_collection, counter, 0);
  }

  Int_t MemoryMapManager::GetTracksNo(Int_t event_collection, Int_t track_collection, Int_t counter) const {
    // return fTrackCounter[event_collection][track_collection][counter];
    return fTrackCounter->Get(event_collection, track_collection, counter);
  }

  Int_t MemoryMapManager::GetTrackCollectionsNo(Int_t event_collection) const { return fEventToTrackNo[event_collection]; }

  Int_t MemoryMapManager::GetTemporaryTotalTracksNo() const { return fCurrentEvent->GetTotalTrackNo(); }

  Event* MemoryMapManager::GetEvent(Int_t collection) const { return (Event*) (fEvents[collection]->At(fCounter[collection])); }

  Track* MemoryMapManager::GetTrack(Int_t event_collection, Int_t track_collection, Int_t index) const {
    //(fEvents[event_collection]->At(fCounter[event_collection])
    //)->GetTrack(track,fTrackMap[event_collection][track_collection][fCounter[event_collection]][index]
    //);
    return (fEvents[event_collection]->At(fCounter[event_collection]))
      ->GetTrack(fTrackMap->Get(event_collection, track_collection, fCounter[event_collection], index));
  }

  Track* MemoryMapManager::GetRawTrack(Int_t event_collection, Int_t index) const {
    return (fEvents[event_collection]->At(fCounter[event_collection]))->GetTrack(index);
  }

  Event* MemoryMapManager::GetEvent(Int_t collection, Int_t counter) const { return (Event*) (fEvents[collection]->At(counter)); }

  Track* MemoryMapManager::GetTrack(Int_t event_collection, Int_t track_collection, Int_t counter, Int_t index) const {
    return (fEvents[event_collection]->At(counter))->GetTrack(fTrackMap->Get(event_collection, track_collection, counter, index));
  }

  Track* MemoryMapManager::GetRawTrack(Int_t event_collection, Int_t counter, Int_t index) const {
    return (fEvents[event_collection]->At(counter))->GetTrack(index);
  }

  void MemoryMapManager::PrintMap() const {
    Event* ptr  = NULL;
    Track* ptr1 = NULL;
    for (int ev_trig = 0; ev_trig < fEventCollectionsNo; ev_trig++) {
      for (int count = 0; count < fMixSize; count++) {
        // if(i!=2) continue;
        std::cout << Form("Event %d collection %d", count, ev_trig) << std::endl;
        ptr = GetEvent(ev_trig, count);
        ptr->Print();

        for (int tr_trig = 0; tr_trig < fTrackCollectionsNo; tr_trig++) {
          std::cout << Form("buffered %i", GetTracksNo(ev_trig, tr_trig, count)) << std::endl;
          for (int k = 0; k < GetTracksNo(ev_trig, tr_trig, count); k++) {
            // for(int k=0;k<ptr->GetTotalTrackNo();k++){
            //	GetBufferedRawTrack(ptr1,j,i,k);
            Int_t pos = fTrackMap->Get(ev_trig, tr_trig, count, k);
            // Int_t pos = fTrackMap[ev_trig][tr_trig][count][k];
            ptr1 = GetRawTrack(ev_trig, count, pos);
            std::cout << Form("%i val %i", k, pos) << std::endl;
            ptr1->Print();
            ptr1 = GetTrack(ev_trig, tr_trig, count, k);
            ptr1->Print();
          }
        }
      }
    }
  }

  void MemoryMapManager::RejectLastEvent(Int_t event_collection) {
    for (int i = 0; i < fTrackCollectionsNo; i++)
      fTrackCounter->Set(event_collection, i, fCounter[event_collection], 0);
    --fCounter[event_collection];
  }

  Int_t MemoryMapManager::GetCounter(Int_t event_collection) const { return fCounter[event_collection]; }

  void MemoryMapManager::ClearEvent() {
    // need to be called  before next Exec() or lead to memory leak
    fCurrentEvent->Clear();
  }

  void MemoryMapManager::CalculateCompressedMap(Int_t event_collection) {
    int total_tracks = fCurrentEvent->GetTotalTrackNo();
    int collections  = fEventToTrackNo[event_collection];

    Int_t counter = fCounter[event_collection];
    for (int i = 0; i < total_tracks; i++) {
      fIndexMap[i] = -1;
    }
    // index global map - if fIndexMap[i=>0 need to store particle
    for (int iTrackCollection = 0; iTrackCollection < collections; iTrackCollection++) {
      for (int j = 0; j < GetTracksNo(event_collection, iTrackCollection); j++) {
        Int_t index            = fTrackMap->Get(event_collection, iTrackCollection, counter, j);
        Track* track           = fCurrentEvent->GetTrack(index);
        std::vector<int> links = track->GetLinks();
        for (int iLink = 0; iLink < (int) links.size(); iLink++) {
          fIndexMap[links[iLink]] = 0;
        }
      }
    }
    Int_t id = -1;
    for (int iTrack = 0; iTrack < total_tracks; iTrack++) {
      if (fIndexMap[iTrack] >= 0) {
        fIndexMap[iTrack] = ++id;  // new index like -1 0 -1 1 -1 -1 2 etc.
      }
    }
    fSumMapSize = id + 1;
    for (int iTrackCollection = 0; iTrackCollection < collections; iTrackCollection++) {
      for (int iTrack = 0; iTrack < GetTracksNo(event_collection, iTrackCollection); iTrack++) {
        Int_t old_index    = fTrackMap->Get(event_collection, iTrackCollection, counter, iTrack);
        Int_t new_index    = fIndexMap[old_index];
        fSumMap[new_index] = old_index;
        fTrackMap->Set(event_collection, iTrackCollection, counter, iTrack, new_index);
      }
    }

#ifdef MAPMANAGER_DEBUG
    std::cout << "  collections at" << counter << std::endl;
    for (int i = 0; i < collections; i++) {
      std::cout << "Collection " << i << " end = " << End[i] << std::endl;
      for (int j = 0; j < fTrackCounter->Get(event_collection, i, counter); j++) {
        std::cout << Form(" %4d", fTrackMap->Get(event_collection, i, counter, j));
      }
      std::cout << std::endl;
    }
    std::cout << "compression" << std::endl;
#endif

#ifdef MAPMANAGER_DEBUG
    std::cout << "SUMM" << std::endl;
    for (int i = 0; i < fSumMapSize; i++) {
      //	std::cout<<Form(" %4d",fTrackMap->Get(event_collection,0,counter,i));
    }
    std::cout << std::endl;
    for (int i = 0; i < fSumMapSize; i++) {
      std::cout << Form(" %4d", fSumMap[i]);
    }
    std::cout << std::endl;
#endif
  }

  void MemoryMapManager::PrepareMaps(Int_t collection) {
    Int_t dif = fMixSize - fCounter[collection];
    switch (dif) {
      case 1: fCounter[collection] = -1; break;
      case 2: fReadyToMix[collection] = kTRUE; break;
      default: break;
    }
    ++fCounter[collection];
    ResetTrackMaps(collection, fCounter[collection]);
  }

  MemoryMapManager::~MemoryMapManager() {
    if (fEvents) {
      for (int i = 0; i < fEventCollectionsNo; i++)
        delete fEvents[i];
      delete[] fEvents;
    }
    delete[] fSumMap;
    delete fTrackCounter;
    delete fTrackMap;
    delete[] fReadyToMix;
    delete[] fTotalTracks;
    delete[] fEventToTrackNo;
    delete[] fCounter;
    delete[] fIndexMap;
  }

  EventArray::EventArray(Int_t task_id, Int_t size) : fSize(size) {
    DataFormatManager* dataManager = DataFormatManager::Instance();
    fArray                         = new Event*[fSize];
    for (int i = 0; i < fSize; i++) {
      fArray[i] = dataManager->GetNewEvent(task_id, EFormatDepth::kBuffered);
    }
  }

  Track* MemoryMapManager::GetTemporaryTrack(Int_t event_collection, Int_t track_collection, Int_t index) const {
    return fCurrentEvent->GetTrack(fTrackMap->Get(event_collection, track_collection, fCounter[event_collection], index));
  }
}  // namespace Hal
