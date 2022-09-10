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

  EventAna::EventAna(ECutUpdate tiers) :
    Task(),
    fProcessedEvents(0),
    fMixSize(1),
    fEventCollectionsNo(0),
    fCurrentEventCollectionID(0),
    fTiers(tiers),
    fPDG(nullptr),
    fCutContainer(NULL),
    fMemoryMap(nullptr),
    fCurrentEvent(nullptr),
    fComment(""),
    fInit(kFALSE),
    fInChain(kFALSE),
    fTaskID(0),
    fInFileName(""),
    fDataFormatManager(nullptr) {
    fTaskID            = DataFormatManager::Instance()->RegisterFormat();
    fDataFormatManager = DataFormatManager::Instance();
    SetFormatOption(EFormatOption::kStandardAcess);
    SetFormatOption(EFormatOption::kNoKeepSource);
    SetFormatOption(EFormatOption::kNoCompress);
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

    if (TESTBIT(fFormatOption, eBitFormat::kChecking) == kTRUE) {
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
    SetFormatOption(EFormatOption::kNoReaderAcces);
  }

  EventAna::EventAna(const EventAna& ana) :
    Task(),
    fFormatOption(ana.fFormatOption),
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
    fCurrentEvent = nullptr;
    if (fCutContainer) delete fCutContainer;
    if (fMemoryMap) delete fMemoryMap;
    fDataFormatManager = nullptr;
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
    TList* tagList = new TList();
    for (auto str : fTagList) {
      tagList->AddLast(new TObjString(str));
    }
    ana_metadata->AddObject(tagList);
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
      for (auto str : fTagList) {
        if (str.EqualTo(tag)) return;
      }
      fTagList.push_back(tag);
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
      case EFormatOption::kDirectAccess: {
        SETBIT(fFormatOption, eBitFormat::kDirectAcesss);  // direct access
        CLRBIT(fFormatOption, eBitFormat::kReader);        // disable reader
        CLRBIT(fFormatOption, eBitFormat::kSource);        // disable source (probably is not stored by direct access class
      } break;
      case EFormatOption::kNoDirectAccess: {
        CLRBIT(fFormatOption, eBitFormat::kDirectAcesss);  // no direct access
      } break;
      case EFormatOption::kNoReaderAcces: {
        CLRBIT(fFormatOption, eBitFormat::kReader);  // reader disabled access
      } break;
      case EFormatOption::kCompress: {
        SETBIT(fFormatOption, eBitFormat::kCompression);
      } break;
      case EFormatOption::kNoCompress: {
        CLRBIT(fFormatOption, eBitFormat::kCompression);
      } break;
      case EFormatOption::kKeepSource: {
        SETBIT(fFormatOption, eBitFormat::kSource);
      } break;
      case EFormatOption::kNoKeepSource: {
        CLRBIT(fFormatOption, eBitFormat::kSource);
      } break;
      case EFormatOption::kReaderAccess: {
        SETBIT(fFormatOption, eBitFormat::kReader);
        CLRBIT(fFormatOption, eBitFormat::kDirectAcesss);
      } break;
      case EFormatOption::kStandardAcess: {
        CLRBIT(fFormatOption, eBitFormat::kDirectAcesss);  // no direct access
        CLRBIT(fFormatOption, eBitFormat::kReader);        // no reader
      } break;
      case EFormatOption::kDisableChecking: {
        CLRBIT(fFormatOption, eBitFormat::kChecking);
      } break;
    };
  }

  Task::EInitFlag EventAna::CheckFormat() {
    DataFormatManager* formatManager = DataFormatManager::Instance();
    DataManager* datamanager         = DataManager::Instance();
    SetInputFileName(DataManager::Instance()->GetInputFileName());
    if (formatManager->GetFormat(GetTaskID(), EFormatDepth::kNonBuffered) == nullptr) {
      Cout::PrintInfo("Format is not set, switching to reader", EInfo::kError);
      SetFormatOption(EFormatOption::kReaderAccess);
    }
    if (TESTBIT(fFormatOption, eBitFormat::kReader)) {  // use reader data
      Event* event = nullptr;
      event        = dynamic_cast<Hal::Event*>(datamanager->GetObject("HalEvent."));
      if (event) {
        Cout::PrintInfo("L1 format from reader has been found", EInfo::kInfo);
        formatManager->SetFormat(event->GetNewEvent(), GetTaskID(), EFormatDepth::kNonBuffered, kTRUE);
        if (formatManager->GetFormat(GetTaskID(), EFormatDepth::kBuffered) == nullptr) {
          formatManager->SetFormat(event->GetNewEvent(), GetTaskID(), EFormatDepth::kBuffered, kTRUE);
        }
        return Task::EInitFlag::kSUCCESS;
      }
    } else {
      if (TESTBIT(fFormatOption, eBitFormat::kDirectAcesss)) {  // direct access to data by classname.
        TString branchName = formatManager->GetFormat(GetTaskID(), EFormatDepth::kNonBuffered)->ClassName();
        std::vector<TString> patterns;
        patterns.push_back(branchName);
        patterns.push_back(branchName + ".");
        Bool_t foundFormat = kFALSE;
        for (auto name : patterns) {
          Hal::Event* event = dynamic_cast<Hal::Event*>(DataManager::Instance()->GetObject(name));
          if (event) {
            foundFormat = kTRUE;
            formatManager->SetFormat(event->GetNewEvent(), GetTaskID(), EFormatDepth::kNonBuffered, kTRUE);
            if (formatManager->GetFormat(GetTaskID(), EFormatDepth::kBuffered) == nullptr) {
              formatManager->SetFormat(event->GetNewEvent(), GetTaskID(), EFormatDepth::kBuffered, kTRUE);
            }
            break;
          }
        }
        if (!foundFormat) {
          Cout::PrintInfo(Form("Could not find direct access format %s", branchName.Data()), EInfo::kError);
          return Task::EInitFlag::kFATAL;
        }
      } else {  // not direct access nor reader, standard way
        Bool_t exist = formatManager->GetFormat(GetTaskID(), EFormatDepth::kNonBuffered)->ExistInTree();
        if (!exist) {
          Cout::PrintInfo("The format is present but does not exist in the tree!", EInfo::kError);
          return Task::EInitFlag::kFATAL;
        }
      }
    }
    return Task::EInitFlag::kSUCCESS;
  }

  void EventAna::InitMemoryMap() {
    fMemoryMap = new MemoryMapManager(fCutContainer);
    fMemoryMap->SetMixSize(fMixSize);
#ifdef HAL_DEBUG
    Cout::PrintInfo("Initialization MemoryMap", EInfo::kDebugInfo);
#endif
    std::vector<TString> brName;
    if (TESTBIT(fFormatOption, eBitFormat::kReader)) {
      brName.push_back("HalEvent.");
    } else if (TESTBIT(fFormatOption, eBitFormat::kDirectAcesss)) {
      TString evName = DataFormatManager::Instance()->GetFormat(GetTaskID())->ClassName();
      brName.push_back(Form("%s.", evName.Data()));
      brName.push_back(evName);
    }
    fMemoryMap->Init(GetTaskID(), TESTBIT(fFormatOption, eBitFormat::kCompression), brName);
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
    if (this->fTiers != other.fTiers) {
      Cout::PrintInfo(Form("Cannot assign %s to %s, different tiers no", this->ClassName(), other.ClassName()), EInfo::kError);
      return *this;
    }
    if (this != &other) {
      fFormatOption             = other.fFormatOption;
      fProcessedEvents          = other.fProcessedEvents;
      fMixSize                  = other.fMixSize;
      fEventCollectionsNo       = other.fEventCollectionsNo;
      fCurrentEventCollectionID = other.fCurrentEventCollectionID;
      fPDG                      = other.fPDG;
      if (fCutContainer) {
        delete fCutContainer;
        fCutContainer = nullptr;
      }
      if (fMemoryMap) {
        delete fMemoryMap;
        fMemoryMap = nullptr;
      }
      if (other.fCutContainer) fCutContainer = (CutContainer*) other.fCutContainer->Clone();
      if (other.fMemoryMap) fMemoryMap = (MemoryMapManager*) other.fMemoryMap->Clone();
      fTaskID                    = DataFormatManager::Instance()->RegisterFormat();
      DataFormatManager* manager = DataFormatManager::Instance();
      if (manager->FormatExist(other.fTaskID)) { SetFormat(manager->GetNewEvent(other.fTaskID)); }
      fCurrentEvent      = nullptr;
      fComment           = other.fComment;
      fInit              = other.fInit;
      fInChain           = other.fInChain;
      fTagList           = other.fTagList;
      fInFileName        = other.fInFileName;
      fDataFormatManager = other.fDataFormatManager;
      if (fInit == kTRUE) { Cout::PrintInfo(Form("Copying initialized task  %s", this->ClassName()), EInfo::kWarning); }
    }
    return *this;
  }
}  // namespace Hal
