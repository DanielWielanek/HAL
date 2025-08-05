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
  CutsAndMonitors::CutsAndMonitors() : fGlobalOptionCuts(""), fGlobalOptionsCutMonitors("") {
    fCuts.SetOwner(kTRUE);
    fCutMonitors.SetOwner(kTRUE);
  }

  CutsAndMonitors::CutsAndMonitors(const CutsAndMonitors& other) :
    TObject(other),
    fGlobalOptionCuts(other.fGlobalOptionCuts),
    fGlobalOptionsCutMonitors(other.fGlobalOptionsCutMonitors),
    fGlobalCollectionId(other.fGlobalCollectionId) {
    fCuts.SetOwner(kTRUE);
    fCutsOptions        = other.fCutsOptions;
    fCutMonitorsOptions = other.fCutMonitorsOptions;
    for (int iCut = 0; iCut < other.fCuts.GetEntries(); iCut++) {
      Cut* cut = (Cut*) other.fCuts.UncheckedAt(iCut);
      fCuts.AddLast(cut->MakeCopy());
    }

    for (int iCutMon = 0; iCutMon < other.fCutMonitors.GetEntries(); iCutMon++) {
      CutMonitor* cut = (CutMonitor*) other.fCutMonitors.UncheckedAt(iCutMon);
      fCutMonitors.AddLast(cut->MakeCopy());
    }
  }

  Int_t CutsAndMonitors::GetNCuts() const { return fCuts.GetEntries(); }

  Int_t CutsAndMonitors::GetNCutMonitors() const { return fCutMonitors.GetEntries(); }

  CutsAndMonitors& CutsAndMonitors::operator=(const CutsAndMonitors& other) {
    if (&other == this) return *this;

    fCuts.Clear();
    fCutsOptions = other.fCutsOptions;
    fCutMonitors.Clear();
    fCutMonitorsOptions       = other.fCutMonitorsOptions;
    fGlobalOptionCuts         = other.fGlobalOptionCuts;
    fGlobalOptionsCutMonitors = other.fGlobalOptionsCutMonitors;
    for (int iCut = 0; iCut < other.fCuts.GetEntries(); iCut++) {
      Cut* cut = (Cut*) other.fCuts.UncheckedAt(iCut);
      fCuts.AddLast(cut->MakeCopy());
    }
    for (int iCutMon = 0; iCutMon < other.fCutMonitors.GetEntries(); iCutMon++) {
      CutMonitor* cut = (CutMonitor*) other.fCutMonitors.UncheckedAt(iCutMon);
      fCutMonitors.AddLast(cut->MakeCopy());
    }
    return *this;
  }

  void CutsAndMonitors::AddCut(const Cut& cut, Option_t* opt) {
    TString str = opt;
    fCuts.Add(cut.MakeCopy());
    fCutsOptions.push_back(str);
  }

  void CutsAndMonitors::AddCutMonitor(const CutMonitor& monitor, Option_t* opt) {
    TString str = opt;
    fCutMonitors.Add(monitor.MakeCopy());
    fCutMonitorsOptions.push_back(str);
  }

  const Cut* CutsAndMonitors::GetCut(Int_t i) const { return static_cast<Cut*>(fCuts.UncheckedAt(i)); }

  TString CutsAndMonitors::GetCutOption(Int_t i) const {
    TString opt = fCutsOptions[i];
    if (opt == "" || fGlobalOptionCuts == "") { return opt + fGlobalOptionCuts; }
    return opt + "+" + fGlobalOptionCuts;
  }

  const CutMonitor* CutsAndMonitors::GetMonitor(Int_t i) const { return static_cast<CutMonitor*>(fCutMonitors.UncheckedAt(i)); }

  TString CutsAndMonitors::GetCutMonitorOption(Int_t i) const {
    TString opt = fCutMonitorsOptions[i];
    if (opt == "" || fGlobalOptionsCutMonitors == "") { return opt + fGlobalOptionsCutMonitors; }
    return opt + "+" + fGlobalOptionsCutMonitors;
  }

  CutsAndMonitors::~CutsAndMonitors() {}

  Cut* CutsAndMonitors::CutAt(Int_t i) const { return static_cast<Cut*>(fCuts.UncheckedAt(i)); }

  CutMonitor* CutsAndMonitors::MonitorAt(Int_t i) const { return static_cast<CutMonitor*>(fCutMonitors.UncheckedAt(i)); }

  void CutsAndMonitors::ClearCuts() {
    fCuts.Clear();
    fCutsOptions.clear();
  }

  void CutsAndMonitors::ClearMonitors() {
    fCutMonitors.Clear();
    fCutMonitorsOptions.clear();
  }

  void CutsAndMonitors::AddRawCut(Cut* cut, TString opt) {
    fCuts.Add(cut);
    fCutsOptions.push_back(opt);
  }

  void CutsAndMonitors::AddRawCutMonitor(CutMonitor* mon, TString opt) {
    fCutMonitors.Add(mon);
    fCutMonitorsOptions.push_back(opt);
  }

  CutMonitor* CutsAndMonitors::MakeCutMonitor(Int_t request_no) const {
    if (request_no < 0 || request_no > (int) fCutMonitorRequests.size()) return nullptr;
    CutMonitorRequest req = fCutMonitorRequests[request_no];
    Int_t dim             = req.GetNDim();
    switch (dim) {
      case 1: {
        CutMonAxisConf x = req.GetConf('x');
        if (x.GetCutNo() < 0) x.FixCutNo(GetCutNo(x.GetCutClassName()));
        const Cut* CutX  = GetCut(x.GetCutNo());
        CutMonitorX* mon = new CutMonitorX(CutX->CutName(), x.GetParameterNo());
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
        CutMonitorXY* mon = new CutMonitorXY(CutX->CutName(), x.GetParameterNo(), CutY->CutName(), y.GetParameterNo());
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
        CutMonitorXYZ* mon = new CutMonitorXYZ(
          CutX->CutName(), x.GetParameterNo(), CutY->CutName(), y.GetParameterNo(), CutZ->CutName(), z.GetParameterNo());
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
    for (int i = 0; i < fCuts.GetEntriesFast(); i++) {
      Cut* cut     = static_cast<Cut*>(fCuts.UncheckedAt(i));
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
      for (int iCut = 0; iCut < fCuts.GetEntriesFast(); iCut++) {
        ((Cut*) fCuts.UncheckedAt(iCut))->SetCollectionID(fGlobalCollectionId);
      }
      for (int iMon = 0; iMon < fCutMonitors.GetEntriesFast(); iMon++) {
        ((CutMonitor*) fCutMonitors.UncheckedAt(iMon))->SetCollectionID(fGlobalCollectionId);
      }
    }


  }  // namespace Hal
}  // namespace Hal
