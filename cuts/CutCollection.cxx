/*
 * CutCollection.cxx
 *
 *  Created on: 06-08-2013
 *      Author: Daniel Wielanek
 *      E-mail: daniel.wielanek@gmail.com
 *      Warsaw University of Technology, Faculty of Physics
 */

#include "CutCollection.h"

#include <FairLogger.h>
#include <TClass.h>
#include <TCollection.h>
#include <TList.h>

#include "Cout.h"
#include "CutMonitor.h"
#include "CutMonitorX.h"
#include "CutMonitorXY.h"
#include "CutMonitorXYZ.h"
#include "Event.h"
#include "EventCut.h"
#include "HalStdString.h"
#include "Package.h"
#include "Parameter.h"
#include "Track.h"
#include "TrackCut.h"
#include "TwoTrack.h"
#include "TwoTrackCut.h"

// template CutCollection<UEvent,UParticle>//
namespace Hal {
  CutCollection::CutCollection(TObjArray** container, Int_t cont_size, ECutUpdate mode, Int_t collectionNo) :
    fCutMonitors(NULL),
    fCuts(NULL),
    fFastCuts(NULL),
    fPassedSlow(0),
    fFailedSlow(0),
    fPassedFast(0),
    fFailedFast(0),
    fFastCutsNo(0),
    fSlowCutsNo(0),
    fCutMonitorsNo(0),
    fPreviousAddress(NULL),
    fNextAddress(NULL),
    fNextNo(NULL),
    fPreviousNo(NULL),
    fInit(kFALSE),
    fDummy(kFALSE),
    fCollectionID(0),
    fContainerSize(cont_size),
    fStep(1),
    fCutContainerArr(NULL),  // this is needed to link cut monitors in case of cloning
    fNextUsed(kFALSE),
    fPrevUsed(kFALSE) {
    fCutContainerArr = container;
    fCutMonitors     = new TObjArray();
    fCuts            = new TObjArray();
    fFastCuts        = new TObjArray();
    fInit            = kFALSE;
    fMode            = mode;
    fCollectionID    = collectionNo;
  }

  void CutCollection::AddCut(Cut* cut, Option_t* opt) {
    if (fDummy) return;
    if (cut == NULL) {
      LOG(WARNING) << "Empty cut";
      return;
    }
    TString option    = opt;
    Bool_t keepDouble = HalStd::FindParam(option, "double");
    if (!HalStd::FindParam(option, "fast")) {  // add normal cut
      for (int i = 0; i < fCuts->GetEntriesFast(); i++) {
        if (cut->CutName() == ((Cut*) fCuts->UncheckedAt(i))->CutName()) {
          LOG(WARNING) << Form("Cut used %s in collection no %i, cut will be removed", cut->ClassName(), cut->GetCollectionID());
          return;
        }
      }
      for (int i = 0; i < fFastCuts->GetEntriesFast(); i++) {
        if (cut->CutName() == ((Cut*) fFastCuts->UncheckedAt(i))->CutName()) {
          if (!keepDouble) {
            LOG(WARNING) << Form("Cut used %s in collection no %i in fast cuts group,  cut will "
                                 "be "
                                 "removed",
                                 cut->ClassName(),
                                 cut->GetCollectionID());
            return;
          }
        }
      }
      fCuts->AddLast(cut);
    } else {
      for (int i = 0; i < fFastCuts->GetEntriesFast(); i++) {
        if (cut->CutName() == ((Cut*) fFastCuts->UncheckedAt(i))->CutName()) {
          LOG(WARNING) << Form("Cut used %s in collection no %i in as fast, cut will be "
                               "removed",
                               cut->ClassName(),
                               cut->GetCollectionID());
          return;
        }
      }
      if (!keepDouble) {
        for (int i = 0; i < fCuts->GetEntriesFast(); i++) {
          if (cut->CutName() == ((Cut*) fCuts->UncheckedAt(i))->CutName()) {
            LOG(WARNING) << Form(
              "Cut used %s in collection no %i but cut will be removed", cut->ClassName(), cut->GetCollectionID());
            return;
          }
        }
      }
      fFastCuts->AddLast(cut);
    }
  }

  Bool_t CutCollection::CheckIfComptatiblie(Cut* cut1, Cut* cut2) {
    if (cut1 == NULL || cut2 == NULL) return kFALSE;
    if (cut1->GetUpdateRatio() != cut2->GetUpdateRatio()) return kFALSE;
    if (cut1->GetCollectionID() != cut2->GetCollectionID()) return kFALSE;
    return kTRUE;
  }

  void CutCollection::AddCutMonitor(CutMonitor* monitor) {
    if (fDummy) return;
    if (monitor == NULL) {
      LOG(ERROR) << "Empty monitor";
      return;
    }

    for (int i = 0; i < fCutMonitors->GetEntriesFast(); i++) {
      if (monitor == (CutMonitor*) fCutMonitors->UncheckedAt(i)) {
        LOG(WARNING) << "Monitor cut used";
        return;
      }
    }
    fCutMonitors->AddLast(monitor);
  }

