/*
 * Cuts.cxx
 *
 *  Created on: 21 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "CutsAndMonitors.h"
#include "CutMonitorX.h"
#include "CutMonitorXY.h"
#include "CutMonitorXYZ.h"

#include "HistogramManager.h"

#include <TObjArray.h>
#include <TObjString.h>

namespace Hal {
  CutsAndMonitors::CutsAndMonitors() :
    fCuts(nullptr),
    fCutsOptions(nullptr),
    fCutMonitors(nullptr),
    fCutMonitorsOptions(nullptr),
    fGlobalOptionCuts(""),
    fGlobalOptionsCutMonitors("") {}

  CutsAndMonitors::CutsAndMonitors(const CutsAndMonitors& other) :
    TObject(other),
    fCuts(nullptr),
    fCutsOptions(nullptr),
    fCutMonitors(nullptr),
    fCutMonitorsOptions(nullptr),
    fGlobalOptionCuts(other.fGlobalOptionCuts),
    fGlobalOptionsCutMonitors(other.fGlobalOptionsCutMonitors),
    fGlobalCollectionId(other.fGlobalCollectionId) {
    if (other.fCuts) {
      fCuts = new TObjArray();
      fCuts->SetOwner(kTRUE);
      fCutsOptions = new TObjArray();
      fCutsOptions->SetOwner(kTRUE);
      for (int iCut = 0; iCut < other.fCuts->GetEntries(); iCut++) {
        Cut* cut        = (Cut*) other.fCuts->UncheckedAt(iCut);
        TObjString* str = (TObjString*) other.fCutsOptions->UncheckedAt(iCut);
        fCuts->AddLast(cut->MakeCopy());
        fCutsOptions->AddLast(str->Clone());
      }
    }
    if (other.fCutMonitors) {
      fCutMonitors = new TObjArray();
      fCutMonitors->SetOwner(kTRUE);
      fCutMonitorsOptions = new TObjArray();
      fCutMonitorsOptions->SetOwner(kTRUE);
      for (int iCutMon = 0; iCutMon < other.fCutMonitors->GetEntries(); iCutMon++) {
        CutMonitor* cut = (CutMonitor*) other.fCutMonitors->UncheckedAt(iCutMon);
        TObjString* str = (TObjString*) other.fCutMonitorsOptions->UncheckedAt(iCutMon);
        fCutMonitors->AddLast(cut->MakeCopy());
        fCutMonitorsOptions->AddLast(str->Clone());
      }
    }
  }

  Int_t CutsAndMonitors::GetNCuts() const {
    if (fCuts == nullptr) return 0;
    return fCuts->GetEntries();
  }

  Int_t CutsAndMonitors::GetNCutMonitors() const {
    if (fCutMonitors == nullptr) return 0;
    return fCutMonitors->GetEntries();
  }

  CutsAndMonitors& CutsAndMonitors::operator=(const CutsAndMonitors& other) {
    if (&other == this) return *this;
    if (fCuts) {
      delete fCuts;
      delete fCutsOptions;
      fCuts        = nullptr;
      fCutsOptions = nullptr;
    }
    if (fCutMonitors) {
      delete fCutMonitors;
      delete fCutMonitorsOptions;
      fCutMonitors        = nullptr;
      fCutMonitorsOptions = nullptr;
    }
    fGlobalOptionCuts         = other.fGlobalOptionCuts;
    fGlobalOptionsCutMonitors = other.fGlobalOptionsCutMonitors;

    if (other.fCuts) {
      fCuts = new TObjArray();
      fCuts->SetOwner(kTRUE);
      fCutsOptions = new TObjArray();
      fCutsOptions->SetOwner(kTRUE);
      for (int iCut = 0; iCut < other.fCuts->GetEntries(); iCut++) {
        Cut* cut        = (Cut*) other.fCuts->UncheckedAt(iCut);
        TObjString* str = (TObjString*) other.fCutsOptions->UncheckedAt(iCut);
        fCuts->AddLast(cut->MakeCopy());
        fCutsOptions->AddLast(str->Clone());
      }
    }
    if (other.fCutMonitors) {
      fCutMonitors = new TObjArray();
      fCutMonitors->SetOwner(kTRUE);
      fCutMonitorsOptions = new TObjArray();
      fCutMonitorsOptions->SetOwner(kTRUE);
      for (int iCutMon = 0; iCutMon < other.fCutMonitors->GetEntries(); iCutMon++) {
        CutMonitor* cut = (CutMonitor*) other.fCutMonitors->UncheckedAt(iCutMon);
        TObjString* str = (TObjString*) other.fCutMonitorsOptions->UncheckedAt(iCutMon);
        fCutMonitors->AddLast(cut->MakeCopy());
        fCutMonitorsOptions->AddLast(str->Clone());
      }
    }
    return *this;
  }

  void CutsAndMonitors::AddCut(const Cut& cut, Option_t* opt) {
    if (fCuts == nullptr) {
      fCuts        = new TObjArray();
      fCutsOptions = new TObjArray();
      fCuts->SetOwner(kTRUE);
      fCutsOptions->SetOwner(kTRUE);
    }
    TObjString* str = new TObjString(opt);
    fCuts->Add(cut.MakeCopy());
    fCutsOptions->Add(str);
  }

  void CutsAndMonitors::AddCutMonitor(const CutMonitor& monitor, Option_t* opt) {
    if (fCutMonitors == nullptr) {
      fCutMonitors        = new TObjArray();
      fCutMonitorsOptions = new TObjArray();
      fCutMonitors->SetOwner(kTRUE);
      fCutMonitorsOptions->SetOwner(kTRUE);
    }
    TObjString* str = new TObjString(opt);
    fCutMonitors->Add(monitor.MakeCopy());
    fCutMonitorsOptions->Add(str);
  }

  const Cut* CutsAndMonitors::GetCut(Int_t i) const { return static_cast<Cut*>(fCuts->UncheckedAt(i)); }

  TString CutsAndMonitors::GetCutOption(Int_t i) const {
    TString opt = static_cast<TObjString*>(fCutsOptions->UncheckedAt(i))->GetString();
    if (opt == "" || fGlobalOptionCuts == "") { return opt + fGlobalOptionCuts; }
    return opt + "+" + fGlobalOptionCuts;
  }

  const CutMonitor* CutsAndMonitors::GetMonitor(Int_t i) const { return static_cast<CutMonitor*>(fCutMonitors->UncheckedAt(i)); }

  TString CutsAndMonitors::GetCutMonitorOption(Int_t i) const {
    TString opt = static_cast<TObjString*>(fCutMonitorsOptions->UncheckedAt(i))->GetString();
    if (opt == "" || fGlobalOptionsCutMonitors == "") { return opt + fGlobalOptionsCutMonitors; }
    return opt + "+" + fGlobalOptionsCutMonitors;
  }

  CutsAndMonitors::~CutsAndMonitors() {
    if (fCutMonitors) delete fCutMonitors;
    if (fCutMonitorsOptions) delete fCutMonitorsOptions;
    if (fCuts) delete fCuts;
    if (fCutsOptions) delete fCutsOptions;
  }

  Cut* CutsAndMonitors::CutAt(Int_t i) const { return static_cast<Cut*>(fCuts->UncheckedAt(i)); }

  CutMonitor* CutsAndMonitors::MonitorAt(Int_t i) const { return static_cast<CutMonitor*>(fCutMonitors->UncheckedAt(i)); }

  void CutsAndMonitors::ClearCuts() {
    fCuts->Clear();
    fCutsOptions->Clear();
  }

  void CutsAndMonitors::ClearMonitors() {
    fCutMonitors->Clear();
    fCutMonitorsOptions->Clear();
  }

  void CutsAndMonitors::AddRawCut(Cut* cut, TObjString* opt) {
    if (opt == nullptr) { opt = new TObjString(""); }
    if (fCuts == nullptr) {
      fCuts        = new TObjArray();
      fCutsOptions = new TObjArray();
      fCuts->SetOwner(kTRUE);
      fCutsOptions->SetOwner(kTRUE);
    }
    fCuts->Add(cut);
    fCutsOptions->Add(opt);
  }

  void CutsAndMonitors::AddRawCutMonitor(CutMonitor* mon, TObjString* opt) {
    if (opt == nullptr) { opt = new TObjString(""); }
    if (fCutMonitors == nullptr) {
      fCutMonitors        = new TObjArray();
      fCutMonitorsOptions = new TObjArray();
      fCutMonitors->SetOwner(kTRUE);
      fCutMonitorsOptions->SetOwner(kTRUE);
    }
    fCutMonitors->Add(mon);
    fCutMonitorsOptions->Add(opt);
  }

  CutMonitor* CutsAndMonitors::MakeCutMonitor(Int_t request_no) const {
    if (request_no < 0 || request_no > (int) fCutMonitorRequests.size()) return nullptr;
    CutMonitorRequest req = fCutMonitorRequests[request_no];
    auto TransformOpt     = [](TString opt) {
      if (Hal::Std::FindParam(opt, "re")) {
        return "re";
      } else if (Hal::Std::FindParam(opt, "im")) {
        return "im";
      } else {
        return "";
      }
    };
    Int_t dim = req.GetNDim();
    switch (dim) {
      case 1: {
        CutMonAxisConf x = req.GetConf('x');
        if (x.GetCutNo() < 0) x.FixCutNo(GetCutNo(x.GetCutClassName()));
        const Cut* CutX  = GetCut(x.GetCutNo());
        TString optX     = TransformOpt(GetCutOption(x.GetCutNo()));
        CutMonitorX* mon = new CutMonitorX(CutX->CutName(optX), x.GetParameterNo());
        mon->SetXaxis(x.GetNBins(), x.GetMin(), x.GetMax());
        return mon;
      } break;
      case 2: {
        CutMonAxisConf x = req.GetConf('x');
        CutMonAxisConf y = req.GetConf('y');
        if (x.GetCutNo() < 0) x.FixCutNo(GetCutNo(x.GetCutClassName()));
        if (y.GetCutNo() < 0) y.FixCutNo(GetCutNo(y.GetCutClassName()));
        const Cut* CutX   = GetCut(x.GetCutNo());
        const Cut* CutY   = GetCut(y.GetCutNo());
        TString optX      = TransformOpt(GetCutOption(x.GetCutNo()));
        TString optY      = TransformOpt(GetCutOption(y.GetCutNo()));
        CutMonitorXY* mon = new CutMonitorXY(CutX->CutName(optX), x.GetParameterNo(), CutY->CutName(optY), y.GetParameterNo());
        mon->SetXaxis(x.GetNBins(), x.GetMin(), x.GetMax());
        mon->SetYaxis(y.GetNBins(), y.GetMin(), y.GetMax());
        return mon;
      } break;
      case 3: {
        CutMonAxisConf x = req.GetConf('x');
        CutMonAxisConf y = req.GetConf('y');
        CutMonAxisConf z = req.GetConf('z');
        if (x.GetCutNo() < 0) x.FixCutNo(GetCutNo(x.GetCutClassName()));
        if (y.GetCutNo() < 0) y.FixCutNo(GetCutNo(y.GetCutClassName()));
        if (z.GetCutNo() < 0) z.FixCutNo(GetCutNo(z.GetCutClassName()));
        const Cut* CutX    = GetCut(x.GetCutNo());
        const Cut* CutY    = GetCut(y.GetCutNo());
        const Cut* CutZ    = GetCut(z.GetCutNo());
        TString optX       = TransformOpt(GetCutOption(x.GetCutNo()));
        TString optY       = TransformOpt(GetCutOption(y.GetCutNo()));
        TString optZ       = TransformOpt(GetCutOption(z.GetCutNo()));
        CutMonitorXYZ* mon = new CutMonitorXYZ(CutX->CutName(optX),
                                               x.GetParameterNo(),
                                               CutY->CutName(optY),
                                               y.GetParameterNo(),
                                               CutZ->CutName(optZ),
                                               z.GetParameterNo());
        mon->SetXaxis(x.GetNBins(), x.GetMin(), x.GetMax());
        mon->SetYaxis(y.GetNBins(), y.GetMin(), y.GetMax());
        mon->SetZaxis(z.GetNBins(), z.GetMin(), z.GetMax());
        return mon;
      } break;
    }
    return nullptr;
  }

  void CutsAndMonitors::MakeCutMonitors(Option_t* opt) {
    AddAllCutMonitorRequests(opt);
    if (GetNRequest() != 0) {
      for (int iMon = 0; iMon < GetNRequest(); iMon++) {
        CutMonitor* mon = MakeCutMonitor(iMon);
        if (fGlobalCollectionId != -1) { mon->SetCollectionID(fGlobalCollectionId); }
        if (mon != nullptr) { AddRawCutMonitor(mon); }
      }
    }
  }

  Int_t CutsAndMonitors::GetCutNo(TString classname) const {
    for (int i = 0; i < fCuts->GetEntriesFast(); i++) {
      Cut* cut     = static_cast<Cut*>(fCuts->UncheckedAt(i));
      TString name = cut->ClassName();
      if (name.EqualTo(classname)) return i;
    }
    return -1;
  }

  void CutsAndMonitors::AddCutMonitorRequest(const CutMonAxisConf& x) { fCutMonitorRequests.push_back(CutMonitorRequest(x)); }

  void CutsAndMonitors::AddCutMonitorRequest(const CutMonAxisConf& x, const CutMonAxisConf& y) {
    fCutMonitorRequests.push_back(CutMonitorRequest(x, y));
  }

  void CutsAndMonitors::AddCutMonitorRequest(const CutMonAxisConf& x, const CutMonAxisConf& y, const CutMonAxisConf& z) {
    fCutMonitorRequests.push_back(CutMonitorRequest(x, y, z));
  }

  void CutsAndMonitors::SetCollectionID(Int_t id) {
    fGlobalCollectionId = id;
    if (fGlobalCollectionId != -1) {
      for (int iCut = 0; iCut < fCuts->GetEntriesFast(); iCut++) {
        ((Cut*) fCuts->UncheckedAt(iCut))->SetCollectionID(fGlobalCollectionId);
      }
      for (int iMon = 0; iMon < fCutMonitors->GetEntriesFast(); iMon++) {
        ((CutMonitor*) fCutMonitors->UncheckedAt(iMon))->SetCollectionID(fGlobalCollectionId);
      }
    }


  }  // namespace Hal
}  // namespace Hal
