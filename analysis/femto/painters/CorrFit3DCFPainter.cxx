/*
 * CorrFit3DCFPainter.cxx
 *
 *  Created on: 24 lip 2024
 *      Author: daniel
 */

#include "CorrFit3DCFPainter.h"

#include <RtypesCore.h>
#include <TString.h>
#include <TVirtualPad.h>
#include <vector>

#include "CorrFit.h"
#include "CorrFit3DCF.h"
#include "CorrFitFunc.h"
#include "CorrFitFunc3D.h"
#include "Cout.h"
#include "Femto3DCF.h"
#include "Femto3DCFPainter.h"
#include "Painter.h"


namespace Hal {

  void CorrFit3DCFPainter::UpdateParameters() {
    auto func = (CorrFit3DCF*) fFittedFunc;
    for (int i = 0; i < fFittedFunc->GetParametersNo(); i++) {
      func->fTempParamsEval[i] = func->GetParameter(i);
    }
    func->ParametersChanged();
    for (auto vec : fFunctions) {
      for (auto fx : vec) {
        if (fx) func->CopyParamsToTF1(fx, kTRUE, kTRUE);
      }
    }
  }

  TF1* CorrFit3DCFPainter::GetDrawableFunc(TString opt) {
    if (!fFittedFunc) return nullptr;
    return ((CorrFit3DCF*) fFittedFunc)->GetDrawableFunc(opt);
  }

  void CorrFit3DCFPainter::MakeFunctions() {
    auto addSingle = [&](TString opt) {
      std::vector<TF1*> vec;
      if (opt == "") {
        vec.push_back(nullptr);
        return vec;
      }
      auto th = GetDrawableFunc(opt);
      vec.push_back(th);
      return vec;
    };
    switch (fDrawOpt) {
      case 1: {
        fFunctions.push_back(addSingle("x"));
        fFunctions.push_back(addSingle("y"));
        fFunctions.push_back(addSingle("z"));
        fFunctions.push_back(addSingle(""));
      } break;
      case 2: {
        fFunctions.push_back(addSingle("x"));
        fFunctions.push_back(addSingle("y"));
        fFunctions.push_back(addSingle("z"));

        fFunctions.push_back(addSingle("xy++"));
        fFunctions.push_back(addSingle("xz++"));
        fFunctions.push_back(addSingle("yz++"));

        fFunctions.push_back(addSingle("xy+-"));
        fFunctions.push_back(addSingle("xz+-"));
        fFunctions.push_back(addSingle("yz+-"));

      } break;
      case 3: {
        fFunctions.push_back(addSingle("x"));
        fFunctions.push_back(addSingle("y"));
        fFunctions.push_back(addSingle("z"));
        fFunctions.push_back(addSingle(""));

        fFunctions.push_back(addSingle("xy++"));
        fFunctions.push_back(addSingle("xz++"));
        fFunctions.push_back(addSingle("yz++"));
        fFunctions.push_back(addSingle(""));

        fFunctions.push_back(addSingle("xy+-"));
        fFunctions.push_back(addSingle("xz+-"));
        fFunctions.push_back(addSingle("yz+-"));
        fFunctions.push_back(addSingle(""));

        fFunctions.push_back(addSingle("xyz+++"));
        fFunctions.push_back(addSingle("xyz+-+"));
        fFunctions.push_back(addSingle("xyz+--"));
        fFunctions.push_back(addSingle("xyz++-"));
      } break;
    }
  }

  void CorrFit3DCFPainter::ScaleHistograms() {
    auto func = (CorrFit3DCF*) fFittedFunc;
    if (!CheckOpt(kAutoNormBit)) fCFPainter->Rescale(1.0 / func->GetNorm());
  }

  CorrFit3DCFPainter::CorrFit3DCFPainter(CorrFit3DCF* fit, Femto3DCF* cf) : CorrFitPainter(fit), fCF(cf) {
    fCFPainter = (Femto3DCFPainter*) fCF->GetPainter();
    if (fCFPainter) fCFPainter->AddPainter(this);
    fLegendPos[0] = 0.05;
    fLegendPos[1] = 0.05;
    fLegendPos[2] = 0.95;
    fLegendPos[3] = 0.95;
  }

  CorrFit3DCFPainter::~CorrFit3DCFPainter() {}

  void CorrFit3DCFPainter::InnerPaint() {
    fCFPainter  = (Femto3DCFPainter*) fCF->GetPainter();
    TString opt = fCFPainter->GetOptionForCorrFit();
    if (opt.EqualTo("")) {
      Hal::Cout::PrintInfo(Form("%s %i not supported options for drawing histogram, corrfit cannot adapt", __FILE__, __LINE__),
                           EInfo::kCriticalError);
    }
    if (opt.EqualTo("fit1")) fDrawOpt = 1;
    if (opt.EqualTo("fit2")) fDrawOpt = 2;
    if (opt.EqualTo("fit3")) fDrawOpt = 3;
    switch (fDrawOpt) {
      case 1: fLegendPad = GetPad(4); break;
      case 2: fLegendPad = GetPad(9); break;
      case 3: fLegendPad = GetPad(4); break;
    }
    CorrFitPainter::InnerPaint();
  }

} /* namespace Hal */
