/*
 * CorrFit1DPainter.cxx
 *
 *  Created on: 23 lip 2024
 *      Author: daniel
 */

#include "CorrFit1DCFPainter.h"

#include <RtypesCore.h>
#include <TF1.h>
#include <TLegend.h>
#include <TString.h>
#include <TVirtualPad.h>
#include <vector>

#include "CorrFit.h"
#include "CorrFit1DCF.h"
#include "CorrFitFunc.h"
#include "Femto1DCF.h"
#include "Femto1DCFPainter.h"
#include "Painter.h"

namespace Hal {

  void CorrFit1DCFPainter::MakeFunctions() {
    std::vector<TF1*> vec;
    vec.push_back(GetDrawableFunc(""));
    fFunctions.push_back(vec);
  }

  void CorrFit1DCFPainter::ScaleHistograms() {
    auto func = (CorrFit1DCF*) fFittedFunc;
    if (!CheckOpt(kAutoNormBit)) fCFPainter->Rescale(1.0 / func->GetNorm());
  }

  CorrFit1DCFPainter::CorrFit1DCFPainter(CorrFit1DCF* fit, Femto1DCF* cf) : CorrFitPainter(fit), fCF(cf) {
    fCFPainter = (Femto1DCFPainter*) fCF->GetPainter();
    if (fCFPainter) fCFPainter->AddPainter(this);
  }

  TF1* CorrFit1DCFPainter::GetDrawableFunc(TString opt) {
    if (!fFittedFunc) return nullptr;
    return ((CorrFit1DCF*) fFittedFunc)->GetFunctionForDrawing();
  }

  void CorrFit1DCFPainter::MakeLegend() {
    if (CheckOpt(kLegendBit)) {
      LockPad();
      fLegendPad = GetPad(1);
      UnlockPad();
    }
    CorrFitPainter::MakeLegend();
  }

  CorrFit1DCFPainter::~CorrFit1DCFPainter() {}

  void CorrFit1DCFPainter::UpdateParameters() {
    auto func = (CorrFit1DCF*) fFittedFunc;
    for (int i = 0; i < fFittedFunc->GetParametersNo(); i++) {
      func->fTempParamsEval[i] = func->GetParameter(i);
    }
    // TODO fFittedFunction->SetParameter(par,val);
    func->ParametersChanged();
    func->CopyParamsToTF1(fFunctions[0][0], kTRUE, kTRUE);
  }

} /* namespace Hal */
