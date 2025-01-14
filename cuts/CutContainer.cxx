/*
 * CutContainer.cxx
 *
 *  Created on: 06-08-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CutContainer.h"

#include <TClass.h>
#include <TCollection.h>
#include <TList.h>
#include <TRegexp.h>
#include <iostream>

#include "AnaFile.h"
#include "Cout.h"
#include "CutFuncs.h"
#include "CutOptions.h"
#include "EventBinningCut.h"
#include "EventComplexCut.h"
#include "EventVirtualCut.h"
#include "Package.h"
#include "Parameter.h"
#include "StdString.h"
#include "TrackComplexCut.h"
#include "TrackVirtualCut.h"
#include "TwoTrackComplexCut.h"
#include "TwoTrackVirtualCut.h"

namespace Hal {
  CutContainer::CutContainer() : TObject(), fInit(kFALSE), fSize(0), fTempCutMonitors(NULL), fCutContainers(NULL) {}

  CutContainer::CutContainer(ECutUpdate tiers) : fInit(kFALSE), fTempCutMonitors(NULL), fCutContainers(NULL) {
    switch (tiers) {
      case ECutUpdate::kEvent: fSize = 1; break;
      case ECutUpdate::kTrack: fSize = 2; break;
      case ECutUpdate::kTwoTrack: fSize = 3; break;
      case ECutUpdate::kTwoTrackBackground: fSize = 4; break;
      default: {
        Cout::PrintInfo("Unknown update ration in CutContainer", EInfo::kError);
      } break;
    }
    fCutContainers   = new TObjArray*[fSize];
    fTempCutMonitors = new TObjArray*[fSize];
    for (int i = 0; i < fSize; i++) {
      fCutContainers[i]   = new TObjArray();
      fTempCutMonitors[i] = new TObjArray();
    }
  }

  CutContainer::CutContainer(const CutContainer& cont) :
    TObject(cont), fInit(kFALSE), fSize(cont.fSize), fTempCutMonitors(NULL), fCutContainers(NULL) {
    if (cont.fInit == kTRUE) { Cout::PrintInfo("CutContainer already initialized", EInfo::kError); }
    fCutContainers   = new TObjArray*[fSize];
    fTempCutMonitors = new TObjArray*[fSize];
    for (int i = 0; i < fSize; i++) {
      fCutContainers[i] = new TObjArray();
      for (int j = 0; j < cont.fCutContainers[i]->GetEntries(); j++) {
        CutCollection* subcont = (CutCollection*) cont.fCutContainers[i]->UncheckedAt(j);
        fCutContainers[i]->Add(subcont->MakeNewCopy(fCutContainers));
      }
      fTempCutMonitors[i] = new TObjArray();
      for (int j = 0; j < cont.fTempCutMonitors[i]->GetEntries(); j++) {
        CutMonitor* mon = (CutMonitor*) cont.fTempCutMonitors[i]->At(j);
        fTempCutMonitors[i]->Add(mon->MakeCopy());
      }
    }
  }

  void CutContainer::AddCut(const Cut& cut, Option_t* opt) {
    Hal::CutOptions opts(opt, cut.GetCollectionID());
    auto modified_cut  = std::unique_ptr<Hal::Cut>(opts.MakeCutCopy(cut));
    auto collectionsNo = opts.GetCollectionsIds();

    // add to cut containers
    auto addCutRaw = [&](ECutUpdate upd, Int_t colNo) {
      if (fSize <= static_cast<Int_t>(upd)) {
        TString update_ratio_name = Hal::Cuts::GetCutUpdateRatioName(upd);
        Cout::PrintInfo(Form("CutContainer can't hold %s cut because it's update ratio (%s) is "
                             "too big, check fTries or call SetOption(backround) before adding cuts "
                             "or cut monitors",
                             cut.ClassName(),
                             update_ratio_name.Data()),
                        EInfo::kLowWarning);
        return;
      }
      if (colNo < 0) {
        Cout::PrintInfo(Form("Cannot have negative collID for cut %s", cut.ClassName()), EInfo::kWarning);
        return;
      }
      if (GetCutContainer(upd)->At(colNo) == nullptr) {
        CutCollection* subcont = new CutCollection(fCutContainers, fSize, upd, colNo);
        subcont->AddCut(modified_cut->MakeCopy(), opts);
        GetCutContainer(upd)->AddAtAndExpand(subcont, colNo);
      } else if ((colNo >= GetCutContainer(upd)->GetEntriesFast())) {
        CutCollection* subcont = new CutCollection(fCutContainers, fSize, upd, colNo);
        subcont->AddCut(modified_cut->MakeCopy(), opts);
        GetCutContainer(upd)->AddAtAndExpand(subcont, colNo);
      } else {
        ((CutCollection*) GetCutContainer(upd)->UncheckedAt(colNo))->AddCut(modified_cut->MakeCopy(), opts);
      }
    };
    std::vector<ECutUpdate> upds;
    if (cut.GetUpdateRatio() == ECutUpdate::kTwoTrack || cut.GetUpdateRatio() == ECutUpdate::kTwoTrackBackground) {
      if (opts.Sig()) upds.push_back(ECutUpdate::kTwoTrack);
      if (opts.Bckg()) upds.push_back(ECutUpdate::kTwoTrackBackground);
    } else {
      upds.push_back(cut.GetUpdateRatio());
    }
    for (auto upd : upds) {
      for (auto colId : collectionsNo) {
        addCutRaw(upd, colId);
      }
    }
  }

  void CutContainer::AddMonitor(const CutMonitor& monitor, Option_t* opt) {
    Hal::CutOptions opts(opt, monitor.GetCollectionID());
    auto monitor_copy = std::unique_ptr<Hal::CutMonitor>(opts.MakeMonitorCopy(monitor));
    std::vector<ECutUpdate> upds;
    if (monitor.GetUpdateRatio() == ECutUpdate::kTwoTrack || monitor.GetUpdateRatio() == ECutUpdate::kTwoTrackBackground) {
      if (opts.Sig()) upds.push_back(ECutUpdate::kTwoTrack);
      if (opts.Bckg()) upds.push_back(ECutUpdate::kTwoTrackBackground);
    } else {
      upds.push_back(monitor.GetUpdateRatio());
    }
    for (auto upd : upds) {
      if (fSize <= static_cast<Int_t>(upd)) {
        Cout::PrintInfo("CutContainer can't hold this cut because it's update ratio is to big, check fTries or call "
                        "SetOption(backround) before adding cuts or cut monitors",
                        EInfo::kLowWarning);
        return;
      }
      auto collections = opts.GetCollectionsIds();
      for (int colId : collections) {
        auto copy = monitor_copy->MakeCopy();
        copy->SetCollectionID(colId);
        fTempCutMonitors[static_cast<Int_t>(upd)]->AddLast(copy);
      }
    }
  }

  void CutContainer::InitReport() const {
    for (int i = 0; i < fSize; i++) {
      for (int j = 0; j < fCutContainers[i]->GetEntriesFast(); j++) {
        ((CutCollection*) fCutContainers[i]->UncheckedAt(j))->PrintInfo();
      }
    }
  }

  void CutContainer::LinkCollections(ECutUpdate opt_low, Int_t in_low, ECutUpdate opt_high, Int_t in_high) {
    if (opt_high == opt_low || opt_low > opt_high) { Cout::PrintInfo("Wrong ECut Update in Link Collections", EInfo::kError); }
    if (static_cast<Int_t>(opt_high) >= fSize) {
      Cout::PrintInfo("To big opt_high, link will be ingored", EInfo::kError);
    } else if (static_cast<Int_t>(opt_low) >= fSize) {
      Cout::PrintInfo("To big opt_low, link will be ingored", EInfo::kError);
    }
    CutCollection* low  = NULL;
    CutCollection* high = NULL;
    if (opt_low == ECutUpdate::kEvent) {
      low = (CutCollection*) GetCutContainer(opt_low)->At(in_low);
    } else if (opt_low == ECutUpdate::kTrack) {
      low = (CutCollection*) GetCutContainer(opt_low)->At(in_low);
    } else {
      Cout::PrintInfo("Invalid opt_low argument in CutContainer::LinkCollections", EInfo::kError);
      return;
    }
    if (opt_high == ECutUpdate::kTrack) {
      high = (CutCollection*) GetCutContainer(opt_high)->At(in_high);
    } else if (opt_high == ECutUpdate::kTwoTrack || opt_high == ECutUpdate::kTwoTrackBackground) {
      high = (CutCollection*) GetCutContainer(opt_high)->At(in_high);
    } else {
      Cout::PrintInfo("Invalid opt_high argument in CutContainer::LinkCollections", EInfo::kError);
      return;
    }
    if (low == NULL || high == NULL) {
      Cout::PrintInfo("Cant link some collections", EInfo::kError);
      return;
    }
    high->AddPreviousAddr(in_low, 0);
    if (opt_high == ECutUpdate::kTwoTrackBackground) {
      if (!high->IsDummy()) low->AddNextAddr(in_high, kTRUE);
    } else {
      if (!high->IsDummy()) low->AddNextAddr(in_high, kFALSE);
    }
  }

  void CutContainer::ReplicateCollection(ECutUpdate type, Int_t collection_no, Int_t new_collection_no, Option_t* /*option*/) {
    CutCollection* old = NULL;
    old                = (CutCollection*) GetCutContainer(type)->UncheckedAt(collection_no);
    GetCutContainer(type)->AddAtAndExpand(old->Replicate(new_collection_no), new_collection_no);
  }

  void CutContainer::VerifyOrder(TObjArray* obj) {
    for (int i = 0; i < obj->GetEntriesFast(); i++) {
      if (obj->UncheckedAt(i) == NULL) { Cout::PrintInfo(Form("Null cutsubcontainers at %i", i), EInfo::kError); }
      Int_t collection_no = ((CutCollection*) obj->UncheckedAt(i))->GetCollectionID();
      if (collection_no != i) { Cout::PrintInfo(Form("Wrong order of cuts [%i]!=%i", i, collection_no), EInfo::kError); }
    }
  }

  void CutContainer::RemoveCollection(ECutUpdate update, Int_t collection) { GetCutContainer(update)->RemoveAt(collection); }

  CutContainer::~CutContainer() {
    for (int i = 0; i < fSize; i++) {
      if (fCutContainers[i] != NULL) fCutContainers[i]->Delete();
    }
    if (fCutContainers) delete fCutContainers;
    if (fTempCutMonitors) {
      for (int i = 0; i < fSize; i++) {
        fTempCutMonitors[i]->Delete();
      }
      delete[] fTempCutMonitors;
    }
  }

  void CutContainer::Init(const Int_t task_id) {
    if (fInit == kTRUE) {
      Cout::PrintInfo("CutContainer has been initialized before", EInfo::kLowWarning);
      return;
    }
    for (int i = 0; i < fSize; i++) {
      VerifyOrder(fCutContainers[i]);
    }
    for (int k = 0; k < fSize; k++) {
      for (int j = 0; j < fTempCutMonitors[k]->GetEntriesFast(); j++) {
        CutMonitor* cutmon = (CutMonitor*) fTempCutMonitors[k]->UncheckedAt(j);
        if (cutmon->GetCollectionID() != -1) {
          CutMonitor* clone = cutmon->MakeCopy();
          Int_t collection  = cutmon->GetCollectionID();
          if ((CutCollection*) (fCutContainers[k]->UncheckedAt(collection)) == NULL) {
            Cout::PrintInfo(Form(" Collection %i for cut monitor %s not found", collection, clone->ClassName()), EInfo::kError);
          } else {
            ((CutCollection*) (fCutContainers[k]->UncheckedAt(collection)))->AddCutMonitor(clone);
          }
        } else {
          for (int i = 0; i < fCutContainers[k]->GetEntriesFast(); i++) {
            CutMonitor* clone = cutmon->MakeCopy();
            ((CutCollection*) (fCutContainers[k]->UncheckedAt(i)))->AddCutMonitor(clone);
          }
        }
      }
#ifdef HAL_DEBUG
      Cout::PrintInfo("Initializing cut collection", EInfo::kDebugInfo);
#endif
      for (int i = 0; i < fCutContainers[k]->GetEntriesFast(); i++) {
#ifdef HAL_DEBUG
        Cout::PrintInfo(Form("Initializing cut collection at %i", k), EInfo::kDebugInfo);
#endif
        ((CutCollection*) (fCutContainers[k]->UncheckedAt(i)))->Init(task_id);
      }
    }
#ifdef HAL_DEBUG
    Cout::PrintInfo("Deleting temporary cut monitors", EInfo::kDebugInfo);
#endif
    for (int i = 0; i < fSize; i++) {
      fTempCutMonitors[i]->Delete();
    }
    delete[] fTempCutMonitors;
    fTempCutMonitors = NULL;
    fInit            = kTRUE;
  }

  Package* CutContainer::Report() const {
    Package* pack = new Package(this, kTRUE);
    auto getList  = [&](ECutUpdate upd) {
      TString collectionNo = Hal::AnaFile::GetCollectionCountName(upd);
      pack->AddObject(new ParameterInt(collectionNo, GetCutContainer(upd)->GetEntriesFast()));
      TList* list1 = new TList();
      list1->SetOwner(kTRUE);
      list1->SetName(Hal::AnaFile::GetCollectionListName(upd));
      for (int i = 0; i < GetCutContainer(upd)->GetEntriesFast(); i++) {
        list1->Add(((CutCollection*) GetCutContainer(upd)->UncheckedAt(i))->Report());
      }
      pack->AddObject(list1);
    };
    if (fSize > 0) getList(ECutUpdate::kEvent);
    if (fSize > 1) getList(ECutUpdate::kTrack);
    if (fSize > 2) getList(ECutUpdate::kTwoTrack);
    if (fSize > 3) getList(ECutUpdate::kTwoTrackBackground);
    pack->SetComment(this->ClassName());
    return pack;
  }

  Int_t CutContainer::GetEventCollectionsNo() const {
    if (fSize < 1) return 0;
    return GetCutContainer(ECutUpdate::kEvent)->GetEntriesFast();
  }

  Int_t CutContainer::GetTrackCollectionsNo() const {
    if (fSize < 2) return 0;
    return GetCutContainer(ECutUpdate::kTrack)->GetEntriesFast();
  }

  Int_t CutContainer::GetTwoTrackCollectionsNo() const {
    if (fSize < 3) return 0;
    return GetCutContainer(ECutUpdate::kTwoTrack)->GetEntriesFast();
  }

  Int_t CutContainer::GetTwoTrackCollectionsBackgroundNo() const {
    if (fSize < 4) return 0;
    return GetCutContainer(ECutUpdate::kTwoTrackBackground)->GetEntriesFast();
  }

  void CutContainer::MakeDummyCopies(ECutUpdate update, CutContainer* other, Bool_t copy_link) {
    if (static_cast<Int_t>(update) < 0) return;
    if (this->fSize <= static_cast<Int_t>(update)) {
      Cout::PrintInfo("Cannot Make Dummy copy of CutCollection!lack of space "
                      "in  target cut container!",
                      EInfo::kError);
      return;
    }
    if (other->fSize <= static_cast<Int_t>(update)) {
      Cout::PrintInfo("Cannot Make Dummy copy of CutCollection!! lack of "
                      "object in source cut container",
                      EInfo::kError);
      return;
    }
    if (this->fInit == kTRUE || other->fInit == kFALSE) {
      Cout::PrintInfo("You can't copy sub containers from not initialized cut container to initialized cut container!! ",
                      EInfo::kLowWarning);
    }
    for (int i = 0; i < other->GetCutContainer(update)->GetEntriesFast(); i++) {
      CutCollection* from = (CutCollection*) other->GetCutContainer(update)->UncheckedAt(i);
      CutCollection* to   = (CutCollection*) this->GetCutContainer(update)->UncheckedAt(i);
      to->MakeDummyCopy(from, copy_link);
    }
  }

  Int_t CutContainer::GetCollectionsNo(ECutUpdate update) const {
    switch (update) {
      case ECutUpdate::kEvent: {
        return GetEventCollectionsNo();
      } break;
      case ECutUpdate::kTrack: {
        return GetTrackCollectionsNo();
      } break;
      case ECutUpdate::kTwoTrack: {
        return GetTwoTrackCollectionsNo();
      } break;
      case ECutUpdate::kTwoTrackBackground: {
        return GetTwoTrackCollectionsBackgroundNo();
      } break;
      case ECutUpdate::kNo: {
        return 0;
        // DO nothing
      } break;
    }
    return 0;
  }

  void CutContainer::AddVirtualCut(ECutUpdate update, Int_t col) {
    switch (update) {
      case ECutUpdate::kEvent: {
        EventVirtualCut v;
        v.SetCollectionID(col);
        AddCut(v);
      } break;
      case ECutUpdate::kTrack: {
        TrackVirtualCut v;
        v.SetCollectionID(col);
        AddCut(v);
      } break;
      case ECutUpdate::kTwoTrack: {
        TwoTrackVirtualCut v;
        v.SetCollectionID(col);
        AddCut(v, "sig");
      } break;
      case ECutUpdate::kTwoTrackBackground: {
        TwoTrackVirtualCut v;
        v.SetCollectionID(col);
        AddCut(v, "bckg");
      } break;
      case ECutUpdate::kNo: {
        /* do nothing */
      } break;
    }
  }

  Bool_t CutContainer::LinkCollections(ECutUpdate first, ECutUpdate last, ELinkPolicy policy) {
    Int_t lowLinks  = GetCollectionsNo(first);
    Int_t highLinks = GetCollectionsNo(last);
    if (lowLinks == 0) AddVirtualCut(first, 0);
    if (highLinks == 0) AddVirtualCut(last, 0);
    switch (policy) {
      case ELinkPolicy::kOneToMany: {
        if (lowLinks != 1) {
          Cout::PrintInfo("EventAna::LinkCollections one-to-many to much first collections!", EInfo::kError);
          return kFALSE;
        }
        for (int i = 0; i < highLinks; i++) {
          LinkCollections(first, 0, last, i);
        }
        return kTRUE;
      } break;
      case ELinkPolicy::kEqual: {
        if (lowLinks > highLinks) {
          Int_t jump = lowLinks / highLinks;
          if (lowLinks % highLinks) {
            Cout::PrintInfo("EventAna::LinkCollections equal link cannot group cuts!", EInfo::kError);
            return kFALSE;
          }
          for (int i = 0; i < highLinks; i++) {
            for (int j = 0; j < jump; j++) {
              LinkCollections(first, i * jump + j, last, i);
            }
          }
          return kTRUE;
        } else {
          Int_t jump = highLinks / lowLinks;
          if (highLinks % lowLinks) {
            Cout::PrintInfo("EventAna::LinkCollections equal link cannot group cuts!", EInfo::kError);
            return kFALSE;
          }
          for (int i = 0; i < lowLinks; i++) {
            for (int j = 0; j < jump; j++) {
              LinkCollections(first, i, last, i * jump + j);
            }
          }
          return kTRUE;
        }
      } break;
      case ELinkPolicy::kReplicateLast: {
        for (int i = 1; i < lowLinks; i++) {
          for (int j = 0; j < highLinks; j++) {
            ReplicateCollection(last, j, i * highLinks + j);
          }
        }
        for (int i = 0; i < lowLinks; i++) {
          for (int j = 0; j < highLinks; j++) {
            LinkCollections(first, i, last, i * highLinks + j);
          }
        }
        return kTRUE;
      } break;
      case ELinkPolicy::kReplicateFirst: {
        for (int i = 1; i < highLinks; i++) {
          for (int j = 0; j < lowLinks; j++) {
            ReplicateCollection(first, j, i * lowLinks + j);
          }
        }
        for (int i = 0; i < highLinks; i++) {
          for (int j = 0; j < lowLinks; j++) {
            LinkCollections(first, i * lowLinks + j, last, i);
          }
        }
        return kTRUE;
      } break;
      case ELinkPolicy::kAnyToAny: {
        for (int i = 0; i < lowLinks; i++) {
          for (int j = 0; j < highLinks; j++) {
            LinkCollections(first, i, last, j);
          }
        }
        return kTRUE;
      } break;
    }
    return kFALSE;
  }


}  // namespace Hal
