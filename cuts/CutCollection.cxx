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
    fContainerSize(cont_size),
    fCutContainerArr(container)  // this is needed to link cut monitors in case of cloning
  {
    fCutMonitors  = new TObjArray();
    fCuts         = new TObjArray();
    fFastCuts     = new TObjArray();
    fMode         = mode;
    fCollectionID = collectionNo;
  }

  void CutCollection::AddCut(Cut* cut, Hal::CutOptions opts) {
    Bool_t fast       = opts.IsFast();
    Bool_t keepDouble = opts.IsKeepDouble();
    if (fDummy) return;
    if (cut == NULL) {
      Cout::PrintInfo("CutCollection: Empty cut", EInfo::kLowWarning);
      return;
    }

    auto compare = [](Cut* localcut, TObjArray* array) {
      for (int i = 0; i < array->GetEntriesFast(); i++) {
        if (localcut->CutName() == ((Cut*) array->UncheckedAt(i))->CutName()) return kTRUE;
      }
      return kFALSE;
    };
    Bool_t sameCut     = compare(cut, fCuts);
    Bool_t sameFastCut = compare(cut, fFastCuts);

    if (!fast) {  // add normal cut
      if (sameCut) {
        Cout::PrintInfo(Form("CutCollection: Cut used %s in collection no %i, cut will be removed",
                             cut->CutName().Data(),
                             cut->GetCollectionID()),
                        EInfo::kLowWarning);
        return;
      }
      if (sameFastCut) {
        if (!keepDouble) {
          Cout::PrintInfo(Form("CutCollection: Cut used %s in collection no %i in fast cuts group,  cut will "
                               "be "
                               "removed",
                               cut->ClassName(),
                               cut->GetCollectionID()),
                          EInfo::kLowWarning);
          return;
        }
      }
      cut->SetCollectionID(fCollectionID);
      fCuts->AddLast(cut);
    } else {
      if (sameFastCut) {
        Cout::PrintInfo(Form("CutCollection: Cut used %s in collection no %i in as fast, cut will be "
                             "removed",
                             cut->CutName().Data(),
                             cut->GetCollectionID()),
                        EInfo::kLowWarning);
        return;
      }
      if (!keepDouble) {
        if (sameCut) {
          Cout::PrintInfo(Form("CutCollection: Cut used %s in collection no %i but cut will be removed",
                               cut->CutName().Data(),
                               cut->GetCollectionID()),
                          EInfo::kLowWarning);
          return;
        }
      }
      cut->SetCollectionID(fCollectionID);
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
      Cout::PrintInfo("CutCollection: Empty monitor", EInfo::kError);
      return;
    }

    for (int i = 0; i < fCutMonitors->GetEntriesFast(); i++) {
      if (monitor == (CutMonitor*) fCutMonitors->UncheckedAt(i)) {
        Cout::PrintInfo("CutCollection: Monitor cut used", EInfo::kLowWarning);
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
      Cout::PrintInfo("CutCollection: CutCollection has been initialized", EInfo::kError);
    } else {
      for (int i = 0; i < fCuts->GetEntriesFast(); i++) {
        Bool_t ok = ((Cut*) fCuts->UncheckedAt(i))->Init(task_id);
        if (!ok) {
          Cout::PrintInfo(Form("CutCollection: Failed to init %s in cut collection %i",
                               ((Cut*) fCuts->UncheckedAt(i))->CutName().Data(),
                               GetCollectionID()),
                          EInfo::kDebugInfo);
          fCuts->RemoveAt(i);
          fCuts->Compress();
          i--;
        } else {
          Cout::PrintInfo(Form("CutCollection: Successfully  inited %s in cut collection %i",
                               ((Cut*) fCuts->UncheckedAt(i))->CutName().Data(),
                               GetCollectionID()),
                          EInfo::kDebugInfo);
        }
      }
      for (int i = 0; i < fFastCuts->GetEntriesFast(); i++) {
        Bool_t ok = ((Cut*) fFastCuts->UncheckedAt(i))->Init(task_id);
        if (!ok) {
          Cout::PrintInfo(Form("Fast CutCollection: Failed to init %s in cut collection %i",
                               ((Cut*) fFastCuts->UncheckedAt(i))->CutName().Data(),
                               GetCollectionID()),
                          EInfo::kDebugInfo);
          fFastCuts->RemoveAt(i);
          fFastCuts->Compress();
          i--;
        } else {
          Cout::PrintInfo(Form("Fast CutCollection: Successfully inited %s in cut collection %i",
                               ((Cut*) fFastCuts->UncheckedAt(i))->CutName().Data(),
                               GetCollectionID()),
                          EInfo::kDebugInfo);
        }
      }
      Cout::PrintInfo("CutCollection: Initializing cut monitors", EInfo::kDebugInfo);
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
    Cout::PrintInfo(Form("CutCollection: Looking for %s in subcontainer %i", name.Data(), GetCollectionID()), EInfo::kDebugInfo);
#endif
    for (int i = 0; i < fCuts->GetEntriesFast(); i++) {
      TString name2 = ((Cut*) fCuts->UncheckedAt(i))->CutName();
      if (name2 == name) { return (Cut*) fCuts->UncheckedAt(i); }
    }  // cut not found in standard cuts, looking in complex cuts
    return NULL;
  }

  CutCollection* CutCollection::Replicate(Int_t new_collection) const {
    if (fInit == kTRUE) {
      Cout::PrintInfo("CutCollection: Cant replicate initialized CutCollection - this lead "
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
        cutmon->SetCollectionID(fCollectionID);
        Bool_t init = cutmon->Init(task_id);
        if (init == kFALSE) {
          Cout::PrintInfo(Form("CutCollection: Problem with initalization obj monitor [%s]", cutmon->ClassName()),
                          EInfo::kLowWarning);
          fCutMonitors->RemoveAt(i);
        }
        continue;
      }
      TString monName = cutmon->ClassName();
      if (monName == "Hal::CutMonitorX") {
        auto cuts = BasicCheckMonitor(cutmon);
        if (cuts.size() != 1) {
          fCutMonitors->RemoveAt(i);
        } else {
          cutmon->AddForcedCut(cuts[0], 0);
        }
      } else if (monName == "Hal::CutMonitorXY") {
        auto cuts = BasicCheckMonitor(cutmon);
        if (cuts.size() != 2 || prev_size > 1) {
          fCutMonitors->RemoveAt(i);
        } else {
          cutmon->AddForcedCut(cuts[0], 0);
          cutmon->AddForcedCut(cuts[1], 1);
        }
      } else if (monName == "Hal::CutMonitorXYZ") {
        auto cuts = BasicCheckMonitor(cutmon);
        if (cuts.size() != 3 || prev_size > 1) {
          fCutMonitors->RemoveAt(i);
        } else {
          cutmon->AddForcedCut(cuts[0], 0);
          cutmon->AddForcedCut(cuts[1], 1);
          cutmon->AddForcedCut(cuts[2], 2);
        }
      } else {
        Cout::PrintInfo(Form("CutCollection: Problems with class name %s of cuts this is an critial error", monName.Data()),
                        EInfo::kCriticalError);
      }
    }

    std::vector<int> toRemove;
    for (int i = 0; i < fCutMonitors->GetEntries(); i++) {
      auto a = (CutMonitor*) fCutMonitors->UncheckedAt(i);
      if (a == nullptr) continue;
      for (int j = i + 1; j < fCutMonitors->GetEntries(); j++) {
        auto b = (CutMonitor*) fCutMonitors->UncheckedAt(j);
        if (b == nullptr) continue;
        if (a->AreSimilar(*b)) { fCutMonitors->RemoveAt(j); }
      }
    }

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

  std::vector<Cut*> CutCollection::BasicCheckMonitor(CutMonitor* mon) {
    std::vector<Cut*> res;
    TString monitors[]  = {"CutMonitorX", "CutMonitorXY", "CutMonitorXYZ"};
    TString monitorName = "";
    Int_t size          = mon->GetAxisNo();
    for (int i = 0; i < size; i++) {
      monitorName = monitorName + Form("[%s] ", mon->GetCutName(i).Data());
    }
    for (int i = 0; i < size; i++) {
      auto cut = FindCut(mon->GetCutName(i));
      if (cut == nullptr) {
        Cout::PrintInfo(Form("CutCollection: %s cannot find cut %s", monitorName.Data(), mon->GetCutName(i).Data()),
                        EInfo::kLowWarning);
        return res;
      }
      if (cut->GetCutSize() <= mon->GetCutParameter(i)) {
        Cout::PrintInfo(Form("CutCollection: %s too large parameter for axis=%i [requested par no = %i]",
                             monitorName.Data(),
                             i,
                             mon->GetCutParameter(i)),
                        EInfo::kLowWarning);
        return res;
      }
      res.push_back(cut);
    }
    return res;
  }

  Int_t CutCollection::Compare(const TObject* obj) const {
    if (obj->ClassName() != this->ClassName()) return 0;
    Int_t trigg = ((CutCollection*) obj)->GetCollectionID();
    if (trigg == fCollectionID) return 0;
    if (trigg > fCollectionID) return -1;
    return 1;
  }

  void CutCollection::PrintInfo() const {
    Cout::InStars(Form(" Subcontainer trig %i", fCollectionID));
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
      default: Cout::PrintInfo("CutCollection: Unknown update mode inf CutCollection", EInfo::kLowWarning); break;
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

  CutCollection::CutCollection() : fMode(ECutUpdate::kNo), fCutContainerArr(0) {
    Cout::PrintInfo("CutCollection: default constructor of CutCollection should never be used !!", EInfo::kLowWarning);
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

  void CutCollectionLinks::SafeInit() {
    if (fLinks.size() == 0) fLinks.push_back(0);
  }

  void CutCollectionLinks::AddAddr(Int_t addr) {
    if (addr < 0) return;
    fLinks.push_back(addr);
    fUsed = kTRUE;
  }
}  // namespace Hal
