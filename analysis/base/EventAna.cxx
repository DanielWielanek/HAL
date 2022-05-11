/*
 * EventAna.cxx
 *
 *  Created on: 05-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "EventAna.h"
#include "MemoryMapManager.h"

#include "Cut.h"
#include "CutContainer.h"
#include "CutsAndMonitors.h"
#include "DataFormatManager.h"
#include "DataManager.h"
#include "Event.h"
#include "EventVirtualCut.h"
#include "Field.h"
#include "HalStdString.h"
#include "Helix.h"
#include "Package.h"
#include "Parameter.h"

#include <FairLogger.h>
#include <TCollection.h>
#include <TDatabasePDG.h>
#include <TFile.h>
#include <TList.h>
#include <TNamed.h>
#include <TObjString.h>
#include <vector>


namespace Hal {

  EventAna::EventAna() :
    Task(),
    fCompressEvents(kFALSE),
    fDirectAcces(kFALSE),
    fKeepSource(kFALSE),
    fDisableFormatChecking(kFALSE),
    fProcessedEvents(0),
    fMixSize(1),
    fEventCollectionsNo(0),
    fCurrentEventCollectionID(0),
    fTiers(ECutUpdate::kEventUpdate),
    fPDG(NULL),
    fCutContainer(NULL),
    fMemoryMap(NULL),
    fCurrentEvent(NULL),
    fComment(""),
    fInit(kFALSE),
    fInChain(kFALSE),
    fTagList(NULL),
    fTaskID(0),
    fInFileName(""),
    fDataFormatManager(NULL) {
    fTagList = new TList();
    fTagList->SetName("Tags");
    fTagList->SetOwner(kTRUE);
    fTaskID            = DataFormatManager::Instance()->RegisterFormat();
    fDataFormatManager = DataFormatManager::Instance();
    AddTags("ana");
#ifdef _HAL_CLEAR_BUFFER_
    fIsLastTask = kFALSE;
#endif
  }

  Task::EInitFlag EventAna::Init() {
#ifdef _HAL_CLEAR_BUFFER_
    TList* list = FairRunAna::Instance()->GetMainTask()->GetListOfTasks();
    if (this == list->At(list->GetEntries() - 1)) { fIsLastTask = kTRUE; }
#endif
    /* trying to set magfield */
    fPDG = TDatabasePDG::Instance();

    if (fDisableFormatChecking == kFALSE) {
      LOG(DEBUG3) << "Format checking";
      if (CheckFormat() == Task::EInitFlag::kFATAL) return Task::EInitFlag::kFATAL;
    }
    Task::EInitFlag stat = InitCutContainer();
    if (stat == Task::EInitFlag::kFATAL) return Task::EInitFlag::kFATAL;
    InitMemoryMap();
    fEventCollectionsNo = fCutContainer->GetEventCollectionsNo();
    if (!this->InheritsFrom("TrackAna") && !this->InheritsFrom("MultiTrackAna")) {
      AddTags("eventana");
      AddTags(DataFormatManager::Instance()->GetFormatName(this->fTaskID));
    }
    fInit = kTRUE;
    return Task::EInitFlag::kSUCCESS;
  }

#ifdef _HAL_CLEAR_BUFFER_
  void EventAna::ClearMemory() {
    if (fIsLastTask) { fMemoryMap->ClearEvent(); }
  }