  void CutCollection::Update(Bool_t val, TObject* obj) {
    if (val) {
      ++fPassedSlow;
    } else {
      ++fFailedSlow;
    }
    for (int i = 0; i < fCutMonitorsNo; i++)
      ((CutMonitor*) fCutMonitors->UncheckedAt(i))->Update(val, obj);
  }

  void CutCollection::Init(Int_t task_id) {
    if (fDummy) {
      fInit = kTRUE;
      if (fNextNo == NULL) {
        fNextNo = new Array_1<Int_t>();
        fNextNo->MakeBigger(1);
        fNextNo->Set(0, 0);
      }
      if (fPreviousNo == NULL) {
        fPreviousNo = new Array_1<Int_t>();
        fPreviousNo->MakeBigger(1);
        fPreviousNo->Set(0, 0);
      }
      return;
    }
    if (fInit) {
      LOG(ERROR) << "CutCollection has been initialized";
    } else {
      for (int i = 0; i < fCuts->GetEntriesFast(); i++) {
        Bool_t ok = ((Cut*) fCuts->UncheckedAt(i))->Init(task_id);
        if (!ok) {
          LOG(DEBUG2) << Form(
            "Failed to init %s in cut collection %i", ((Cut*) fCuts->UncheckedAt(i))->CutName().Data(), GetCollectionID());
          fCuts->RemoveAt(i);
          fCuts->Compress();
          i--;
        } else {
          LOG(DEBUG4) << Form(
            "Succesfully  inited %s in cut collection %i", ((Cut*) fCuts->UncheckedAt(i))->CutName().Data(), GetCollectionID());
        }
      }
      for (int i = 0; i < fFastCuts->GetEntriesFast(); i++) {
        Bool_t ok = ((Cut*) fFastCuts->UncheckedAt(i))->Init(task_id);
        if (!ok) {
          LOG(DEBUG2) << Form(
            "Failed to init %s in cut collection %i", ((Cut*) fFastCuts->UncheckedAt(i))->CutName().Data(), GetCollectionID());
          fFastCuts->RemoveAt(i);
          fFastCuts->Compress();
          i--;
        } else {
          LOG(DEBUG4) << Form("Succesfully inited %s in cut collection %i",
                              ((Cut*) fFastCuts->UncheckedAt(i))->CutName().Data(),
                              GetCollectionID());
        }
      }
      LOG(DEBUG4) << "Initializing cut monitors";
      AdvancedMonitorInitialization(task_id);
      fInit = kTRUE;
      if (fNextNo == NULL) {
        fNextNo = new Array_1<Int_t>();
        fNextNo->MakeBigger(1);
        fNextNo->Set(0, 0);
      } else {
        fNextUsed = kTRUE;
      }
      if (fPreviousNo == NULL) {
        fPreviousNo = new Array_1<Int_t>();
        fPreviousNo->MakeBigger(1);
        fPreviousNo->Set(0, 0);
      } else {
        fPrevUsed = kTRUE;
      }
    }
    fFastCutsNo    = fFastCuts->GetEntriesFast();
    fSlowCutsNo    = fCuts->GetEntriesFast();
    fCutMonitorsNo = fCutMonitors->GetEntriesFast();
  }

  Int_t CutCollection::MatchCut(Cut* adreess, CutCollection* input) {
    for (int i = 0; i < input->GetCutNo(); i++) {
      if (adreess == input->GetCut(i)) return i;
    }
    return -1;
  }

  CutMonitor* CutCollection::GetCutMonitor(Int_t index) const { return (CutMonitor*) fCutMonitors->At(index); }

  void CutCollection::AddPreviousAddr(Int_t value, Bool_t backround) {
    if (fDummy) return;
    if (value < 0) return;  // ignore
    if (fPreviousAddress == NULL) {
      fPreviousAddress = new Array_2<Int_t>();
      fPreviousNo      = new Array_1<Int_t>();
    }
    Int_t layer = 0;
    if (backround) layer = 1;
    fPreviousNo->MakeBigger(layer + 1);
    fPreviousAddress->MakeBigger(layer + 1, fPreviousNo->Get(layer) + 1);
    fPreviousAddress->Set(layer, fPreviousNo->Get(layer), value);
    fPreviousNo->IncrementAfter(layer);
    fPrevUsed = kTRUE;
  }

  void CutCollection::AddNextAddr(Int_t value, Bool_t backround) {
    if (fDummy) return;
    if (value < 0) return;  // ignore
    Int_t layer = 0;
    if (backround) layer = 1;
    if (fNextAddress == NULL) {
      fNextAddress = new Array_2<Int_t>();
      fNextNo      = new Array_1<Int_t>();
    }
    fNextNo->MakeBigger(layer + 1);
    fNextAddress->MakeBigger(layer + 1, fNextNo->Get(layer) + 1);
    fNextAddress->Set(layer, fNextNo->Get(layer), value);
    fNextNo->IncrementAfter(layer);
    fNextUsed = kTRUE;
  }

  Cut* CutCollection::FindCut(TString name) const {
    LOG(DEBUG4) << Form("Looking for %s in subcontainer %i", name.Data(), GetCollectionID());
    for (int i = 0; i < fCuts->GetEntriesFast(); i++) {
      TString name2 = ((Cut*) fCuts->UncheckedAt(i))->CutName();
      if (name2 == name) { return (Cut*) fCuts->UncheckedAt(i); }
    }  // cut not found in standard cuts, looking in complex cuts
    return NULL;
  }

