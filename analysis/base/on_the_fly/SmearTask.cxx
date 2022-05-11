/*
 * SmearTask.cxx
 *
 *  Created on: 8 gru 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "SmearTask.h"

#include "ComplexEvent.h"
#include "CutCollection.h"
#include "CutContainer.h"
#include "DataFormatManager.h"
#include "Event.h"
#include "EventAna.h"
#include "HalStd.h"
#include "MemoryMapManager.h"
#include "Package.h"
#include "SmearedEvent.h"
#include "SmearedTrack.h"

#include <FairLogger.h>


namespace Hal {
  SmearTask::SmearTask() :
    TrackAna(),
    fOutputFormatID(0),
    fSave(kFALSE),
    fEventAlgorithm(NULL),
    fTrackAlgorithm(NULL),
    fCurrentTrackSmeared(NULL),
    fCurrentEventSmeared(NULL) {
    DataFormatManager* formats = DataFormatManager::Instance();
    fOutputFormatID            = formats->RegisterFormat();
  }

  void SmearTask::CheckCutContainerCollections() {
    if (fCutContainer->GetEventCollectionsNo() > 1) {
      LOG(WARNING) << "Too many event collections, they will be removed! ";
      for (int i = 1; i < fCutContainer->GetEventCollectionsNo(); i++)
        fCutContainer->RemoveCollection(ECutUpdate::kEventUpdate, i);
    }
    TrackAna::CheckCutContainerCollections();
  }

  Package* SmearTask::Report() const {
    Package* pack  = TrackAna::Report();
    Package* pack2 = fEventAlgorithm->Report();
    Package* pack3 = fTrackAlgorithm->Report();
    pack->AddObject(pack2);
    pack->AddObject(pack3);
    return pack;
  }

  Task::EInitFlag SmearTask::Init() {
    DataFormatManager* formats = DataFormatManager::Instance();
    if (formats->FormatExist(GetTaskID())) {
      const Event* temp = formats->GetFormat(GetTaskID());
      if (temp->InheritsFrom("EventSmeared")) {
        LOG(WARNING) << "Cannot set smeared algorithm in smearing task";
        return Task::EInitFlag::kFATAL;
      }
    }
    Task::EInitFlag stat = TrackAna::Init();
    if (stat == Task::EInitFlag::kFATAL) return stat;
    Event* event         = fMemoryMap->GetTemporaryEvent();
    fCurrentEventSmeared = new SmearedEvent(event);
    fCurrentEventSmeared->ActivateSmearing();
    fCurrentEventSmeared->LinkWithTree();
    fCurrentEventSmeared->GetImgEvent()->Register(kFALSE);
    if (fEventAlgorithm == NULL) {
      LOG(DEBUG) << "No event smear algorithm, new will be added but do virtual";
      fEventAlgorithm = new EventSmearVirtual();
    }
    if (fTrackAlgorithm == NULL) {
      LOG(DEBUG) << "No track smear algorithm, new will be added but do virtual";
      fTrackAlgorithm = new TrackSmearVirtual();
    }
    if (fEventAlgorithm->Init() == kFATAL) {
      LOG(FATAL) << "Failed to initialize EventSmear";
      return Task::EInitFlag::kFATAL;
    }
    if (fTrackAlgorithm->Init() == kFATAL) {
      LOG(FATAL) << "Failed to initialize TrackSmear";
      return Task::EInitFlag::kFATAL;
    }
    return stat;
  }

  void SmearTask::SaveEvents() { fSave = kTRUE; }

  void SmearTask::ProcessEvent() {
    fEventAlgorithm->ModifyEvent((SmearedEvent*) fCurrentEventSmeared);
    CutCollection* cont = fCutContainer->GetEventCollection(fCurrentEventCollectionID);
    for (int i = 0; i < fMemoryMap->GetTemporaryTotalTracksNo(); i++) {
      for (int j = 0; j < cont->GetNextNo(); j++) {
        fCurrentTrackCollectionID = cont->GetNextAddr(j);
        fCurrentTrack             = fCurrentEventSmeared->GetTrack(i);
        if (fCutContainer->PassTrack(fCurrentTrack, fCurrentTrackCollectionID)) {
          fTrackAlgorithm->ModifyTrack((SmearedTrack*) fCurrentTrack);
        }
      }
    }
  }
  void SmearTask::Exec(Option_t* /*opt*/) {
    fProcessedEvents++;
    fCurrentEvent = fMemoryMap->GetTemporaryEvent();
    fCurrentEventSmeared->Update();
    for (fCurrentEventCollectionID = 0; fCurrentEventCollectionID < fEventCollectionsNo; fCurrentEventCollectionID++) {
      if (fCutContainer->PassEvent(fCurrentEvent, fCurrentEventCollectionID)) { ProcessEvent(); }
    }
  }

  SmearTask::~SmearTask() {
    if (fEventAlgorithm) delete fEventAlgorithm;
    if (fTrackAlgorithm) delete fTrackAlgorithm;
    if (fCurrentEventSmeared) delete fCurrentEventSmeared;
    if (fCurrentTrackSmeared) delete fCurrentTrackSmeared;
  }
}  // namespace Hal
