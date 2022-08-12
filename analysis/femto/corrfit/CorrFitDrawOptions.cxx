/*
 * CorrFitDrawOptions.cxx
 *
 *  Created on: 11 sie 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "CorrFitDrawOptions.h"
#include "Std.h"
#include "StdString.h"

#include <TRegexp.h>
namespace Hal {

  CorrFitDrawOptions::CorrFitDrawOptions() {
    // TODO Auto-generated constructor stub
  }

  CorrFitDrawOptions::~CorrFitDrawOptions() {
    // TODO Auto-generated destructor stub
  }

  CorrFitDrawOptions::CorrFitDrawOptions(TString options) : fRawCommand(options) {
    ExtrDraw(options);
    fAutoNorm   = Hal::Std::FindParam(options, "norm", kTRUE);
    fSame       = Hal::Std::FindParam(options, "same", kTRUE);
    fDrawCF     = Hal::Std::FindParam(options, "cf", kTRUE);
    fDrawNumErr = Hal::Std::FindParam(options, "ee", kTRUE);
    fDrawLegend = Hal::Std::FindParam(options, "leg", kTRUE);
    fDiag1      = Hal::Std::FindParam(options, "diag1", kTRUE);
    fDiag2      = Hal::Std::FindParam(options, "diag2", kTRUE);

    if (options == "full") {
      fDrawCF     = kTRUE;
      fDrawLegend = kTRUE;
    }
    CheckConflicts();
  }

  void CorrFitDrawOptions::ExtrDraw(TString& opt) {
    TRegexp regexp("{[-+]?[0-9]*\\.?[0-9]*,[-+]?[0-9]*\\.?[0-9]*}");
    TString expr = opt(regexp);
    if (expr.Length() > 1) {
      TRegexp low_expr("{[-+]?[0-9]*\\.?[0-9]*,");
      TRegexp high_expr(",[-+]?[0-9]*\\.?[0-9]*}");
      TString first = expr(low_expr);
      first.ReplaceAll(",", "");
      first.ReplaceAll("{", "");
      TString last = expr(high_expr);
      last.ReplaceAll(",", "");
      last.ReplaceAll("}", "");
      fMin = first.Atof();
      fMax = last.Atof();
      opt.ReplaceAll(expr, "");
    } else {
      fMin = 0;
      fMax = 0;
    }
  }

  CorrFitDrawOptions::CorrFitDrawOptions(std::initializer_list<int> opt, std::initializer_list<Double_t> val) {
    for (auto i : opt) {
      SetIntOpt(i);
    }
    if (val.size() == 2) {
      int cval = 0;
      for (auto v : val) {
        cval++;
        if (cval == 1) fMin = v;
        if (cval == 2) fMax = v;
      }
    }
    CheckConflicts();
  }

  void CorrFitDrawOptions::SetIntOpt(Int_t opt) {
    //    enum eCommon { kNorm = 0, kSame = 1, kNumError = 3, kLegend = 4 };
    // enum e3d { kRgb = 5 };
    switch (opt) {
      case kNorm: fAutoNorm = kTRUE; break;
      case kSame: fSame = kTRUE; break;
      case kNumError: fDrawNumErr = kTRUE; break;
      case kLegend: fDrawLegend = kTRUE; break;
      case kDrawCF: fDrawCF = kTRUE; break;
      case kDiag1: fDrawLegend = kTRUE; break;
      case kDiag2: fDrawCF = kTRUE; break;
      case kRgb: fRgb = kTRUE; break;
    }
  }

  void CorrFitDrawOptions::CheckConflicts() {
    if (fDiag1 == kTRUE && fDiag2 == kTRUE) fDiag1 = kFALSE;
    // if (fSame) fDrawCF = kFALSE;  // disable drawing cf when same is enabled
  }
} /* namespace Hal */