  CutCollection* CutCollection::Replicate(Int_t new_collection) const {
    if (fInit == kTRUE) {
      LOG(WARNING) << "Cant replicate initialized CutCollection - this lead "
                      "to mess with cut monitors !";
    }
    CutCollection* clone = new CutCollection(fCutContainerArr, fContainerSize, fMode, new_collection);
    clone->fStep         = this->fStep;
    for (int i = 0; i < this->fCuts->GetEntriesFast(); i++) {
      Cut* cut = ((Cut*) fCuts->UncheckedAt(i))->MakeCopy();
      cut->SetCollectionID(new_collection);
      clone->AddCut(cut);
    }
    for (int i = 0; i < fFastCuts->GetEntriesFast(); i++) {
      Cut* cut = ((Cut*) fFastCuts->UncheckedAt(i))->MakeCopy();
      cut->SetCollectionID(new_collection);
      clone->fFastCuts->AddLast(cut);
    }
    for (int i = 0; i < this->fCutMonitors->GetEntriesFast(); i++) {
      TString orig = fCutMonitors->UncheckedAt(i)->ClassName();
      if (orig == "CutMonitorX") {
        CutMonitorX* cutmon = new CutMonitorX(*(CutMonitorX*) fCutMonitors->UncheckedAt(i));
        clone->AddCutMonitor(cutmon);
      } else if (orig == "CutMonitorXY") {
        CutMonitorXY* cutmon = new CutMonitorXY(*(CutMonitorXY*) fCutMonitors->UncheckedAt(i));
        clone->AddCutMonitor(cutmon);
      } else if (orig == "CutMonitorXYZ") {
        CutMonitorXYZ* cutmon = new CutMonitorXYZ(*(CutMonitorXYZ*) fCutMonitors->UncheckedAt(i));
        clone->AddCutMonitor(cutmon);
      } else {
        clone->AddCutMonitor((CutMonitor*) fCutMonitors->UncheckedAt(i)->Clone());
      }
    }
    clone->fCutMonitorsNo = fCutMonitorsNo;
    clone->fFastCutsNo    = fFastCutsNo;
    clone->fSlowCutsNo    = fSlowCutsNo;
    return clone;
  }

