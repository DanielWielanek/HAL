/*
 * CutMonitorXYZ.cxx
 *
 *  Created on: 10-07-2013
 *      Author: Daniel Wielanek
 */

#include "CutMonitorXYZ.h"


#include <TAxis.h>
#include <TH1.h>
#include <TH3.h>
#include <TString.h>

#include "Cut.h"
#include "StdString.h"

namespace Hal {
  CutMonitorXYZ::CutMonitorXYZ() : CutMonitor(3) {
#ifdef MPPCUTFULL
    fWeightFlag = 0;
#endif
  }

  CutMonitorXYZ::CutMonitorXYZ(TString cut_A, TString cut_B, TString cut_C) : CutMonitorXYZ() {
    AddCut(cut_A);
    AddCut(cut_B);
    AddCut(cut_C);
  }

  CutMonitorXYZ::CutMonitorXYZ(TString cut_A, Int_t par_A, TString cut_B, Int_t par_B, TString cut_C, Int_t par_C) :
    CutMonitorXYZ() {
    AddCut(cut_A, par_A);
    AddCut(cut_B, par_B);
    AddCut(cut_C, par_C);
  }

  void CutMonitorXYZ::TrueUpdate(Bool_t pass) {
#ifdef MPPCUTFULL
    Double_t weight = fCut[0]->GetWeight();
    switch (fWeightFlag) {
      case 2: weight = fCut[0]->GetWeight() * fCut[2]->GetWeight(); break;
      case 3: weight = fCut[0]->GetWeight() * fCut[1]->GetWeight(); break;
      case 1: weight = fCut[0]->GetWeight(); break;
      default: weight = fCut[0]->GetWeight() * fCut[1]->GetWeight() * fCut[2]->GetWeight(); break;
    }
    if (pass) {
      ((TH3F*) fHistoPassed)
        ->Fill(fCut[0]->GetValue(fOptionAxis[0]), fCut[1]->GetValue(fOptionAxis[1]), fCut[2]->GetValue(fOptionAxis[2]), weight);
    } else {
      ((TH3F*) fHistoFailed)
        ->Fill(fCut[0]->GetValue(fOptionAxis[0]), fCut[1]->GetValue(fOptionAxis[1]), fCut[2]->GetValue(fOptionAxis[2]), weight);
    }
#endif
    if (pass) {
      ((TH3D*) fHistoPassed)
        ->Fill(fCut[0]->GetValue(fOptionAxis[0]), fCut[1]->GetValue(fOptionAxis[1]), fCut[2]->GetValue(fOptionAxis[2]));
    } else {
      ((TH3D*) fHistoFailed)
        ->Fill(fCut[0]->GetValue(fOptionAxis[0]), fCut[1]->GetValue(fOptionAxis[1]), fCut[2]->GetValue(fOptionAxis[2]));
    }
  }

  CutMonitor* CutMonitorXYZ::MakeCopy() const { return (CutMonitor*) new CutMonitorXYZ(*this); }

  CutMonitorXYZ::CutMonitorXYZ(const CutMonitorXYZ& other) : CutMonitor(other) {}

  CutMonitorXYZ::~CutMonitorXYZ() {}

  void CutMonitorXYZ::Update(Bool_t pass, TObject* /*obj*/) {
    if (fExUpdate) {
      if (fCut[0]->HasPassed() && fCut[1]->HasPassed() && fCut[2]->HasPassed()) {
        TrueUpdate(kTRUE);
      } else {
        TrueUpdate(kFALSE);
      }
    } else {
      TrueUpdate(pass);
    }
  }

  void CutMonitorXYZ::CreateHistograms() {
#ifdef MPPCUTFULL
    // ignore 2nd weight
    if (fCut[0] == fCut[1] || fCut[1] == fCut[2]) { fWeightFlag = 2; }
    // ignore 3rd weight
    if (fCut[0] == fCut[2] || fCut[1] == fCut[2]) { fWeightFlag = 3; }
    // all 3 cuts are the same - ignore 2nd and 3rd weight
    if ((fCut[0] == fCut[1]) && fCut[0] == fCut[2]) { fWeightFlag = 1; }
#endif
    TString title = Form("%s vs %s vs %s",
                         Hal::Std::RemoveUnits(fCut[2]->GetUnit(fOptionAxis[2])).Data(),
                         Hal::Std::RemoveUnits(fCut[1]->GetUnit(fOptionAxis[1])).Data(),
                         Hal::Std::RemoveUnits(fCut[0]->GetUnit(fOptionAxis[0])).Data());
    TString
      name;  // = Form("%s_vs_%s_vs_%s",
             // fCut[0]->GetUnit(fOptionAxis[0]).Data(),fCut[1]->GetUnit(fOptionAxis[1]).Data(),fCut[2]->GetUnit(fOptionAxis[2]).Data());
    name = "Passed";
    // title = title + Form(" ** %s %s %s
    // ",fCut[0]->ClassName(),fCut[1]->ClassName(),fCut[2]->ClassName());
    fHistoPassed = new TH3D(name,
                            title,
                            fAxisBins[0],
                            fAxisMin[0],
                            fAxisMax[0],
                            fAxisBins[1],
                            fAxisMin[1],
                            fAxisMax[1],
                            fAxisBins[2],
                            fAxisMin[2],
                            fAxisMax[2]);
    fHistoPassed->GetXaxis()->SetTitle(fCut[0]->GetUnit(fOptionAxis[0]));
    fHistoPassed->GetYaxis()->SetTitle(fCut[1]->GetUnit(fOptionAxis[1]));
    fHistoPassed->GetZaxis()->SetTitle(fCut[2]->GetUnit(fOptionAxis[2]));
    name         = "Failed";
    fHistoFailed = (TH3D*) fHistoPassed->Clone(name);
    fInit        = kTRUE;
  }

  Bool_t CutMonitorXYZ::Init(Int_t task_id) {
    if (fInit == kFALSE) { return CutMonitor::Init(task_id); }
    return kFALSE;
  }

  CutMonitorXYZ& CutMonitorXYZ::operator=(const CutMonitorXYZ& other) {
    if (this != &other) { CutMonitor::operator=(other); }
    return *this;
  }
}  // namespace Hal
