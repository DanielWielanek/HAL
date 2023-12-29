/*
 * CutMonitorX.cxx
 *
 *  Created on: 10-07-2013
 *      Author: Daniel Wielanek
 */

#include "CutMonitorX.h"


#include <TAttFill.h>
#include <TAxis.h>
#include <TH1.h>
#include <TString.h>

#include "Cut.h"
#include "StdString.h"

namespace Hal {
  CutMonitorX::CutMonitorX() : CutMonitor(1) {}

  CutMonitorX::CutMonitorX(TString cut_name, Int_t parameter_no, std::initializer_list<Double_t> list) :
    CutMonitorX(cut_name, parameter_no) {
    auto vec = Hal::Std::GetVector(list);
    if (vec.size() < 3) return;
    SetXaxis(vec[0], vec[1], vec[2]);
  }

  CutMonitorX::CutMonitorX(TString cut_name, Int_t parameter_no) : CutMonitorX() { AddCut(cut_name, parameter_no); }

  void CutMonitorX::TrueUpdate(Bool_t passed) {
#ifdef MPPCUTFULL
    if (passed) {
      fHistoPassed->Fill(fCut[0]->GetValue(fOptionAxis[0]), fCut[0]->GetWeight());
    } else {
      fHistoFailed->Fill(fCut[0]->GetValue(fOptionAxis[0]), fCut[0]->GetWeight());
    }
#else
    if (passed) {
      fHistoPassed->Fill(fCut[0]->GetValue(fOptionAxis[0]), 1.0);
    } else {
      fHistoFailed->Fill(fCut[0]->GetValue(fOptionAxis[0]), 1.0);
    }
#endif
  }

  CutMonitor* CutMonitorX::MakeCopy() const { return (CutMonitor*) new CutMonitorX(*this); }

  CutMonitorX::CutMonitorX(const CutMonitorX& other) : CutMonitor(other) {}

  CutMonitorX::~CutMonitorX() {}

  void CutMonitorX::Update(Bool_t passed, TObject* /*obj*/) {
    if (fExUpdate) {
      TrueUpdate(fCut[0]->HasPassed());
    } else {
      TrueUpdate(passed);
    }
  }

  void CutMonitorX::CreateHistograms() {
    TString title = Form("%s", Hal::Std::RemoveUnits(fCut[0]->GetUnit(fOptionAxis[0])).Data());
    TString name  = "Passed";
    fHistoPassed  = new FastHist1D(name, title, fAxisBins[0], fAxisMin[0], fAxisMax[0]);
    fHistoPassed->SetXaxisName(fCut[0]->GetUnit(fOptionAxis[0]).Data());
    TString y_axis_name       = fCut[0]->GetGroupFlag();  // take flag group like dNTracks
    TString y_axis_name_units = fCut[0]->GetUnit(fOptionAxis[0]);
    y_axis_name               = y_axis_name + "/";
    y_axis_name               = y_axis_name + Hal::Std::RemoveUnits(y_axis_name_units);
    fHistoPassed->SetYaxisName(y_axis_name);
    name         = "Failed";
    fHistoFailed = (FastHist1D*) fHistoPassed->Clone(name);
  }

  Bool_t CutMonitorX::Init(Int_t task_id) {
    if (fInit == kFALSE) { return CutMonitor::Init(task_id); }
    return kFALSE;
  }

  CutMonitorX& CutMonitorX::operator=(const CutMonitorX& other) {
    if (this != &other) { CutMonitor::operator=(other); }
    return *this;
  }
}  // namespace Hal