  void CutCollection::AdvancedMonitorInitialization(Int_t task_id) {
    // linking cuts by names with pointers with cuts
    Int_t oryginal_monitors_no = fCutMonitors->GetEntries();
    Int_t prev_size            = 0;
    if (fPrevUsed) prev_size = fPreviousNo->GetSize();
    for (int i = 0; i < oryginal_monitors_no; i++) {
      CutMonitor* cutmon = (CutMonitor*) fCutMonitors->UncheckedAt(i);
      if (cutmon->ObjMonitor()) {
        Bool_t init = cutmon->Init(task_id);
        if (init == kFALSE) {
          LOG(WARNING) << Form("Problem with initalization  [%s]", cutmon->ClassName());
          fCutMonitors->RemoveAt(i);
        }
        continue;
      }
      TString monName = cutmon->ClassName();
      if (monName == "CutMonitorX") {
        TString cut = cutmon->GetCutName(0);
        Cut* newCut = this->FindCut(cut);
        if (newCut) {
          if (newCut->GetCutSize() <= cutmon->GetCutParameter(0)) {
            LOG(WARNING) << Form("Problem with initalization CutMonitorXY [%s]", newCut->ClassName());
            LOG(WARNING) << Form(
              "Cut %s has only %i not %i parameters", newCut->ClassName(), newCut->GetCutSize(), cutmon->GetCutParameter(0));
            fCutMonitors->RemoveAt(i);
          } else {
            cutmon->AddForcedCut(newCut, 0);
          }
        } else {
          LOG(ERROR) << Form("CutMonitorX [%s] not found!", cut.Data());
          fCutMonitors->RemoveAt(i);
        }
      } else if (monName == "CutMonitorXY") {
        TString cut1_name = cutmon->GetCutName(0);
        TString cut2_name = cutmon->GetCutName(1);
        TObjArray* Ncut1  = LocateCuts(cut1_name);
        TObjArray* Ncut2  = LocateCuts(cut2_name);

        if (Ncut1->GetEntries() == 0 || Ncut2->GetEntries() == 0) {
          LOG(WARNING) << Form("Problem with initalization CutMonitorXY [%s] vs [%s]", cut1_name.Data(), cut2_name.Data());
          if (Ncut1->GetEntries() == 0) LOG(WARNING) << Form("Cut %s not found", cut1_name.Data());
          if (Ncut2->GetEntries() == 0) LOG(WARNING) << Form("Cut %s not found", cut2_name.Data());
          fCutMonitors->RemoveAt(i);
        } else if (Ncut1->GetEntriesFast() > 1 || Ncut2->GetEntriesFast() > 1 || prev_size > 1) {
          LOG(ERROR) << Form(
            "There is too many links or cuts cut monitor %s %s will be removed", cut1_name.Data(), cut2_name.Data());
          fCutMonitors->RemoveAt(i);
        } else {
          Cut* cut1 = (Cut*) Ncut1->UncheckedAt(0);
          Cut* cut2 = (Cut*) Ncut2->UncheckedAt(0);
          if (cut1->GetCutSize() <= cutmon->GetCutParameter(0)) {
            LOG(WARNING) << Form("Problem with initalization CutMonitorXY [%s] vs [%s]", cut1_name.Data(), cut2_name.Data());
            LOG(WARNING) << Form(
              "Cut %s has only %i not %i parameters", cut1->ClassName(), cut1->GetCutSize(), cutmon->GetCutParameter(0));
            fCutMonitors->RemoveAt(i);
          } else if (cut2->GetCutSize() <= cutmon->GetCutParameter(1)) {
            LOG(WARNING) << Form("Problem with initalization CutMonitorXY [%s] vs [%s]", cut1_name.Data(), cut2_name.Data());
            LOG(WARNING) << Form(
              "Cut %s has only %i not %i parameters", cut1->ClassName(), cut1->GetCutSize(), cutmon->GetCutParameter(1));
            fCutMonitors->RemoveAt(i);
          } else {
            cutmon->AddForcedCut(cut1, 0);
            cutmon->AddForcedCut(cut2, 1);
          }
        }
        Ncut1->SetOwner(kFALSE);
        Ncut2->SetOwner(kFALSE);
        delete Ncut1;
        delete Ncut2;
      } else if (monName == "CutMonitorXYZ") {
        TString cut1_name = cutmon->GetCutName(0);
        TString cut2_name = cutmon->GetCutName(1);
        TString cut3_name = cutmon->GetCutName(2);
        TObjArray* Ncut1  = LocateCuts(cut1_name);
        TObjArray* Ncut2  = LocateCuts(cut2_name);
        TObjArray* Ncut3  = LocateCuts(cut3_name);
        if (Ncut1->GetEntries() == 0 || Ncut2->GetEntries() == 0 || Ncut3->GetEntries() == 0) {
          LOG(WARNING) << Form(
            " Problem with CutMonitorXYZ [%s] vs [%s] vs [%s]", cut1_name.Data(), cut2_name.Data(), cut3_name.Data());
          if (Ncut1->GetEntries() == 0) LOG(WARNING) << Form("Cut %s not found", cut1_name.Data());
          if (Ncut2->GetEntries() == 0) LOG(WARNING) << Form("Cut %s not found", cut2_name.Data());
          if (Ncut3->GetEntries() == 0) LOG(WARNING) << Form("Cut %s not found", cut3_name.Data());
          fCutMonitors->RemoveAt(i);
        } else if (Ncut1->GetEntries() > 1 || Ncut2->GetEntries() > 1 || Ncut3->GetEntriesFast() > 1 || prev_size > 1) {
          LOG(ERROR) << "There is too many links or cuts monitor %s %s will be removed";
          fCutMonitors->RemoveAt(i);
        } else {
          Cut* cut1      = (Cut*) Ncut1->UncheckedAt(0);
          Cut* cut2      = (Cut*) Ncut2->UncheckedAt(0);
          Cut* cut3      = (Cut*) Ncut3->UncheckedAt(0);
          Int_t own_cuts = 0;
          ECutUpdate upd = fMode;
          if (fMode == ECutUpdate::kTwoTrackBackgroundUpdate) { upd = ECutUpdate::kTwoTrackUpdate; }
          if (cut1->GetUpdateRatio() == upd) own_cuts = 1;
          if (cut2->GetUpdateRatio() == upd) own_cuts += 2;
          if (cut3->GetUpdateRatio() == upd) own_cuts += 4;
          ;
          switch (own_cuts) {
            case 0: {
              LOG(WARNING) << "CutMonitorXYZ has no cuts compatible with "
                              "container that own it";
              fCutMonitors->RemoveAt(i);
            } break;
            case 1: {  // 1 local
              cutmon->AddForcedCut(cut1, 0);
              if (CheckIfComptatiblie(cut2, cut3)) {
                cutmon->AddForcedCut(cut2, 1);
                cutmon->AddForcedCut(cut3, 2);
              } else {
                LOG(WARNING) << "Couldn't find compatible cuts for monitoring";
                fCutMonitors->RemoveAt(i);
              }
            } break;
            case 2:  // 2 local
              cutmon->AddForcedCut(cut2, 1);
              if (CheckIfComptatiblie(cut1, cut3)) {
                cutmon->AddForcedCut(cut1, 0);
                cutmon->AddForcedCut(cut3, 2);
              } else {
                LOG(WARNING) << "Couldn't find compatible cuts for monitoring";
                fCutMonitors->RemoveAt(i);
              }
              break;
            case 4:  // 3 local
              cutmon->AddForcedCut(cut3, 2);
              if (CheckIfComptatiblie(cut1, cut2)) {
                cutmon->AddForcedCut(cut1, 0);
                cutmon->AddForcedCut(cut2, 1);
              } else {
                LOG(WARNING) << "Couldn't find compatible cuts for monitoring";
                fCutMonitors->RemoveAt(i);
              }
              break;
            default: {
              if (cut1->GetCutSize() <= cutmon->GetCutParameter(0)) {
                LOG(WARNING) << Form("Problem with initalization CutMonitorXY [%s] vs [%s] vs "
                                     "[%s]",
                                     cut1_name.Data(),
                                     cut2_name.Data(),
                                     cut3_name.Data());
                LOG(WARNING) << Form(
                  "Cut %s has only %i not %i parameters", cut1->ClassName(), cut1->GetCutSize(), cutmon->GetCutParameter(0));
                fCutMonitors->RemoveAt(i);
              } else if (cut2->GetCutSize() <= cutmon->GetCutParameter(1)) {
                LOG(WARNING) << Form("Problem with initalization CutMonitorXY [%s] vs [%s] vs "
                                     "[%s]",
                                     cut1_name.Data(),
                                     cut2_name.Data(),
                                     cut3_name.Data());
                LOG(WARNING) << Form(
                  "Cut %s has only %i not %i parameters", cut2->ClassName(), cut2->GetCutSize(), cutmon->GetCutParameter(1));
                fCutMonitors->RemoveAt(i);
              } else if (cut3->GetCutSize() <= cutmon->GetCutParameter(2)) {
                LOG(WARNING) << Form("Problem with initalization CutMonitorXY [%s] vs [%s] vs "
                                     "[%s]",
                                     cut1_name.Data(),
                                     cut2_name.Data(),
                                     cut3_name.Data());
                LOG(WARNING) << Form(
                  "Cut %s has only %i not %i parameters", cut3->ClassName(), cut3->GetCutSize(), cutmon->GetCutParameter(2));
                fCutMonitors->RemoveAt(i);
              } else {
                cutmon->AddForcedCut(cut1, 0);
                cutmon->AddForcedCut(cut2, 1);
                cutmon->AddForcedCut(cut3, 2);
              }
            } break;
          }
        }
        Ncut1->SetOwner(kFALSE);
        Ncut2->SetOwner(kFALSE);
        Ncut3->SetOwner(kFALSE);
        delete Ncut1;
        delete Ncut2;
        delete Ncut3;
      } else {
        LOG(FATAL) << Form("Problems with class name %s of cuts this is an critial error", monName.Data());
      }
    }
    fCutMonitors->Compress();
    // standard initialization
    for (int i = 0; i < fCutMonitors->GetEntriesFast(); i++) {
      CutMonitor* mon = ((CutMonitor*) fCutMonitors->UncheckedAt(i));
      mon->SetCollectionID(this->fCollectionID);
      if (!mon->ObjMonitor()) mon->Init(task_id);
    }
  }

