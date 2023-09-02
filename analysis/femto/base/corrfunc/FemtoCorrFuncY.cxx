/*
 * FemtoCorrFuncY.cxx
 *
 *  Created on: 25 cze 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoCorrFuncY.h"

#include "Cout.h"
#include "FemtoPair.h"

#include <TCanvas.h>
#include <TROOT.h>

namespace Hal {

  FemtoCorrFuncY::FemtoCorrFuncY(const DividedHisto1D& h, const std::initializer_list<double>& init, Int_t yBins) :
    FemtoCorrFunc1D(h, init, yBins, "Y_{pair}", "Y_{pair} ", "N_{pairs}") {}

  Double_t FemtoCorrFuncY::GetPairValNum(FemtoPair* pair) const {
    Double_t e1  = pair->GetE1();
    Double_t e2  = pair->GetE2();
    Double_t pz1 = pair->GetPz1();
    Double_t pz2 = pair->GetPz2();
    Double_t E   = e1 + e2;
    Double_t Pz  = pz1 + pz2;
    return 0.5 * TMath::Log((E + Pz) / (E - Pz));
  }

  Double_t FemtoCorrFuncY::GetPairValDenRot(FemtoPair* pair) const { return GetPairValNum(pair); }

  Double_t FemtoCorrFuncY::GetPairValDenHemi(FemtoPair* pair) const {
    Double_t e1  = pair->GetE1();
    Double_t e2  = pair->GetE2();
    Double_t pz1 = pair->GetPz1();
    Double_t pz2 = pair->GetPz2();
    Double_t E   = e1 + e2;
    Double_t Pz  = pz1 - pz2;
    return 0.5 * TMath::Log((E + Pz) / (E - Pz));
  }

  FemtoCorrFuncY::FemtoCorrFuncY(const FemtoCorrFuncY& other) : FemtoCorrFunc1D(other) {}

}  // namespace Hal
