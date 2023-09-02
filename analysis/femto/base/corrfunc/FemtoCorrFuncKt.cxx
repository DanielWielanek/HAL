/*
 * CorrFuncCollectionKt.cxx
 *
 *  Created on: 4 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "FemtoCorrFuncKt.h"

#include "Array.h"
#include "Cout.h"
#include "DividedHisto.h"
#include "FemtoPair.h"
#include "HtmlCore.h"
#include "HtmlFile.h"
#include "ObjectMatrix.h"
#include "Std.h"

#include <TCanvas.h>
#include <TH1.h>
#include <TMath.h>
#include <TROOT.h>
#include <TString.h>
#include <initializer_list>

#include <iostream>

namespace Hal {
  FemtoCorrFuncKt::FemtoCorrFuncKt(const DividedHisto1D& h, const std::initializer_list<double>& init, Int_t ktBins) :
    FemtoCorrFunc1D(h, init, ktBins, "kT", "k_{T} [GeV/c]", "k_{T} [GeV/c];N_{pairs}") {}

  Double_t FemtoCorrFuncKt::GetPairValDenRot(Hal::FemtoPair* pair) const {
    Double_t px = pair->GetPx1() - pair->GetPx2();
    Double_t py = pair->GetPy1() - pair->GetPy2();
    return TMath::Sqrt(px * px + py * py) * 0.5;
  }

  Double_t FemtoCorrFuncKt::GetPairValDenHemi(Hal::FemtoPair* pair) const {
    Double_t px = pair->GetPx1() - pair->GetPx2();
    Double_t py = pair->GetPy1() - pair->GetPy2();
    return TMath::Sqrt(px * px + py * py) * 0.5;
  }

  Double_t FemtoCorrFuncKt::GetPairValNum(Hal::FemtoPair* pair) const {
    Double_t px = pair->GetPx1() + pair->GetPx2();
    Double_t py = pair->GetPy1() + pair->GetPy2();
    return TMath::Sqrt(px * px + py * py) * 0.5;
  }

}  // namespace Hal