  TObjArray* CutCollection::LocateInLowerCollections(TString cut) {
    TObjArray* obj = new TObjArray();
    obj->SetOwner(kFALSE);
    if (this->fMode == ECutUpdate::kEventUpdate) { return obj; }
    if (this->fMode == ECutUpdate::kTrackUpdate) {
      for (int i = 0; i < this->GetPrevNo(); i++) {
        CutCollection* subcont = (CutCollection*) (GetCutContainerArray(ECutUpdate::kEventUpdate)->At(this->GetPrevAddr(i)));
        if (subcont->FindCut(cut)) { obj->AddLast(subcont->FindCut(cut)); }
      }
      return obj;
    }
    if (this->fMode == ECutUpdate::kTwoTrackUpdate) {
      // look over track containers
      for (int i = 0; i < this->GetPrevNo(); i++) {
        CutCollection* subcont = (CutCollection*) (GetCutContainerArray(ECutUpdate::kTrackUpdate)->At(this->GetPrevAddr(i)));
        if (subcont->FindCut(cut)) { obj->AddLast(subcont->FindCut(cut)); }
      }
      // still not found - we need go deeper !! to event cuts
      for (int i = 0; i < this->GetPrevNo(); i++) {
        CutCollection* subcont = (CutCollection*) (GetCutContainerArray(ECutUpdate::kTrackUpdate)->At(this->GetPrevAddr(i)));
        for (int j = 0; j < subcont->GetPrevNo(); j++) {
          CutCollection* subsubcont =
            (CutCollection*) (GetCutContainerArray(ECutUpdate::kTwoTrackUpdate)->At(this->GetPrevAddr(j)));
          if (subsubcont->FindCut(cut)) { obj->AddLast(subsubcont->FindCut(cut)); }
        }
      }
      return obj;
    }
    if (this->fMode == ECutUpdate::kTwoTrackBackgroundUpdate) {
      // look over track containers
      for (int i = 0; i < this->GetPrevNo(); i++) {
        CutCollection* subcont = (CutCollection*) (GetCutContainerArray(ECutUpdate::kTrackUpdate)->At(this->GetPrevAddr(i)));
        if (subcont->FindCut(cut)) { obj->AddLast(subcont->FindCut(cut)); }
      }
      // still not found - we need go deeper !! to event cuts
      for (int i = 0; i < this->GetPrevNo(); i++) {
        CutCollection* subcont = (CutCollection*) (GetCutContainerArray(ECutUpdate::kTrackUpdate)->At(this->GetPrevAddr(i)));
        for (int j = 0; j < subcont->GetPrevNo(); j++) {
          CutCollection* subsubcont =
            (CutCollection*) (GetCutContainerArray(ECutUpdate::kTwoTrackBackgroundUpdate)->At(this->GetPrevAddr(j)));
          if (subsubcont->FindCut(cut)) { obj->AddLast(subsubcont->FindCut(cut)); }
        }
      }
      return obj;
    }
    return obj;
  }

