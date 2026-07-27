/*
 * CutMonitorXY.cpp
 *
 *  Created on: 10-07-2013
 *      Author: Daniel Wielanek
 */

#include "CutMonitorXY.h"

#include "Cut.h"
#include "CutOptions.h"

#include <RtypesCore.h>
#include <TAxis.h>
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

  CutMonitorXY::CutMonitorXY(TString cut_A,
                             Int_t parameter_A,
                             std::initializer_list<Double_t> xAxis,
                             TString cut_B,
                             Int_t parameter_B,
                             std::initializer_list<Double_t> yAxis) :
    CutMonitorXY(cut_A, parameter_A, cut_B, parameter_B) {
    auto vecX = Hal::Std::GetVector(xAxis);
    auto vecY = Hal::Std::GetVector(yAxis);
    if (vecX.size() == 3) SetXaxis(vecX[0], vecX[1], vecX[2]);
    if (vecY.size() == 3) SetYaxis(vecY[0], vecY[1], vecY[2]);
  }

  CutMonitorXY::CutMonitorXY(TString cut_A, Int_t parameter_A, TString cut_B, Int_t parameter_B) : CutMonitorXY() {
    AddCut(cut_A, parameter_A);
    AddCut(cut_B, parameter_B);
  }

  void CutMonitorXY::TrueUpdate(Bool_t passed) {
    ManualFill2D(fCut[0]->GetValue(fOptionAxis[0]), fCut[1]->GetValue(fOptionAxis[1]), passed);
  }

  Bool_t CutMonitorXY::Init(Int_t task_id) {
    return CutMonitor::Init(task_id);
#ifdef MPPCUTFULL
    if (fCut[0] == fCut[1]) fIdenticalCuts = kTRUE;
#endif
  }

  void CutMonitorXY::Update(Bool_t passed, TObject* /*obj*/) {
    if (IsExclusive()) {
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
    fHistoPassed = new FastHist2D(name, title, fAxisBins[0], fAxisMin[0], fAxisMax[0], fAxisBins[1], fAxisMin[1], fAxisMax[1]);
    fHistoPassed->SetXaxisName(fCut[0]->GetUnit(fOptionAxis[0]));
    fHistoPassed->SetYaxisName(fCut[1]->GetUnit(fOptionAxis[1]));
    name         = "Failed";
    fHistoFailed = new FastHist2D(name, title, fAxisBins[0], fAxisMin[0], fAxisMax[0], fAxisBins[1], fAxisMin[1], fAxisMax[1]);
    fHistoFailed->SetXaxisName(fCut[0]->GetUnit(fOptionAxis[0]));
    fHistoFailed->SetYaxisName(fCut[1]->GetUnit(fOptionAxis[1]));

#ifdef MPPCUTFULL
    if (fCut[0] == fCut[1]) { fIdenticalCuts = kTRUE; }
#endif
  }

  CutMonitorXY& CutMonitorXY::operator=(const CutMonitorXY& other) {
    if (this != &other) { CutMonitor::operator=(other); }
    return *this;
  }
}  // namespace Hal
