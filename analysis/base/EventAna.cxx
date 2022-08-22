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

#include "Cout.h"
#include "Cut.h"
#include "CutContainer.h"
#include "CutsAndMonitors.h"
#include "DataFormatManager.h"
#include "DataManager.h"
#include "Event.h"
#include "EventVirtualCut.h"
#include "Helix.h"
#include "MagField.h"
#include "Package.h"
#include "Parameter.h"
#include "StdString.h"

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
    fTiers(ECutUpdate::kEvent),
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
    // TList* list = FairRunAna::Instance()->GetMainTask()->GetListOfTasks();
    // if (this == list->At(list->GetEntries() - 1)) { fIsLastTask = kTRUE; }
#endif
    /* trying to set magfield */
    fPDG = TDatabasePDG::Instance();

    if (fDisableFormatChecking == kFALSE) {
#ifdef HAL_DEBUG
      Cout::PrintInfo("Format checking", EInfo::kDebugInfo);
#endif
      if (CheckFormat() == Task::EInitFlag::kFATAL) {
        Cout::PrintInfo("Failed to check format", EInfo::kError);
        return Task::EInitFlag::kFATAL;
      }
    }
    Task::EInitFlag stat = InitCutContainer();
    if (stat == Task::EInitFlag::kFATAL) {
      Cout::PrintInfo("Failed to init cut container", EInfo::kError);
      return Task::EInitFlag::kFATAL;
    }
    InitMemoryMap();
    fEventCollectionsNo = fCutContainer->GetEventCollectionsNo();
    if (!this->InheritsFrom("Hal::TrackAna") && !this->InheritsFrom("Hal::MultiTrackAna")) {
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
      std::vector<TString> arr = Hal::Std::ExplodeString(option, '+');
      for (int i = 0; i < (int) arr.size(); i++) {
        SetOption(arr[i]);
      }
    } else {
      Cout::PrintInfo(Form("Invalid SetOption %s for class %s", option.Data(), this->ClassName()), EInfo::kLowWarning);
    }
  }

  Task::EInitFlag EventAna::InitCutContainer() {
    if (fCutContainer == NULL) {
#ifdef HAL_DEBUG
      Cout::PrintInfo("Null Cut Container, building ...", EInfo::kDebugInfo);
#endif
      InitNewCutContainer();
    }
#ifdef HAL_DEBUG
    Cout::PrintInfo("Checking Cut Containers ...", EInfo::kDebugInfo);
#endif
    CheckCutContainerCollections();
    for (int i = 0; i < fCutContainer->GetSize(); i++) {
      CutCollection* empty_cont;
      switch (i) {
        case 0: {
          for (int j = 0; j < fCutContainer->GetEventCollectionsNo(); j++) {
            empty_cont = fCutContainer->GetEventCollection(j);
            if (empty_cont == NULL) {
              Cout::PrintInfo("NULL subcontainer for events", EInfo::kError);
              return Task::EInitFlag::kFATAL;
            }
          }
        } break;
        case 1: {
          for (int j = 0; j < fCutContainer->GetTrackCollectionsNo(); j++) {
            empty_cont = fCutContainer->GetTrackCollection(j);
            if (empty_cont == NULL) {
              Cout::PrintInfo("NULL subcontainer for tracks", EInfo::kError);
              return Task::EInitFlag::kFATAL;
            }
          }
        } break;
        case 2: {
          for (int j = 0; j < fCutContainer->GetTwoTrackCollectionsNo(); j++) {
            empty_cont = fCutContainer->GetTwoTrackCollection(j);
            if (empty_cont == NULL) {
              Cout::PrintInfo("NULL subcontainer for two-tracks", EInfo::kError);
              return Task::EInitFlag::kFATAL;
            }
          }
        } break;
        case 3: {
          for (int j = 0; j < fCutContainer->GetTwoTrackCollectionsBackgroundNo(); j++) {
            empty_cont = fCutContainer->GetTwoTrackBackgroundCollection(j);
            if (empty_cont == NULL) {
              Cout::PrintInfo("NULL subcontainer for background pairs", EInfo::kError);
              return Task::EInitFlag::kFATAL;
            }
          }
        } break;
      }
    }
#ifdef HAL_DEBUG
    Cout::PrintInfo("Linking collections", EInfo::kDebugInfo);
#endif
    LinkCollections();
#ifdef HAL_DEBUG
    Cout::PrintInfo("Initialization cut container", EInfo::kDebugInfo);
#endif
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
#ifdef HAL_DEBUG
      Cout::PrintInfo("Adding virtual event cut", EInfo::kDebugInfo);
#endif
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
    if (ana.fInit) { Cout::PrintInfo("This object has been initialized, this may result in crash", EInfo::kWarning); }
    if (ana.fCutContainer) { fCutContainer = new CutContainer(*ana.fCutContainer); }
    fCurrentEventCollectionID = 0;
    fTaskID                   = fDataFormatManager->RegisterFormat();
    if (fDataFormatManager->GetFormat(ana.fTaskID)) { SetFormat(fDataFormatManager->GetFormat(ana.fTaskID)->GetNewEvent()); }
  }

  void EventAna::AddToAnaMetadata(Package* main_pack, TObject* obj) const {
    if (main_pack == NULL) {
      Cout::PrintInfo("cannot call AddToMetadata if main_pack argument is NULL", EInfo::kLowWarning);
      return;
    }
    Package* metadata_pack = (Package*) main_pack->GetObjectByName("Metadata");
    if (metadata_pack == NULL) {
      Cout::PrintInfo("cannot add object in AddToMetadata package don't contain any Metadata object", EInfo::kLowWarning);
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
    Package* pack = Report();
    GoToDir("HalPhysics");
    pack->Write(Form("AnaPackage_%i", GetTaskID()));
    if (pack) { delete pack; }
    gFile->cd();
    Cout::PrintInfo(Form("%s done, writing results", this->ClassName()), EInfo::kDebugInfo);
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
    if (DataManager::Instance()->GetInFile()) {
      SetInputFileName(DataManager::Instance()->GetInFile()->GetName());
    } else {
      Cout::PrintInfo("Cannot find name of input file", EInfo::kLowWarning);
    }
#ifdef HAL_DEBUG
    Cout::PrintInfo(Form("Initialization format of task with ID = %i", GetTaskID()), EInfo::kDebugInfo);
#endif
    if (formatManager->FormatExist(GetTaskID(), EFormatDepth::kNonBuffered)) {
      TString name = formatManager->GetFormatName(GetTaskID(), EFormatDepth::kNonBuffered);
#ifdef HAL_DEBUG
      Cout::PrintInfo(Form("L1 Format is set to %s", name.Data()), EInfo::kDebugInfo);
#endif
      if (!formatManager->GetFormat(GetTaskID(), EFormatDepth::kNonBuffered)->ExistInTree()) {
        Cout::PrintInfo("Format exists but not present in tree!", EInfo::kError);
        return Task::EInitFlag::kFATAL;
      }
    } else {
      Cout::PrintInfo("L1 format is not set trying to find autosupported format", EInfo::kError);
      Event* temp_format = formatManager->FindReaderFormat();
      if (temp_format) {
#ifdef HAL_DEBUG
        Cout::PrintInfo(Form("Format %s found and will be used for task %s", temp_format->GetFormatName().Data(), ClassName()),
                        EInfo::kDebugInfo);
#endif
        Cout::PrintInfo("L1 format autosupported has been found", EInfo::kInfo);
        SetFormat(temp_format);
        fDirectAcces = kTRUE;
        return Task::EInitFlag::kSUCCESS;
      } else {
        Cout::PrintInfo("Format of data is not set, try to call SetFormat", EInfo::kError);
      }
      return Task::EInitFlag::kFATAL;
    }
    return Task::EInitFlag::kSUCCESS;
  }

  void EventAna::InitMemoryMap() {
    fMemoryMap = new MemoryMapManager(fCutContainer);
    fMemoryMap->SetMixSize(fMixSize);
#ifdef HAL_DEBUG
    Cout::PrintInfo("Initialization MemoryMap", EInfo::kDebugInfo);
#endif
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
      if (fInit == kTRUE) { Cout::PrintInfo(Form("Copying initialized task  %s", this->ClassName()), EInfo::kWarning); }
    }
    return *this;
  }
}  // namespace Hal