  Int_t CutCollection::Compare(const TObject* obj) const {
    if (obj->ClassName() != this->ClassName()) return 0;
    Int_t trigg = ((CutCollection*) obj)->GetCollectionID();
    if (trigg == fCollectionID) return 0;
    if (trigg > fCollectionID) return -1;
    return 1;
  }

  TObjArray* CutCollection::LocateCuts(TString classname) {
    TObjArray* arr = new TObjArray();
    arr->SetOwner(kFALSE);
    ECutUpdate cut_up = ECutUpdate::kNoUpdate;
    cut_up            = GetUpdateFromName(classname);
    if (fMode == cut_up) {
      Cut* cut = FindCut(classname);
      arr->AddLast(cut);
      return arr;
    }
    if (fMode == ECutUpdate::kTwoTrackBackgroundUpdate && cut_up == ECutUpdate::kTwoTrackUpdate) {
      Cut* cut = FindCut(classname);
      arr->AddLast(cut);
      return arr;
    } else if (fPrevUsed) {
      LOG(DEBUG3) << "Looking for cuts in lower collections";
      arr = LocateInLowerCollections(classname);
      return arr;
    } else {
      LOG(DEBUG3) << "Cut not found cut and no lower collections present";
      return arr;
    }
  }

  void CutCollection::PrintInfo() const {
    Cout::InStars(Form("Subcontainer trig %i", fCollectionID));
    TString mode;
    switch (fMode) {
      case (ECutUpdate::kEventUpdate): {
        mode = "Event Upd";
      } break;
      case (ECutUpdate::kTrackUpdate): {
        mode = "Track upd";
      } break;
      case (ECutUpdate::kTwoTrackUpdate): {
        mode = "TTrack mod";
      } break;
      case (ECutUpdate::kTwoTrackBackgroundUpdate): {
        mode = "TTRack mixed";
      } break;
      default: LOG(WARNING) << "Unknown update mode inf CutCollection"; break;
    }
    Cout::Text(Form("Mode %s", mode.Data()), "L", kYellow);
    for (int i = 0; i < fCuts->GetEntriesFast(); i++) {
      Cout::Text(fCuts->UncheckedAt(i)->ClassName(), "L");
    }

    Cout::Text("Fast cuts", "L", kYellow);
    for (int i = 0; i < fFastCuts->GetEntriesFast(); i++) {
      Cout::Text(fFastCuts->UncheckedAt(i)->ClassName(), "L");
    }

    for (int i = 0; i < fCutMonitors->GetEntriesFast(); i++) {
      Cout::Text(fCutMonitors->UncheckedAt(i)->ClassName(), "L");
      CutMonitor* cutmon = (CutMonitor*) fCutMonitors->UncheckedAt(i);
      Cout::Database(1, cutmon->GetCutName(0).Data());
    }
  }

  Bool_t CutCollection::PassEvent(Event* obj) {
    Bool_t result = kTRUE;
    for (int i = 0; i < fFastCutsNo; i++) {
      if (kFALSE == ((EventCut*) fFastCuts->UncheckedAt(i))->Pass(obj)) {
        ++fFailedFast;
        return kFALSE;
      }
    }
    ++fPassedFast;
    for (int i = 0; i < fSlowCutsNo; i++) {
      if (kFALSE == ((EventCut*) fCuts->UncheckedAt(i))->Pass(obj)) { result = kFALSE; }
    }
    Update(result, obj);
    return result;
  }

  Bool_t CutCollection::PassTrack(Track* obj) {
    Bool_t result = kTRUE;
    for (int i = 0; i < fFastCutsNo; i++) {
      if (kFALSE == ((TrackCut*) fFastCuts->UncheckedAt(i))->Pass(obj)) {
        ++fFailedFast;
        return kFALSE;
      }
    }
    ++fPassedFast;
    for (int i = 0; i < fSlowCutsNo; i++) {
      if (kFALSE == ((TrackCut*) fCuts->UncheckedAt(i))->Pass(obj)) { result = kFALSE; }
    }
    Update(result, obj);
    return result;
  }

  Bool_t CutCollection::PassTwoTrack(TwoTrack* pair) {
    Bool_t result = kTRUE;
    for (int i = 0; i < fFastCutsNo; i++) {
      if (kFALSE == ((TwoTrackCut*) fFastCuts->UncheckedAt(i))->Pass(pair)) {
        ++fFailedFast;
        return kFALSE;
      }
    }
    ++fPassedFast;
    for (int i = 0; i < fSlowCutsNo; i++) {
      if (kFALSE == ((TwoTrackCut*) fCuts->UncheckedAt(i))->Pass(pair)) { result = kFALSE; }
    }
    Update(result, pair);
    return result;
  }

