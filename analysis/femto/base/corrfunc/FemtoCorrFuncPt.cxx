/*
 * CorrFuncCollectionPt.cxx
 *
 *  Created on: 4 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoCorrFuncPt.h"

#include "Array.h"
#include "Cout.h"
#include "DividedHisto.h"
#include "FemtoPair.h"
#include "Std.h"
#include "ObjectMatrix.h"

#include <RtypesCore.h>
#include <TMath.h>
#include <TString.h>
#include <initializer_list>


namespace Hal {
  FemtoCorrFuncPt::FemtoCorrFuncPt(const DividedHisto1D* h, const std::initializer_list<double>& init) :
    FemtoCorrFunc1D(h, "p_{T sum} [GeV/c]") {
    SetBins(init);
  }

  Bool_t FemtoCorrFuncPt::Check() {
    for (int i = 1; i < fRange.GetSize(); i++) {
      if (fRange[i] < fRange[i - 1]) {
        Cout::PrintInfo(Form("Wrong order in %s %4.2f<%4.2f", this->ClassName(), fRange[i], fRange[i - 1]),
                        EInfo::kCriticalError);
        return kFALSE;
      }
    }
    return FemtoCorrFunc1D::Check();
  }

  void FemtoCorrFuncPt::FillNum(FemtoPair* pair) {
    Double_t pt1     = pair->GetPx1() * pair->GetPx1() + pair->GetPy1() * pair->GetPy1();
    Double_t pt2     = pair->GetPx2() * pair->GetPx2() + pair->GetPy2() * pair->GetPy2();
    Double_t pt      = TMath::Sqrt(pt1) + TMath::Sqrt(pt2);
    const Int_t size = fRange.GetSize();
    if (pt < fRange.Get(0)) return;
    for (int i = 1; i < size; i++) {
      if (pt < fRange.Get(i)) { ((DividedHisto1D*) fArray->At(i - 1))->FillNumObj(pair); }
    }
  }

  void FemtoCorrFuncPt::FillDenMixed(FemtoPair* pair) {
    Double_t pt1     = pair->GetPx1() * pair->GetPx1() + pair->GetPy1() * pair->GetPy1();
    Double_t pt2     = pair->GetPx2() * pair->GetPx2() + pair->GetPy2() * pair->GetPy2();
    Double_t pt      = TMath::Sqrt(pt1) + TMath::Sqrt(pt2);
    const Int_t size = fRange.GetSize();
    if (pt < fRange.Get(0)) return;
    for (int i = 1; i < size; i++) {
      if (pt < fRange.Get(i)) { ((DividedHisto1D*) fArray->At(i - 1))->FillDenObj(pair); }
    }
  }

  FemtoCorrFuncPt::~FemtoCorrFuncPt() {}

  FemtoCorrFuncPt::FemtoCorrFuncPt(const DividedHisto1D& h, const std::initializer_list<double>& init) :
    FemtoCorrFuncPt(&h, init) {}
}  // namespace Hal
