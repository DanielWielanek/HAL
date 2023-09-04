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
#include "ObjectMatrix.h"
#include "Std.h"

#include <RtypesCore.h>
#include <TMath.h>
#include <TString.h>
#include <initializer_list>


namespace Hal {

  Double_t FemtoCorrFuncPt::GetPairValNum(Hal::FemtoPair* pair) const {
    Double_t pt1 = pair->GetPx1() * pair->GetPx1() + pair->GetPy1() * pair->GetPy1();
    Double_t pt2 = pair->GetPx2() * pair->GetPx2() + pair->GetPy2() * pair->GetPy2();
    return TMath::Sqrt(pt1) + TMath::Sqrt(pt2);
  }

  FemtoCorrFuncPt::FemtoCorrFuncPt(const DividedHisto1D& h, const std::initializer_list<double>& init, Int_t bins) :
    FemtoCorrFunc1D(h, init, bins, "pT", "p_{Tsum} [GeV/c]", "dN/dp_{T}") {}


}  // namespace Hal