  Package* CutCollection::Report() const {
    Package* pack = new Package(this, kTRUE);
    switch (fMode) {
      case (ECutUpdate::kEventUpdate): {
        pack->AddObject(new ParameterString("UpdateMode", "Event"));
      } break;
      case (ECutUpdate::kTrackUpdate): {
        pack->AddObject(new ParameterString("UpdateMode", "Track"));
      } break;
      case (ECutUpdate::kTwoTrackUpdate): {
        pack->AddObject(new ParameterString("UpdateMode", "TwoTrack"));
      } break;
      default: pack->AddObject(new ParameterString("UpdateMode", "Unknown")); break;
    }
    pack->AddObject(new ParameterInt("CollectionID", fCollectionID));
    pack->AddObject(new ParameterInt("CutMonitorNo", fCutMonitors->GetEntriesFast()));
    pack->AddObject(new ParameterInt("CutNo", fCuts->GetEntriesFast()));
    pack->AddObject(new ParameterULong64("PassedFast", fPassedFast, '+'));
    pack->AddObject(new ParameterULong64("FailedFast", fFailedFast, '+'));
    pack->AddObject(new ParameterULong64("PassedSlow", fPassedSlow, '+'));
    pack->AddObject(new ParameterULong64("FailedSlow", fFailedSlow, '+'));
    if (fDummy) {
      pack->AddObject(new ParameterString("State", "Dummy"));
    } else {
      pack->AddObject(new ParameterString("State", "Normal"));
    }
    if (fNextAddress && fNextUsed) {
      pack->AddObject(new ParameterInt("NextLayers", fNextAddress->GetSize()));
      for (Int_t j = 0; j < fNextAddress->GetSize(); j++) {
        TList* list = new TList();
        list->SetOwner(kTRUE);
        list->SetName(Form("NextObj_%i", j));
        for (int i = 0; i < fNextNo->Get(j); i++) {
          list->AddAt(new ParameterInt(Form("%i", i), fNextAddress->Get(j, i)), i);
        }
        pack->AddObject(list);
      }
    }
    if (fPreviousAddress && fPrevUsed) {
      pack->AddObject(new ParameterInt("PreviousLayers", fPreviousAddress->GetSize()));
      for (Int_t j = 0; j < fPreviousAddress->GetSize(); j++) {
        TList* list = new TList();
        list->SetOwner(kTRUE);
        list->SetName(Form("PrevObj_%i", j));
        for (int i = 0; i < fPreviousNo->Get(j); i++) {
          list->AddAt(new ParameterInt(Form("%i", i), fPreviousAddress->Get(j, i)), i);
        }
        pack->AddObject(list);
      }
    }
    TList* CutMonitorList = new TList();
    CutMonitorList->SetOwner(kTRUE);
    CutMonitorList->SetName("CutMonitorList");
    for (int i = 0; i < fCutMonitors->GetEntriesFast(); i++)
      CutMonitorList->Add(((CutMonitor*) fCutMonitors->At(i))->Report());
    TList* CutList = new TList();
    CutList->SetOwner(kTRUE);
    CutList->SetName("CutList");
    for (int i = 0; i < fCuts->GetEntriesFast(); i++)
      CutList->Add(((Cut*) fCuts->At(i))->Report());
    pack->AddObject(CutMonitorList);
    pack->AddObject(CutList);
    TList* CutList2 = new TList();
    CutList2->SetOwner(kTRUE);
    CutList2->SetName("FastCutList");
    for (int i = 0; i < fFastCuts->GetEntriesFast(); i++)
      CutList2->Add(((Cut*) fFastCuts->At(i))->Report());
    pack->AddObject(CutList2);
    pack->AddObject(new ParameterInt("Step", fStep, '='));
    return pack;
  }

  CutCollection::CutCollection() :
    fCutMonitors(NULL),
    fCuts(NULL),
    fFastCuts(NULL),
    fPassedSlow(0),
    fFailedSlow(0),
    fPassedFast(0),
    fFailedFast(0),
    fFastCutsNo(0),
    fSlowCutsNo(0),
    fCutMonitorsNo(0),
    fPreviousAddress(NULL),
    fNextAddress(NULL),
    fNextNo(NULL),
    fPreviousNo(NULL),
    fMode(ECutUpdate::kNoUpdate),
    fInit(kFALSE),
    fDummy(kFALSE),
    fCollectionID(0),
    fContainerSize(0),
    fStep(1),
    fCutContainerArr(0),
    fNextUsed(kFALSE),
    fPrevUsed(kFALSE) {
    LOG(WARNING) << "default constructor of CutCollection should never be used !!";
  }

  CutCollection* CutCollection::MakeNewCopy(TObjArray** container) const {
    CutCollection* subcont = new CutCollection(container, this->fContainerSize, this->fMode, this->fCollectionID);
    for (int i = 0; i < fCutMonitors->GetEntriesFast(); i++) {
      subcont->fCutMonitors->Add(((CutMonitor*) fCutMonitors->UncheckedAt(i))->MakeCopy());
    }
    for (int i = 0; i < fCuts->GetEntriesFast(); i++) {
      subcont->fCuts->Add(((Cut*) fCuts->UncheckedAt(i))->MakeCopy());
    }
    for (int i = 0; i < fFastCuts->GetEntriesFast(); i++) {
      subcont->fFastCuts->Add(((Cut*) fFastCuts->UncheckedAt(i))->MakeCopy());
    }
    return subcont;
  }

