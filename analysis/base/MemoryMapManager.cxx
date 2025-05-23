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
    fInterface(nullptr),
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
     * call update counters (eventCol, kTRUE)
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
      uevent->Build(fCurrentEvent, fCompression);
      // we need to rebuild links
      for (int iTrack = 0; iTrack < uevent->GetTotalTrackNo(); iTrack++) {
        Track* track = uevent->GetTrack(iTrack);
        track->TranslateLinks(fCompression);
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
    fTrackMapSize = size;
#ifdef HAL_DEBUG
    Cout::PrintInfo("Reloading maps", EInfo::kDebugInfo);
#endif
  }

  void MemoryMapManager::SetMixSize(Int_t mix_size) { fMixSize = mix_size; }

  void MemoryMapManager::ManualUpdate(Event* event) { event->Update(fInterface); }

  Event* MemoryMapManager::GetTemporaryEvent() {
    if (!fDirectAcces) fCurrentEvent->Update(fInterface);
    if (fCurrentEvent->GetTotalTrackNo() > fTrackMapSize) {
      for (int i = 0; i < fEventCollectionsNo; i++)
        ReloadMap(fCurrentEvent->GetTotalTrackNo() * 1.2);
    }
    return fCurrentEvent;
  }

  void MemoryMapManager::Init(Int_t event_factor, Int_t task_id, Bool_t compress, std::vector<TString> direct) {
    if (direct.size() > 0) {
      fDirectAcces = kTRUE;
    } else {
      fDirectAcces = kFALSE;
    }
    fFormatID       = task_id;
    fUseCompression = compress;
    if (event_factor > 1) {
      Int_t* old_map      = fEventToTrackNo;
      Int_t realEventCol  = fEventCollectionsNo;
      fEventCollectionsNo = fEventCollectionsNo * event_factor;
      fEventToTrackNo     = new Int_t[fEventCollectionsNo];
      for (int i = 0; i < realEventCol; i++) {
        for (int j = 0; j < event_factor; j++)
          fEventToTrackNo[j + i * event_factor] = old_map[i];
        fMaxTrackCollectionNo = TMath::Max(fEventToTrackNo[i], fMaxTrackCollectionNo);
      }
      delete[] old_map;
    }

    fEvents = new EventArray*[fEventCollectionsNo];
    for (Int_t i = 0; i < fEventCollectionsNo; i++)
      fEvents[i] = new EventArray(fFormatID, fMixSize);

    DataFormatManager* dataManager = DataFormatManager::Instance();
    if (fDirectAcces == kFALSE) {
      fCurrentEvent = dataManager->GetNewEvent(fFormatID, EFormatDepth::kNonBuffered);
      fInterface    = fCurrentEvent->CreateInterface();
      fInterface->ConnectToTree(Hal::EventInterface::eMode::kRead);
    } else {
      for (auto brName : direct) {
        Event* ev = dynamic_cast<Event*>(DataManager::Instance()->GetObject(brName));
        if (ev) {
          fCurrentEvent = ev;
          break;
        }
      }
      if (!fCurrentEvent) { Hal::Cout::PrintInfo("Cannot find a event in provided branch list", EInfo::kCriticalError); }
    }
    fTotalTracks  = new Int_t[fEventCollectionsNo];
    fTrackMap     = new Array_4<Int_t>();
    fTrackCounter = new Array_3<Int_t>();
    fTrackCounter->MakeBigger(fEventCollectionsNo, fTrackCollectionsNo, fMixSize);  // TODO swap order
    fTrackMap->MakeBigger(fEventCollectionsNo, fTrackCollectionsNo, fMixSize, fTrackMapSize);
    fReadyToMix = new Bool_t[fEventCollectionsNo];
    for (int i = 0; i < fEventCollectionsNo; i++)
      fReadyToMix[i] = kFALSE;
    fCounter = new Int_t[fEventCollectionsNo];
    for (int i = 0; i < fEventCollectionsNo; i++)
      fCounter[i] = -1;
  }

  Int_t MemoryMapManager::GetTracksNo(Int_t eventCol, Int_t trackCol) const {
    return fTrackCounter->Get(eventCol, trackCol, fCounter[eventCol]);
    // return fTrackCounter[eventCol][trackCol][
    // fCounter[eventCol] ];
  }

  Int_t MemoryMapManager::GetRawTracksNo(Int_t eventCol) const { return fTotalTracks[eventCol]; }

  Bool_t MemoryMapManager::IsReadyToMixing(Int_t collection) const { return fReadyToMix[collection]; }

  void MemoryMapManager::ResetEventCollection(Int_t collection) {
    fCounter[collection] = -1;
    for (int i = 0; i < fEventToTrackNo[collection]; i++) {
      for (int j = 0; j < fMixSize; j++)
        fTrackCounter->Set(collection, i, j, 0);
    }
    for (Int_t i = 0; i < fMixSize; i++)
      fEvents[collection]->At(i)->Clear("C");
    fReadyToMix[collection] = kFALSE;
  }

  void MemoryMapManager::AddTrackToMapTrack(Int_t eventCol, Int_t trackCol, Int_t index) {
    Int_t counter = fCounter[eventCol];
    fTrackMap->Set(eventCol, trackCol, counter, fTrackCounter->IncrementAfter(eventCol, trackCol, counter), index);
  }

  void MemoryMapManager::ResetTrackMaps(Int_t eventCol, Int_t counter) {
    for (int i = 0; i < fTrackCollectionsNo; i++)
      fTrackCounter->Set(eventCol, i, counter, 0);
  }

  void MemoryMapManager::ResetTrackMaps(Int_t eventCol, Int_t trackCol, Int_t counter) {
    // fTrackCounter[eventCol][trackCol][counter]=0;
    fTrackCounter->Set(eventCol, trackCol, counter, 0);
  }

  Int_t MemoryMapManager::GetTracksNo(Int_t eventCol, Int_t trackCol, Int_t counter) const {
    // return fTrackCounter[eventCol][trackCol][counter];
    return fTrackCounter->Get(eventCol, trackCol, counter);
  }

  Int_t MemoryMapManager::GetTrackCollectionsNo(Int_t eventCol) const { return fEventToTrackNo[eventCol]; }

  Int_t MemoryMapManager::GetTemporaryTotalTracksNo() const { return fCurrentEvent->GetTotalTrackNo(); }

  Event* MemoryMapManager::GetEvent(Int_t collection) const { return (Event*) (fEvents[collection]->At(fCounter[collection])); }

  Track* MemoryMapManager::GetTrack(Int_t eventCol, Int_t trackCol, Int_t index) const {
    //(fEvents[eventCol]->At(fCounter[eventCol])
    //)->GetTrack(track,fTrackMap[eventCol][trackCol][fCounter[eventCol]][index]
    //);
    return (fEvents[eventCol]->At(fCounter[eventCol]))->GetTrack(fTrackMap->Get(eventCol, trackCol, fCounter[eventCol], index));
  }

  Track* MemoryMapManager::GetRawTrack(Int_t eventCol, Int_t index) const {
    return (fEvents[eventCol]->At(fCounter[eventCol]))->GetTrack(index);
  }

  Event* MemoryMapManager::GetEvent(Int_t collection, Int_t counter) const { return (Event*) (fEvents[collection]->At(counter)); }

  Track* MemoryMapManager::GetTrack(Int_t eventCol, Int_t trackCol, Int_t counter, Int_t index) const {
    return (fEvents[eventCol]->At(counter))->GetTrack(fTrackMap->Get(eventCol, trackCol, counter, index));
  }

  Track* MemoryMapManager::GetRawTrack(Int_t eventCol, Int_t counter, Int_t index) const {
    return (fEvents[eventCol]->At(counter))->GetTrack(index);
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

  void MemoryMapManager::RejectLastEvent(Int_t eventCol) {
    for (int i = 0; i < fTrackCollectionsNo; i++)
      fTrackCounter->Set(eventCol, i, fCounter[eventCol], 0);
    --fCounter[eventCol];
  }

  Int_t MemoryMapManager::GetCounter(Int_t eventCol) const { return fCounter[eventCol]; }

  void MemoryMapManager::ClearEvent() {
    // need to be called  before next Exec() or lead to memory leak
    fCurrentEvent->Clear();
  }

  void MemoryMapManager::CalculateCompressedMap(Int_t eventCol) {
    int total_tracks = fCurrentEvent->GetTotalTrackNo();
    int collections  = fEventToTrackNo[eventCol];

    Int_t counter = fCounter[eventCol];
    fCompression.Reset(total_tracks);

    std::vector<int> links;
    links.resize(fCurrentEvent->GetMaxExpectedLinks());
    // index global map - if fIndexMap[i=>0 need to store particle
    for (int iTrackCollection = 0; iTrackCollection < collections; iTrackCollection++) {
      for (int j = 0; j < GetTracksNo(eventCol, iTrackCollection); j++) {
        Int_t index  = fTrackMap->Get(eventCol, iTrackCollection, counter, j);
        Track* track = fCurrentEvent->GetTrack(index);
        Int_t size   = track->GetLinksFast(links, kTRUE);
        for (int iLink = 0; iLink < size; iLink++)
          fCompression.MarkAsGood(links[iLink]);
      }
    }
    fCompression.Recalculate();
    for (int iTrackCollection = 0; iTrackCollection < collections; iTrackCollection++) {
      auto submap = fTrackMap->At(eventCol)->At(iTrackCollection)->At(counter);
      for (int iTrack = 0; iTrack < GetTracksNo(eventCol, iTrackCollection); iTrack++) {
        Int_t old_index = submap->Get(iTrack);
        submap->Set(iTrack, fCompression.GetNewIndex(old_index));
      }
    }

#ifdef MAPMANAGER_DEBUG
    std::cout << "  collections at" << counter << std::endl;
    for (int i = 0; i < collections; i++) {
      std::cout << "Collection " << i << " end = " << End[i] << std::endl;
      for (int j = 0; j < fTrackCounter->Get(eventCol, i, counter); j++) {
        std::cout << Form(" %4d", fTrackMap->Get(eventCol, i, counter, j));
      }
      std::cout << std::endl;
    }
    std::cout << "compression" << std::endl;
#endif

#ifdef MAPMANAGER_DEBUG
    std::cout << "SUMM" << std::endl;
    for (int i = 0; i < fSumMapSize; i++) {
      //	std::cout<<Form(" %4d",fTrackMap->Get(eventCol,0,counter,i));
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
    delete fTrackCounter;
    delete fTrackMap;
    delete[] fReadyToMix;
    delete[] fTotalTracks;
    delete[] fEventToTrackNo;
    delete[] fCounter;
    if (fInterface) delete fInterface;
  }

  EventArray::EventArray(Int_t task_id, Int_t size) : fSize(size) {
    DataFormatManager* dataManager = DataFormatManager::Instance();
    fArray                         = new Event*[fSize];
    for (int i = 0; i < fSize; i++)
      fArray[i] = dataManager->GetNewEvent(task_id, EFormatDepth::kBuffered);
  }

  Track* MemoryMapManager::GetTemporaryTrack(Int_t eventCol, Int_t trackCol, Int_t index) const {
    return fCurrentEvent->GetTrack(fTrackMap->Get(eventCol, trackCol, fCounter[eventCol], index));
  }
}  // namespace Hal
