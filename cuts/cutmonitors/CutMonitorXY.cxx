/*
 * CutMonitorXY.cpp
 *
 *  Created on: 10-07-2013
 *      Author: Daniel Wielanek
 */

#include "CutMonitorXY.h"

#include "Cut.h"

#include <RtypesCore.h>
#include <TAxis.h>
#include <TH1.h>
#include <TH2.h>
#include <TString.h>

#include "StdString.h"

namespace Hal {
  CutMonitorXY::CutMonitorXY() : CutMonitor(2) {
#ifdef MPPCUTFULL
    fIdenticalCuts = kFALSE;
#endif
  }

  CutMonitorXY::CutMonitorXY(TString cut_A, TString cut_B) : CutMonitorXY() {
    AddCut(cut_A);
    AddCut(cut_B);
  }

  CutMonitorXY::CutMonitorXY(TString cut_A, Int_t parameter_A, TString cut_B, Int_t parameter_B) : CutMonitorXY() {
    AddCut(cut_A, parameter_A);
    AddCut(cut_B, parameter_B);
  }

  void CutMonitorXY::TrueUpdate(Bool_t passed) {
#ifdef MPPCUTFULL
    Double_t weight = fCut[0]->GetWeight();
    if (fIdenticalCuts == kFALSE) { weight = weight * fCut[1]->GetWeight(); }
    if (passed) {
      ((TH2*) fHistoPassed)->Fill(fCut[0]->GetValue(fOptionAxis[0]), fCut[1]->GetValue(fOptionAxis[0]), weight);
    } else {
      ((TH2*) fHistoFailed)->Fill(fCut[0]->GetValue(fOptionAxis[0]), fCut[1]->GetValue(fOptionAxis[0]), weight);
    }
#else
    if (passed) {
      ((TH2D*) fHistoPassed)->Fill(fCut[0]->GetValue(fOptionAxis[0]), fCut[1]->GetValue(fOptionAxis[1]));
    } else {
      ((TH2D*) fHistoFailed)->Fill(fCut[0]->GetValue(fOptionAxis[0]), fCut[1]->GetValue(fOptionAxis[1]));
    }
#endif
  }

  CutMonitor* CutMonitorXY::MakeCopy() const { return (CutMonitor*) new CutMonitorXY(*this); }

  CutMonitorXY::CutMonitorXY(const CutMonitorXY& other) : CutMonitor(other) {}

  CutMonitorXY::~CutMonitorXY() {}

  Bool_t CutMonitorXY::Init(Int_t task_id) {
    return CutMonitor::Init(task_id);
#ifdef MPPCUTFULL
    if (fCut[0] == fCut[1]) fIdenticalCuts = kTRUE;
#endif
  }

  void CutMonitorXY::Update(Bool_t passed, TObject* /*obj*/) {
    if (fExUpdate) {
      if (fCut[0]->HasPassed() && fCut[1]->HasPassed()) {
        TrueUpdate(kTRUE);
      } else {
        TrueUpdate(kFALSE);
      }
    } else {
      TrueUpdate(passed);
    }
  }

  void CutMonitorXY::CreateHistograms() {
    TString title = Form("%s vs %s",
                         Hal::Std::RemoveUnits(fCut[1]->GetUnit(fOptionAxis[1])).Data(),
                         Hal::Std::RemoveUnits(fCut[0]->GetUnit(fOptionAxis[0])).Data());
    TString name;  // = Form("%s_vs_%s",
                   // fCut[0]->GetUnit(fOptionAxis[0]).Data(),fCut[1]->GetUnit(fOptionAxis[1]).Data());
    name         = "Passed";
    fHistoPassed = new TH2D(name, title, fAxisBins[0], fAxisMin[0], fAxisMax[0], fAxisBins[1], fAxisMin[1], fAxisMax[1]);
    fHistoPassed->GetXaxis()->SetTitle(fCut[0]->GetUnit(fOptionAxis[0]));
    fHistoPassed->GetYaxis()->SetTitle(fCut[1]->GetUnit(fOptionAxis[1]));
    name         = name + "_F";
    name         = "Failed";
    fHistoFailed = new TH2D(name, title, fAxisBins[0], fAxisMin[0], fAxisMax[0], fAxisBins[1], fAxisMin[1], fAxisMax[1]);
    fHistoFailed->GetXaxis()->SetTitle(fCut[0]->GetUnit(fOptionAxis[0]));
    fHistoFailed->GetYaxis()->SetTitle(fCut[1]->GetUnit(fOptionAxis[1]));

#ifdef MPPCUTFULL
    if (fCut[0] == fCut[1]) { fIdenticalCuts = kTRUE; }
#endif
  }

  CutMonitorXY& CutMonitorXY::operator=(const CutMonitorXY& other) {
    if (this != &other) { CutMonitor::operator=(other); }
    return *this;
  }
}  // namespace Hal