  void CutCollection::MakeDummyCopy(const CutCollection* copy, Bool_t copy_high_links) {
    if (fDummy == kFALSE) {
      delete fCutMonitors;
      delete fCuts;
      delete fFastCuts;
      fPreviousNo = 0;
      fNextNo     = 0;
      fPrevUsed   = kFALSE;
      fNextUsed   = kFALSE;
      if (fNextNo) {
        delete fNextNo;
        fNextNo = nullptr;
      }
      if (fNextAddress) {
        delete fNextAddress;
        fNextAddress = nullptr;
      }
    }
    if (fPreviousNo) {
      delete fPreviousNo;
      fPreviousNo = nullptr;
    }
    if (fPreviousAddress) {
      delete fPreviousAddress;
      fPreviousAddress = nullptr;
    }

    fCutMonitors   = copy->fCutMonitors;
    fCuts          = copy->fCuts;
    fFastCuts      = copy->fFastCuts;
    fPassedSlow    = copy->fPassedSlow;
    fFailedSlow    = copy->fFailedSlow;
    fPassedFast    = copy->fPassedFast;
    fFailedFast    = copy->fPassedFast;
    fMode          = copy->fMode;
    fInit          = copy->fInit;
    fDummy         = copy->fDummy;
    fCollectionID  = copy->fCollectionID;
    fContainerSize = copy->fContainerSize;
    fFailedFast    = copy->fFailedFast;
    fFailedSlow    = copy->fFailedSlow;
    fPassedFast    = copy->fPassedFast;
    fPassedSlow    = copy->fPassedSlow;
    fCutMonitorsNo = copy->fCutMonitorsNo;
    fFastCutsNo    = copy->fFastCutsNo;
    fSlowCutsNo    = copy->fSlowCutsNo;
    if (copy->fPrevUsed) {
      fPreviousNo      = new Array_1<Int_t>(*copy->fPreviousNo);
      fPreviousAddress = new Array_2<Int_t>(*copy->fPreviousAddress);
    }
    if (copy_high_links) {
      if (copy->fNextUsed) {
        if (fNextNo) delete fNextNo;
        if (fNextAddress) delete fNextAddress;
        fNextNo      = new Array_1<Int_t>(*copy->fNextNo);
        fNextAddress = new Array_2<Int_t>(*copy->fNextAddress);
        fNextUsed    = copy->fNextUsed;
      }
    }
    fPrevUsed = copy->fPrevUsed;
    fDummy    = kTRUE;
  }

  CutCollection::~CutCollection() {
    if (fDummy == kFALSE) {
      if (fCuts) fCuts->Delete();
      if (fCutMonitors) fCutMonitors->Delete();
      delete fCuts;
      delete fFastCuts;
      delete fCutMonitors;
    }
    if (fNextNo) delete fNextNo;
    if (fNextAddress) delete fNextAddress;
    if (fPreviousNo) delete fPreviousNo;
    if (fPreviousAddress) delete fPreviousAddress;
  }

  ULong64_t CutCollection::GetPassed(Option_t* opt) const {
    TString option = opt;
    if (option == "fast") {
      return fPassedFast;
    } else {
      return fPassedSlow;
    }
  }

  ULong64_t CutCollection::GetFailed(Option_t* opt) const {
    TString option = opt;
    if (option == "fast") {
      return fFailedFast;
    } else {
      return fFailedSlow;
    }
  }

  void CutCollection::MarkAsDummy() { fDummy = kTRUE; }

  ECutUpdate CutCollection::GetUpdateFromName(TString cutname) const {
    if (cutname.BeginsWith("EventComplexCut")) return ECutUpdate::kEventUpdate;
    if (cutname.BeginsWith("TrackComplexCut")) return ECutUpdate::kTrackUpdate;
    if (cutname.BeginsWith("TwoTrackComplexCut")) return ECutUpdate::kTwoTrackUpdate;
    if (cutname.BeginsWith("EventRealCut")) return ECutUpdate::kEventUpdate;
    if (cutname.BeginsWith("TrackRealCut")) return ECutUpdate::kTrackUpdate;
    if (cutname.BeginsWith("TwoTrackRealCut")) return ECutUpdate::kTwoTrackUpdate;
    if (cutname.BeginsWith("EventImaginaryCut")) return ECutUpdate::kEventUpdate;
    if (cutname.BeginsWith("TrackImaginaryCut")) return ECutUpdate::kTrackUpdate;
    if (cutname.BeginsWith("TwoTrackImaginaryCut")) return ECutUpdate::kTwoTrackUpdate;
    TClass* cl = TClass::GetClass(cutname, kFALSE, kTRUE);
    if (cl->InheritsFrom("EventCut")) {
      return ECutUpdate::kEventUpdate;
    } else if (cl->InheritsFrom("TrackCut")) {
      return ECutUpdate::kTrackUpdate;
    } else {
      return ECutUpdate::kTwoTrackUpdate;
    }
  }
}  // namespace Hal
