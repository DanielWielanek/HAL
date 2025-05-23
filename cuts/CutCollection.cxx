/*
 * CutCollection.cxx
 *
 *  Created on: 06-08-2013
 *      Author: Daniel Wielanek
 *      E-mail: daniel.wielanek@gmail.com
 *      Warsaw University of Technology, Faculty of Physics
 */

#include "CutCollection.h"

#include "Cout.h"
#include "CutMonitor.h"
#include "CutMonitorX.h"
#include "CutMonitorXY.h"
#include "CutMonitorXYZ.h"
#include "Event.h"
#include "EventCut.h"
#include "Package.h"
#include "Parameter.h"
#include "StdString.h"
#include "Track.h"
#include "TrackCut.h"
#include "TwoTrack.h"
#include "TwoTrackCut.h"

#include <TClass.h>
#include <TCollection.h>
#include <TList.h>


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
    fInit(kFALSE),
    fDummy(kFALSE),
    fCollectionID(0),
    fContainerSize(cont_size),
    fStep(1),
    fCutContainerArr(NULL)  // this is needed to link cut monitors in case of cloning
  {
    fCutContainerArr = container;
    fCutMonitors     = new TObjArray();
    fCuts            = new TObjArray();
    fFastCuts        = new TObjArray();
    fInit            = kFALSE;
    fMode            = mode;
    fCollectionID    = collectionNo;
  }

  void CutCollection::AddCut(Cut* cut, Hal::CutOptions opts) {
    Bool_t fast       = opts.Fast();
    Bool_t keepDouble = opts.KeepDouble();
    if (fDummy) return;
    if (cut == NULL) {
      Cout::PrintInfo("Empty cut", EInfo::kLowWarning);
      return;
    }
    if (!fast) {  // add normal cut
      for (int i = 0; i < fCuts->GetEntriesFast(); i++) {
        if (cut->CutName() == ((Cut*) fCuts->UncheckedAt(i))->CutName()) {
          Cout::PrintInfo(
            Form("Cut used %s in collection no %i, cut will be removed", cut->CutName().Data(), cut->GetCollectionID()),
            EInfo::kLowWarning);
          return;
        }
      }
      for (int i = 0; i < fFastCuts->GetEntriesFast(); i++) {
        if (cut->CutName() == ((Cut*) fFastCuts->UncheckedAt(i))->CutName()) {
          if (!keepDouble) {
            Cout::PrintInfo(Form("Cut used %s in collection no %i in fast cuts group,  cut will "
                                 "be "
                                 "removed",
                                 cut->ClassName(),
                                 cut->GetCollectionID()),
                            EInfo::kLowWarning);
            return;
          }
        }
      }
      fCuts->AddLast(cut);
    } else {
      for (int i = 0; i < fFastCuts->GetEntriesFast(); i++) {
        if (cut->CutName() == ((Cut*) fFastCuts->UncheckedAt(i))->CutName()) {
          Cout::PrintInfo(Form("Cut used %s in collection no %i in as fast, cut will be "
                               "removed",
                               cut->CutName().Data(),
                               cut->GetCollectionID()),
                          EInfo::kLowWarning);
          return;
        }
      }
      if (!keepDouble) {
        for (int i = 0; i < fCuts->GetEntriesFast(); i++) {
          if (cut->CutName() == ((Cut*) fCuts->UncheckedAt(i))->CutName()) {
            Cout::PrintInfo(
              Form("Cut used %s in collection no %i but cut will be removed", cut->CutName().Data(), cut->GetCollectionID()),
              EInfo::kLowWarning);
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
      Cout::PrintInfo("Empty monitor", EInfo::kError);
      return;
    }

    for (int i = 0; i < fCutMonitors->GetEntriesFast(); i++) {
      if (monitor == (CutMonitor*) fCutMonitors->UncheckedAt(i)) {
        Cout::PrintInfo("Monitor cut used", EInfo::kLowWarning);
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
      fNext.SafeInit();
      fPrev.SafeInit();
      return;
    }
    if (fInit) {
      Cout::PrintInfo("CutCollection has been initialized", EInfo::kError);
    } else {
      for (int i = 0; i < fCuts->GetEntriesFast(); i++) {
        Bool_t ok = ((Cut*) fCuts->UncheckedAt(i))->Init(task_id);
        if (!ok) {
#ifdef HAL_DEBUG
          Cout::PrintInfo(
            Form("Failed to init %s in cut collection %i", ((Cut*) fCuts->UncheckedAt(i))->CutName().Data(), GetCollectionID()),
            EInfo::kDebugInfo);
#endif
          fCuts->RemoveAt(i);
          fCuts->Compress();
          i--;
        } else {
#ifdef HAL_DEBUG
          Cout::PrintInfo(Form("Succesfully  inited %s in cut collection %i",
                               ((Cut*) fCuts->UncheckedAt(i))->CutName().Data(),
                               GetCollectionID()),
                          EInfo::kDebugInfo);
#endif
        }
      }
      for (int i = 0; i < fFastCuts->GetEntriesFast(); i++) {
        Bool_t ok = ((Cut*) fFastCuts->UncheckedAt(i))->Init(task_id);
        if (!ok) {
#ifdef HAL_DEBUG
          Cout::PrintInfo(Form("Failed to init %s in cut collection %i",
                               ((Cut*) fFastCuts->UncheckedAt(i))->CutName().Data(),
                               GetCollectionID()),
                          EInfo::kDebugInfo);
#endif
          fFastCuts->RemoveAt(i);
          fFastCuts->Compress();
          i--;
        } else {
#ifdef HAL_DEBUG
          Cout::PrintInfo(Form("Succesfully inited %s in cut collection %i",
                               ((Cut*) fFastCuts->UncheckedAt(i))->CutName().Data(),
                               GetCollectionID()),
                          EInfo::kDebugInfo);
#endif
        }
      }
#ifdef HAL_DEBUG
      Cout::PrintInfo("Initializing cut monitors", EInfo::kDebugInfo);
#endif
      AdvancedMonitorInitialization(task_id);
      fInit = kTRUE;
      if (fNext.GetSize() == 0) {
        fNext.SafeInit();
      } else {
        fNext.MakeUsed();
      }
      if (fPrev.GetSize() == 0) {
        fPrev.SafeInit();
      } else {
        fPrev.MakeUsed();
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
    if (backround) {
      fPrevBckg.AddAddr(value);
    } else {
      fPrev.AddAddr(value);
    }
  }

  void CutCollection::AddNextAddr(Int_t value, Bool_t backround) {
    if (fDummy) return;
    if (backround) {
      fNextBckg.AddAddr(value);
    } else {
      fNext.AddAddr(value);
    }
  }

  Cut* CutCollection::FindCut(TString name) const {
#ifdef HAL_DEBUG
    Cout::PrintInfo(Form("Looking for %s in subcontainer %i", name.Data(), GetCollectionID()), EInfo::kDebugInfo);
#endif
    for (int i = 0; i < fCuts->GetEntriesFast(); i++) {
      TString name2 = ((Cut*) fCuts->UncheckedAt(i))->CutName();
      if (name2 == name) {
        return (Cut*) fCuts->UncheckedAt(i);
      } else {
        // Hal::Cout::PrintInfo(Form("try %s %s ", name.Data(), name2.Data()), EInfo::kDebugInfo);
      }
    }  // cut not found in standard cuts, looking in complex cuts
    return NULL;
  }

  CutCollection* CutCollection::Replicate(Int_t new_collection) const {
    if (fInit == kTRUE) {
      Cout::PrintInfo("Cant replicate initialized CutCollection - this lead "
                      "to mess with cut monitors !",
                      EInfo::kLowWarning);
    }
    CutCollection* clone = new CutCollection(fCutContainerArr, fContainerSize, fMode, new_collection);
    clone->fStep         = this->fStep;
    for (int i = 0; i < this->fCuts->GetEntriesFast(); i++) {
      Hal::CutOptions opts("double");
      Cut* cut = ((Cut*) fCuts->UncheckedAt(i))->MakeCopy();
      cut->SetCollectionID(new_collection);
      clone->AddCut(cut, opts);
    }
    for (int i = 0; i < fFastCuts->GetEntriesFast(); i++) {
      Cut* cut = ((Cut*) fFastCuts->UncheckedAt(i))->MakeCopy();
      cut->SetCollectionID(new_collection);
      clone->fFastCuts->AddLast(cut);
    }
    for (int i = 0; i < this->fCutMonitors->GetEntriesFast(); i++) {
      TString orig = fCutMonitors->UncheckedAt(i)->ClassName();
      if (orig == "Hal::CutMonitorX") {
        CutMonitorX* cutmon = new CutMonitorX(*(CutMonitorX*) fCutMonitors->UncheckedAt(i));
        clone->AddCutMonitor(cutmon);
      } else if (orig == "Hal::CutMonitorXY") {
        CutMonitorXY* cutmon = new CutMonitorXY(*(CutMonitorXY*) fCutMonitors->UncheckedAt(i));
        clone->AddCutMonitor(cutmon);
      } else if (orig == "Hal::CutMonitorXYZ") {
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
    if (fPrev.IsUsed()) prev_size = fPrev.GetSize();
    for (int i = 0; i < oryginal_monitors_no; i++) {
      CutMonitor* cutmon = (CutMonitor*) fCutMonitors->UncheckedAt(i);
      if (cutmon->ObjMonitor()) {
        Bool_t init = cutmon->Init(task_id);
        if (init == kFALSE) {
          Cout::PrintInfo(Form("Problem with initalization  [%s]", cutmon->ClassName()), EInfo::kLowWarning);
          fCutMonitors->RemoveAt(i);
        }
        continue;
      }
      TString monName = cutmon->ClassName();
      if (monName == "Hal::CutMonitorX") {
        TString cut = cutmon->GetCutName(0);
        Cut* newCut = this->FindCut(cut);
        if (newCut) {
          if (newCut->GetCutSize() <= cutmon->GetCutParameter(0)) {
            Cout::PrintInfo(Form("Problem with initalization CutMonitorXY [%s]", newCut->ClassName()), EInfo::kLowWarning);
            Cout::PrintInfo(
              Form("Cut %s has only %i not %i parameters", newCut->ClassName(), newCut->GetCutSize(), cutmon->GetCutParameter(0)),
              EInfo::kLowWarning);
            fCutMonitors->RemoveAt(i);
          } else {
            cutmon->AddForcedCut(newCut, 0);
          }
        } else {
          Cout::PrintInfo(Form("Hal::CutMonitorX [%s] not found!", cut.Data()), EInfo::kError);
          fCutMonitors->RemoveAt(i);
        }
      } else if (monName == "Hal::CutMonitorXY") {
        TString cut1_name = cutmon->GetCutName(0);
        TString cut2_name = cutmon->GetCutName(1);
        TObjArray* Ncut1  = LocateCuts(cut1_name);
        TObjArray* Ncut2  = LocateCuts(cut2_name);

        if (Ncut1->GetEntries() == 0 || Ncut2->GetEntries() == 0) {
          Cout::PrintInfo(Form("Problem with initalization CutMonitorXY [%s] vs [%s]", cut1_name.Data(), cut2_name.Data()),
                          EInfo::kLowWarning);
          if (Ncut1->GetEntries() == 0) { Cout::PrintInfo(Form("Cut %s not found", cut1_name.Data()), EInfo::kLowWarning); }
          if (Ncut2->GetEntries() == 0) { Cout::PrintInfo(Form("Cut %s not found", cut2_name.Data()), EInfo::kLowWarning); }
          fCutMonitors->RemoveAt(i);
        } else if (Ncut1->GetEntriesFast() > 1 || Ncut2->GetEntriesFast() > 1 || prev_size > 1) {
          Cout::PrintInfo(
            Form("There is too many links or cuts cut monitor %s %s will be removed", cut1_name.Data(), cut2_name.Data()),
            EInfo::kError);
          fCutMonitors->RemoveAt(i);
        } else {
          Cut* cut1 = (Cut*) Ncut1->UncheckedAt(0);
          Cut* cut2 = (Cut*) Ncut2->UncheckedAt(0);
          if (cut1->GetCutSize() <= cutmon->GetCutParameter(0)) {
            Cout::PrintInfo(Form("Problem with initalization CutMonitorXY [%s] vs [%s]", cut1_name.Data(), cut2_name.Data()),
                            EInfo::kLowWarning);
            Cout::PrintInfo(
              Form("Cut %s has only %i not %i parameters", cut1->ClassName(), cut1->GetCutSize(), cutmon->GetCutParameter(0)),
              EInfo::kLowWarning);
            fCutMonitors->RemoveAt(i);
          } else if (cut2->GetCutSize() <= cutmon->GetCutParameter(1)) {
            Cout::PrintInfo(Form("Problem with initalization CutMonitorXY [%s] vs [%s]", cut1_name.Data(), cut2_name.Data()),
                            EInfo::kLowWarning);
            Cout::PrintInfo(
              Form("Cut %s has only %i not %i parameters", cut1->ClassName(), cut1->GetCutSize(), cutmon->GetCutParameter(1)),
              EInfo::kLowWarning);
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
      } else if (monName == "Hal::CutMonitorXYZ") {
        TString cut1_name = cutmon->GetCutName(0);
        TString cut2_name = cutmon->GetCutName(1);
        TString cut3_name = cutmon->GetCutName(2);
        TObjArray* Ncut1  = LocateCuts(cut1_name);
        TObjArray* Ncut2  = LocateCuts(cut2_name);
        TObjArray* Ncut3  = LocateCuts(cut3_name);
        if (Ncut1->GetEntries() == 0 || Ncut2->GetEntries() == 0 || Ncut3->GetEntries() == 0) {
          Cout::PrintInfo(
            Form(" Problem with Hal::CutMonitorXYZ [%s] vs [%s] vs [%s]", cut1_name.Data(), cut2_name.Data(), cut3_name.Data()),
            EInfo::kLowWarning);
          if (Ncut1->GetEntries() == 0) Cout::PrintInfo(Form("Cut %s not found", cut1_name.Data()), EInfo::kLowWarning);
          if (Ncut2->GetEntries() == 0) Cout::PrintInfo(Form("Cut %s not found", cut2_name.Data()), EInfo::kLowWarning);
          if (Ncut3->GetEntries() == 0) Cout::PrintInfo(Form("Cut %s not found", cut3_name.Data()), EInfo::kLowWarning);
          fCutMonitors->RemoveAt(i);
        } else if (Ncut1->GetEntries() > 1 || Ncut2->GetEntries() > 1 || Ncut3->GetEntriesFast() > 1 || prev_size > 1) {
          Cout::PrintInfo("There is too many links or cuts monitor %s %s will be removed", EInfo::kError);
          fCutMonitors->RemoveAt(i);
        } else {
          Cut* cut1      = (Cut*) Ncut1->UncheckedAt(0);
          Cut* cut2      = (Cut*) Ncut2->UncheckedAt(0);
          Cut* cut3      = (Cut*) Ncut3->UncheckedAt(0);
          Int_t own_cuts = 0;
          ECutUpdate upd = fMode;
          if (fMode == ECutUpdate::kTwoTrackBackground) { upd = ECutUpdate::kTwoTrack; }
          if (cut1->GetUpdateRatio() == upd) own_cuts = 1;
          if (cut2->GetUpdateRatio() == upd) own_cuts += 2;
          if (cut3->GetUpdateRatio() == upd) own_cuts += 4;
          ;
          switch (own_cuts) {
            case 0: {
              Cout::PrintInfo("Hal::CutMonitorXYZ has no cuts compatible with "
                              "container that own it",
                              EInfo::kLowWarning);
              fCutMonitors->RemoveAt(i);
            } break;
            case 1: {  // 1 local
              cutmon->AddForcedCut(cut1, 0);
              if (CheckIfComptatiblie(cut2, cut3)) {
                cutmon->AddForcedCut(cut2, 1);
                cutmon->AddForcedCut(cut3, 2);
              } else {
                Cout::PrintInfo("Couldn't find compatible cuts for monitoring", EInfo::kLowWarning);
                fCutMonitors->RemoveAt(i);
              }
            } break;
            case 2:  // 2 local
              cutmon->AddForcedCut(cut2, 1);
              if (CheckIfComptatiblie(cut1, cut3)) {
                cutmon->AddForcedCut(cut1, 0);
                cutmon->AddForcedCut(cut3, 2);
              } else {
                Cout::PrintInfo("Couldn't find compatible cuts for monitoring", EInfo::kLowWarning);
                fCutMonitors->RemoveAt(i);
              }
              break;
            case 4:  // 3 local
              cutmon->AddForcedCut(cut3, 2);
              if (CheckIfComptatiblie(cut1, cut2)) {
                cutmon->AddForcedCut(cut1, 0);
                cutmon->AddForcedCut(cut2, 1);
              } else {
                Cout::PrintInfo("Couldn't find compatible cuts for monitoring", EInfo::kLowWarning);
                fCutMonitors->RemoveAt(i);
              }
              break;
            default: {
              if (cut1->GetCutSize() <= cutmon->GetCutParameter(0)) {
                Cout::PrintInfo(Form("Problem with initalization Hal::CutMonitorXY [%s] vs [%s] vs "
                                     "[%s]",
                                     cut1_name.Data(),
                                     cut2_name.Data(),
                                     cut3_name.Data()),
                                EInfo::kLowWarning);
                Cout::PrintInfo(
                  Form("Cut %s has only %i not %i parameters", cut1->ClassName(), cut1->GetCutSize(), cutmon->GetCutParameter(0)),
                  EInfo::kLowWarning);
                fCutMonitors->RemoveAt(i);
              } else if (cut2->GetCutSize() <= cutmon->GetCutParameter(1)) {
                Cout::PrintInfo(Form("Problem with initalization Hal::CutMonitorXY [%s] vs [%s] vs "
                                     "[%s]",
                                     cut1_name.Data(),
                                     cut2_name.Data(),
                                     cut3_name.Data()),
                                EInfo::kLowWarning);
                Cout::PrintInfo(
                  Form("Cut %s has only %i not %i parameters", cut2->ClassName(), cut2->GetCutSize(), cutmon->GetCutParameter(1)),
                  EInfo::kLowWarning);
                fCutMonitors->RemoveAt(i);
              } else if (cut3->GetCutSize() <= cutmon->GetCutParameter(2)) {
                Cout::PrintInfo(Form("Problem with initalization Hal::CutMonitorXY [%s] vs [%s] vs "
                                     "[%s]",
                                     cut1_name.Data(),
                                     cut2_name.Data(),
                                     cut3_name.Data()),
                                EInfo::kLowWarning);
                Cout::PrintInfo(
                  Form("Cut %s has only %i not %i parameters", cut3->ClassName(), cut3->GetCutSize(), cutmon->GetCutParameter(2)),
                  EInfo::kLowWarning);
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
        Cout::PrintInfo(Form("Problems with class name %s of cuts this is an critial error", monName.Data()),
                        EInfo::kCriticalError);
      }
    }

    auto sameMon = [](CutMonitor* a, CutMonitor* b) {
      int n = a->GetAxisNo();
      if (n != b->GetAxisNo()) return false;
      int bad = 0;
      for (int no = 0; no < n; no++) {
        if (a->GetCutLink(no) == b->GetCutLink(no))
          if (a->GetCutParameter(no) == b->GetCutParameter(no)) ++bad;
      }
      if (bad == n) return true;
      return false;
    };

    /** TODO fix for field monitors
        for (int i = 0; i < fCutMonitors->GetEntries(); i++) {
          auto mon1 = (CutMonitor*) fCutMonitors->UncheckedAt(i);
          for (int j = i + 1; j < fCutMonitors->GetEntries(); j++) {
            auto mon2 = (CutMonitor*) fCutMonitors->UncheckedAt(j);
            if (mon1->GetAxisNo() == mon2->GetAxisNo()) {
              bool bad = sameMon(mon1, mon2);
              if (bad) {
                TString info = Form("CutCollection::AdvancedMonitorInitialization Duplicate cut found, it will be removed
       (col=%i)", GetCollectionID());

                Cout::PrintInfo(info, EInfo::kWarning);
                fCutMonitors->RemoveAt(j--);
              }
            }
          }
        }**/
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
    if (this->fMode == ECutUpdate::kEvent) { return obj; }
    if (this->fMode == ECutUpdate::kTrack) {
      for (int i = 0; i < this->GetPrevNo(); i++) {
        CutCollection* subcont = (CutCollection*) (GetCutContainerArray(ECutUpdate::kEvent)->At(this->GetPrevAddr(i)));
        if (subcont->FindCut(cut)) { obj->AddLast(subcont->FindCut(cut)); }
      }
      return obj;
    }
    if (this->fMode == ECutUpdate::kTwoTrack) {
      // look over track containers
      for (int i = 0; i < this->GetPrevNo(); i++) {
        CutCollection* subcont = (CutCollection*) (GetCutContainerArray(ECutUpdate::kTrack)->At(this->GetPrevAddr(i)));
        if (subcont->FindCut(cut)) { obj->AddLast(subcont->FindCut(cut)); }
      }
      // still not found - we need go deeper !! to event cuts
      for (int i = 0; i < this->GetPrevNo(); i++) {
        CutCollection* subcont = (CutCollection*) (GetCutContainerArray(ECutUpdate::kTrack)->At(this->GetPrevAddr(i)));
        for (int j = 0; j < subcont->GetPrevNo(); j++) {
          CutCollection* subsubcont = (CutCollection*) (GetCutContainerArray(ECutUpdate::kTwoTrack)->At(this->GetPrevAddr(j)));
          if (subsubcont->FindCut(cut)) { obj->AddLast(subsubcont->FindCut(cut)); }
        }
      }
      return obj;
    }
    if (this->fMode == ECutUpdate::kTwoTrackBackground) {
      // look over track containers
      for (int i = 0; i < this->GetPrevNo(); i++) {
        CutCollection* subcont = (CutCollection*) (GetCutContainerArray(ECutUpdate::kTrack)->At(this->GetPrevAddr(i)));
        if (subcont->FindCut(cut)) { obj->AddLast(subcont->FindCut(cut)); }
      }
      // still not found - we need go deeper !! to event cuts
      for (int i = 0; i < this->GetPrevNo(); i++) {
        CutCollection* subcont = (CutCollection*) (GetCutContainerArray(ECutUpdate::kTrack)->At(this->GetPrevAddr(i)));
        for (int j = 0; j < subcont->GetPrevNo(); j++) {
          CutCollection* subsubcont =
            (CutCollection*) (GetCutContainerArray(ECutUpdate::kTwoTrackBackground)->At(this->GetPrevAddr(j)));
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
    ECutUpdate cut_up = ECutUpdate::kNo;
    cut_up            = GetUpdateFromName(classname);
    if (fMode == cut_up) {
      Cut* cut = FindCut(classname);
      arr->AddLast(cut);
      return arr;
    }
    if (fMode == ECutUpdate::kTwoTrackBackground && cut_up == ECutUpdate::kTwoTrack) {
      Cut* cut = FindCut(classname);
      arr->AddLast(cut);
      return arr;
    } else if (fPrev.IsUsed()) {
#ifdef HAL_DEBUG
      Cout::PrintInfo("Looking for cuts in lower collections", EInfo::kDebugInfo);
#endif
      arr = LocateInLowerCollections(classname);
      return arr;
    } else {
#ifdef HAL_DEBUG
      Cout::PrintInfo("Cut not found cut and no lower collections present", EInfo::kDebugInfo);
#endif
      return arr;
    }
  }

  void CutCollection::PrintInfo() const {
    Cout::InStars(Form("Subcontainer trig %i", fCollectionID));
    TString mode;
    switch (fMode) {
      case (ECutUpdate::kEvent): {
        mode = "Event Upd";
      } break;
      case (ECutUpdate::kTrack): {
        mode = "Track upd";
      } break;
      case (ECutUpdate::kTwoTrack): {
        mode = "TTrack mod";
      } break;
      case (ECutUpdate::kTwoTrackBackground): {
        mode = "TTRack mixed";
      } break;
      default: Cout::PrintInfo("Unknown update mode inf CutCollection", EInfo::kLowWarning); break;
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
      Cout::Database({cutmon->GetCutName(0)});
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
      case (ECutUpdate::kEvent): {
        pack->AddObject(new ParameterString("UpdateMode", "Event"));
      } break;
      case (ECutUpdate::kTrack): {
        pack->AddObject(new ParameterString("UpdateMode", "Track"));
      } break;
      case (ECutUpdate::kTwoTrack): {
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

    auto MakeList = [](TString name, const CutCollectionLinks& col) {
      TList* list = new TList();
      list->SetOwner(kTRUE);
      list->SetName(name);
      for (int i = 0; i < col.GetSize(); i++) {
        list->AddAt(new ParameterInt(Form("%i", i), col.GetAddr(i)), i);
      }
      return list;
    };
    int used = 0;
    if (fNext.IsUsed()) used++;
    if (fNextBckg.IsUsed()) used++;
    pack->AddObject(new ParameterInt("NextLayers", used));
    if (fNext.IsUsed()) pack->AddObject(MakeList("NextObj_0", fNext));
    if (fNextBckg.IsUsed()) pack->AddObject(MakeList("NextObj_1", fNextBckg));

    used = 0;
    if (fPrev.IsUsed()) used++;
    if (fPrevBckg.IsUsed()) used++;

    pack->AddObject(new ParameterInt("PreviousLayers", used));
    if (fPrev.IsUsed()) pack->AddObject(MakeList("PrevObj_0", fPrev));
    if (fPrevBckg.IsUsed()) pack->AddObject(MakeList("PrevObj_1", fPrevBckg));

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
    fMode(ECutUpdate::kNo),
    fInit(kFALSE),
    fDummy(kFALSE),
    fCollectionID(0),
    fContainerSize(0),
    fStep(1),
    fCutContainerArr(0) {
    Cout::PrintInfo("default constructor of CutCollection should never be used !!", EInfo::kLowWarning);
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
    if (copy->fPrev.IsUsed()) fPrev = copy->fPrev;
    if (copy->fPrevBckg.IsUsed()) fPrevBckg = copy->fPrevBckg;
    if (copy_high_links) {
      if (copy->fNext.IsUsed()) fNext = copy->fNext;
      if (copy->fNextBckg.IsUsed()) fNextBckg = copy->fNextBckg;
    }
    fDummy = kTRUE;
  }

  CutCollection::~CutCollection() {
    if (fDummy == kFALSE) {
      if (fCuts) fCuts->Delete();
      if (fCutMonitors) fCutMonitors->Delete();
      delete fCuts;
      delete fFastCuts;
      delete fCutMonitors;
    }
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
    if (cutname.BeginsWith("Hal::EventComplexCut")) return ECutUpdate::kEvent;
    if (cutname.BeginsWith("Hal::TrackComplexCut")) return ECutUpdate::kTrack;
    if (cutname.BeginsWith("Hal::TwoTrackComplexCut")) return ECutUpdate::kTwoTrack;
    if (cutname.BeginsWith("Hal::EventRealCut")) return ECutUpdate::kEvent;
    if (cutname.BeginsWith("Hal::TrackRealCut")) return ECutUpdate::kTrack;
    if (cutname.BeginsWith("Hal::TwoTrackRealCut")) return ECutUpdate::kTwoTrack;
    if (cutname.BeginsWith("Hal::EventImaginaryCut")) return ECutUpdate::kEvent;
    if (cutname.BeginsWith("Hal::TrackImaginaryCut")) return ECutUpdate::kTrack;
    if (cutname.BeginsWith("Hal::TwoTrackImaginaryCut")) return ECutUpdate::kTwoTrack;
    TClass* cl = TClass::GetClass(cutname, kFALSE, kTRUE);
    if (cl->InheritsFrom("Hal::EventCut")) {
      return ECutUpdate::kEvent;
    } else if (cl->InheritsFrom("Hal::TrackCut")) {
      return ECutUpdate::kTrack;
    } else {
      return ECutUpdate::kTwoTrack;
    }
  }

  void CutCollectionLinks::SafeInit() {
    if (fLinks.size() == 0) fLinks.push_back(0);
  }

  void CutCollectionLinks::AddAddr(Int_t addr) {
    if (addr < 0) return;
    fLinks.push_back(addr);
    fUsed = kTRUE;
  }

}  // namespace Hal