#endif

  void EventAna::Exec(Option_t* /*opt*/) {
    fProcessedEvents++;
    fCurrentEvent = fMemoryMap->GetTemporaryEvent();
    for (fCurrentEventCollectionID = 0; fCurrentEventCollectionID < fEventCollectionsNo; fCurrentEventCollectionID++) {
      if (fCutContainer->PassEvent(fCurrentEvent, fCurrentEventCollectionID)) { ProcessEvent(); }
    }
#ifdef _HAL_CLEAR_BUFFER_
    ClearMemory();
#endif
  }

  void EventAna::SetOption(Option_t* opt) {
    TString option = opt;
    if (option.Contains("+")) {
      std::vector<TString> arr = HalStd::ExplodeString(option, '+');
      for (int i = 0; i < (int) arr.size(); i++) {
        SetOption(arr[i]);
      }
    } else {
      LOG(WARNING) << Form("Invalid SetOption %s for class %s", option.Data(), this->ClassName());
    }
  }

  Task::EInitFlag EventAna::InitCutContainer() {
    if (fCutContainer == NULL) {
      LOG(DEBUG3) << "Null Cut Container, building ...";
      InitNewCutContainer();
    }
    LOG(DEBUG3) << "Checking Cut Containers ...";
    CheckCutContainerCollections();
    for (int i = 0; i < fCutContainer->GetSize(); i++) {
      CutCollection* empty_cont;
      switch (i) {
        case 0: {
          for (int j = 0; j < fCutContainer->GetEventCollectionsNo(); j++) {
            empty_cont = fCutContainer->GetEventCollection(j);
            if (empty_cont == NULL) {
              LOG(FATAL) << "NULL subcontainer for events";
              return Task::EInitFlag::kFATAL;
            }
          }
        } break;
        case 1: {
          for (int j = 0; j < fCutContainer->GetTrackCollectionsNo(); j++) {
            empty_cont = fCutContainer->GetTrackCollection(j);
            if (empty_cont == NULL) {
              LOG(FATAL) << "NULL subcontainer for tracks";
              return Task::EInitFlag::kFATAL;
            }
          }
        } break;
        case 2: {
          for (int j = 0; j < fCutContainer->GetTwoTrackCollectionsNo(); j++) {
            empty_cont = fCutContainer->GetTwoTrackCollection(j);
            if (empty_cont == NULL) {
              LOG(FATAL) << "NULL subcontainer for two-tracks";
              return Task::EInitFlag::kFATAL;
            }
          }
        } break;
        case 3: {
          for (int j = 0; j < fCutContainer->GetTwoTrackCollectionsBackgroundNo(); j++) {
            empty_cont = fCutContainer->GetTwoTrackBackgroundCollection(j);
            if (empty_cont == NULL) {
              LOG(FATAL) << "NULL subcontainer for background pairs";
              return Task::EInitFlag::kFATAL;
            }
          }
        } break;
      }
    }
    LOG(DEBUG2) << "Linking collections";
    LinkCollections();
    LOG(DEBUG3) << "Initialization cut container";
    fCutContainer->Init(GetTaskID());
    return Task::EInitFlag::kSUCCESS;
  }

  void EventAna::LinkCollections() {}

  void EventAna::InitNewCutContainer() {
    fCutContainer = new CutContainer(fTiers);
    // adding virtual cuts to push min-bias analysis
    EventVirtualCut virtE;
    virtE.SetCollectionID(0);
    fCutContainer->AddCut(virtE);
  }

  void EventAna::CheckCutContainerCollections() {
    if (fCutContainer->GetEventCollectionsNo() == 0) {
      LOG(DEBUG4) << "Adding virtual event cut";
      EventVirtualCut eventCut;
      fCutContainer->AddCut(eventCut, "fast");
    }
  }

  void EventAna::SetFormat(Event* format, EFormatDepth format_depth) {
    DataFormatManager* dataFormat = DataFormatManager::Instance();
    dataFormat->SetFormat(format, GetTaskID(), format_depth);
  }

  EventAna::EventAna(const EventAna& ana) :
    Task(),
    fCompressEvents(ana.fCompressEvents),
    fDirectAcces(ana.fDirectAcces),
    fKeepSource(ana.fKeepSource),
    fDisableFormatChecking(ana.fDisableFormatChecking),
    fProcessedEvents(ana.fProcessedEvents),
    fMixSize(ana.fMixSize),
    fEventCollectionsNo(ana.fEventCollectionsNo),
    fTiers(ana.fTiers),
    fPDG(ana.fPDG),
    fCutContainer(NULL),
    fMemoryMap(NULL),
    fCurrentEvent(NULL),
    fComment(""),
    fInit(kFALSE),
    fInChain(ana.fInChain),
    fTagList((TList*) ana.fTagList->Clone()),
    fInFileName(ana.fInFileName),
    fDataFormatManager(ana.fDataFormatManager) {
    if (ana.fInit) { LOG(WARNING) << "This object has been initialized, this may result in crash"; }
    if (ana.fCutContainer) { fCutContainer = new CutContainer(*ana.fCutContainer); }
    fCurrentEventCollectionID = 0;
    fTaskID                   = fDataFormatManager->RegisterFormat();
    if (fDataFormatManager->GetFormat(ana.fTaskID)) { SetFormat(fDataFormatManager->GetFormat(ana.fTaskID)->GetNewEvent()); }
  }

  void EventAna::AddToAnaMetadata(Package* main_pack, TObject* obj) const {
    if (main_pack == NULL) {
      LOG(WARNING) << "cannot call AddToMetadata if main_pack argument is NULL";
      return;
    }
    Package* metadata_pack = (Package*) main_pack->GetObjectByName("Metadata");
    if (metadata_pack == NULL) {
      LOG(WARNING) << "cannot add object in AddToMetadata package don't contain "
                      "any Metadata object";
      return;
    }
    metadata_pack->AddObject(obj);
  }

  EventAna::~EventAna() {
    DataFormatManager* dataManager = DataFormatManager::Instance();
    dataManager->Reset();
    fCurrentEvent = NULL;
    if (fTagList) delete fTagList;
    if (fCutContainer) delete fCutContainer;
    if (fMemoryMap) delete fMemoryMap;
    fDataFormatManager = NULL;
  }

  Package* EventAna::Report() const {
    Package* pack = new Package(this);
    pack->SetComment(fComment);
    Package* ana_metadata = new Package();
    ana_metadata->SetName("Metadata");
    ana_metadata->AddObject(new ParameterString("Analysis Name", this->ClassName()));
    DataFormatManager* dataManager = DataFormatManager::Instance();
    ana_metadata->AddObject(new ParameterString("DataType", dataManager->GetFormatName(GetTaskID(), EFormatDepth::kNonBuffered)));
    const Event* event_nonbufferend = dataManager->GetFormat(GetTaskID(), EFormatDepth::kNonBuffered);
    ana_metadata->AddObject(event_nonbufferend->Report());
    if (dataManager->FormatExist(GetTaskID(), EFormatDepth::kBuffered)) {
      const Event* event_bufferend = dataManager->GetFormat(GetTaskID(), EFormatDepth::kBuffered);
      ana_metadata->AddObject(
        new ParameterString("DataTypeBuf", dataManager->GetFormatName(GetTaskID(), EFormatDepth::kBuffered)));
      ana_metadata->AddObject(event_bufferend->Report());
    } else {
      const Event* event_bufferend = dataManager->GetFormat(GetTaskID(), EFormatDepth::kNonBuffered);
      ana_metadata->AddObject(
        new ParameterString("DataTypeBuf", dataManager->GetFormatName(GetTaskID(), EFormatDepth::kNonBuffered)));
      ana_metadata->AddObject(event_bufferend->Report());
    }
    ana_metadata->AddObject(fTagList);
    pack->AddObject(ana_metadata);
    pack->AddObject(fCutContainer->Report());
    return pack;
  }

  void EventAna::FinishTask() {
    Package* pack         = Report();
    Package* metadata_new = new Package();
    metadata_new->SetName("RunInfo");
    metadata_new->AddObject(new ParameterString("Software ver", HAL_PHYSICALANALYSYS_VER));
    metadata_new->AddObject(new ParameterString("Date", HalStd::GetDate(), 'f'));
    metadata_new->AddObject(new ParameterString("Time", HalStd::GetTime(), 'f'));
    metadata_new->AddObject(new ParameterUInt("Processed_events", fProcessedEvents, '+'));
    metadata_new->AddObject(new ParameterString("Input file", GetInputFileName(), 'f'));
    GoToDir("Info");
    TDirectory* dir        = (TDirectory*) gFile;
    TDirectory* metadatata = (TDirectory*) dir->Get("Info");
    if (metadatata->Get("RunInfo")) {
      delete metadata_new;
    } else {
      metadata_new->Write("RunInfo");
    }
    GoToDir("Physics");
    pack->Write(Form("AnaPackage_%i", GetTaskID()));
    if (pack) { delete pack; }
    gFile->cd();
    LOG(INFO) << Form("%s done, writing results", this->ClassName());
    // DataManager::Instance()->GetManager()->Fill();
    //	DataManager::Instance()->GetManager()->GetOutFile()->WriteKeys();
  }

  void EventAna::SetComment(TString comment) { fComment = comment; }

  void EventAna::AddTags(TString tag) {
    if (tag.BeginsWith(" ")) {
      AddTags(tag(1, tag.Length()));
    } else if (tag.Contains(" ")) {
      Int_t begin  = tag.First(' ');
      Int_t lenght = tag.Length();
      TString first_part(tag(0, begin));
      AddTags(first_part);
      TString second_part(tag(begin + 1, lenght));
      AddTags(second_part);
    } else {
      TObjString* obj_string = new TObjString();
      obj_string->SetString(tag);
      for (int i = 0; i < fTagList->GetEntries(); i++) {
        TObjString* temp = (TObjString*) fTagList->At(i);
        if (temp->GetString().EqualTo(obj_string->GetString())) { return; }
      }
      fTagList->AddLast(obj_string);
    }
  }

  void EventAna::GoToDir(TString name) {
    TDirectory* dir            = (TDirectory*) gFile;
    std::vector<TString> array = HalStd::ExplodeString(name, '/');
    for (int i = 0; i < (int) array.size(); i++) {
      TDirectory* newdir = (TDirectory*) dir->FindObject(array[i]);
      if (!newdir) { dir->mkdir(array[i]); }
      dir->cd(array[i]);
    }
  }

  void EventAna::AddCut(const Cut& cut, Option_t* opt) {
    if (fCutContainer == NULL) { fCutContainer = new CutContainer(fTiers); }
    fCutContainer->AddCut(cut, opt);
  }

  void EventAna::AddCutMonitor(const CutMonitor& mon, Option_t* opt) {
    if (fCutContainer == NULL) { fCutContainer = new CutContainer(fTiers); }
    fCutContainer->AddMonitor(mon, opt);
  }

  void EventAna::SetFormatOption(EFormatOption option) {
    switch (option) {
      case kDirectAccess: fDirectAcces = kTRUE; break;
      case kNoDirectAcces: fDirectAcces = kFALSE; break;
      case kCompress: fCompressEvents = kTRUE; break;
      case kNoCompress: fCompressEvents = kFALSE; break;
      case kKeepSource: fKeepSource = kTRUE; break;
      case kNoKeepSource: fKeepSource = kFALSE; break;
    };
  }

  Task::EInitFlag EventAna::CheckFormat() {
    DataFormatManager* formatManager = DataFormatManager::Instance();
    SetInputFileName(DataManager::Instance()->GetInFile()->GetName());
    LOG(DEBUG3) << Form("Initialization format of task with ID = %i", GetTaskID());
    if (formatManager->FormatExist(GetTaskID(), EFormatDepth::kNonBuffered)) {
      TString name = formatManager->GetFormatName(GetTaskID(), EFormatDepth::kNonBuffered);
      LOG(DEBUG) << "L1 Format is set to " << name;
      if (!formatManager->GetFormat(GetTaskID(), EFormatDepth::kNonBuffered)->ExistInTree()) {
        LOG(ERROR) << "Format exists but not present in tree!";
        return kFATAL;
      }
    } else {
      LOG(ERROR) << "L1 format is not set trying to find autosupported format";
      Event* temp_format = formatManager->FindAutoSupportedFormat();
      if (temp_format) {
        LOG(DEBUG) << Form("Format %s found and will be used for task %s", temp_format->GetFormatName().Data(), ClassName());
        SetFormat(temp_format);
        return kSUCCESS;
      } else {
        LOG(FATAL) << "Format of data is not set, try to call SetFormat";
      }
      return kFATAL;
    }
    return kSUCCESS;
  }

  void EventAna::InitMemoryMap() {
    fMemoryMap = new MemoryMapManager(fCutContainer);
    fMemoryMap->SetMixSize(fMixSize);
    LOG(DEBUG3) << "Initialization MemoryMap";
    fMemoryMap->Init(GetTaskID(), fKeepSource, fCompressEvents, fDirectAcces);
  }

  void EventAna::AddCutsAndMonitors(const CutsAndMonitors& monCuts) {
    for (int iCut = 0; iCut < monCuts.GetNCuts(); iCut++) {
      AddCut(*monCuts.GetCut(iCut), monCuts.GetCutOption(iCut));
    }
    for (int iMon = 0; iMon < monCuts.GetNCutMonitors(); iMon++) {
      AddCutMonitor(*monCuts.GetMonitor(iMon), monCuts.GetCutMonitorOption(iMon));
    }
  }

  EventAna& EventAna::operator=(const EventAna& other) {
    if (this != &other) {
      fCompressEvents           = other.fCompressEvents;
      fDirectAcces              = other.fDirectAcces;
      fKeepSource               = other.fKeepSource;
      fProcessedEvents          = other.fProcessedEvents;
      fMixSize                  = other.fMixSize;
      fEventCollectionsNo       = other.fEventCollectionsNo;
      fCurrentEventCollectionID = other.fCurrentEventCollectionID;
      fTiers                    = other.fTiers;
      fPDG                      = other.fPDG;
      if (fCutContainer) {
        delete fCutContainer;
        fCutContainer = NULL;
      }
      if (fMemoryMap) {
        delete fMemoryMap;
        fMemoryMap = NULL;
      }
      if (other.fCutContainer) fCutContainer = (CutContainer*) other.fCutContainer->Clone();
      if (other.fMemoryMap) fMemoryMap = (MemoryMapManager*) other.fMemoryMap->Clone();
      fTaskID                    = DataFormatManager::Instance()->RegisterFormat();
      DataFormatManager* manager = DataFormatManager::Instance();
      if (manager->FormatExist(other.fTaskID)) { SetFormat(manager->GetNewEvent(other.fTaskID)); }
      fCurrentEvent      = NULL;
      fComment           = other.fComment;
      fInit              = other.fInit;
      fInChain           = other.fInChain;
      fTagList           = (TList*) other.fTagList->Clone();
      fInFileName        = other.fInFileName;
      fDataFormatManager = other.fDataFormatManager;
      if (fInit == kTRUE) { LOG(WARNING) << "Copying initialized task " << this->ClassName(); }
    }
    return *this;
  }
}  // namespace Hal
