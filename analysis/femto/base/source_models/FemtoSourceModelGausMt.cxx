/*
 * FemtoSourceModelGausKt.cxx
 *
 *  Created on: 1 mar 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoSourceModelGausMt.h"

#include "Parameter.h"

namespace Hal {
  FemtoSourceModelGausMt::FemtoSourceModelGausMt() : FemtoSourceModel3D(7) {
    SetOutParams(1, 1);
    SetSideParams(1, 1);
    SetLongParams(1, 1);
    SetM2(0);
    SetParName(3, "R_out^A");
    SetParName(4, "R_side^A");
    SetParName(5, "R_long^A");
    SetParName(6, "m^{2}");
  }

  FemtoSourceModelGausMt::FemtoSourceModelGausMt(const FemtoSourceModelGausMt& model) : FemtoSourceModel3D(model) {}

  void FemtoSourceModelGausMt::SetOutParams(Double_t a, Double_t b) {
    SetParameter(a, 0);
    SetParameter(b, 3);
  }

  void FemtoSourceModelGausMt::SetSideParams(Double_t a, Double_t b) {
    SetParameter(a, 1);
    SetParameter(b, 4);
  }

  void FemtoSourceModelGausMt::SetLongParams(Double_t a, Double_t b) {
    SetParameter(a, 2);
    SetParameter(b, 5);
  }

  void FemtoSourceModelGausMt::SetM2(Double_t par) { SetParameter(par, 6); }

  FemtoSourceModel* FemtoSourceModelGausMt::MakeCopy() const { return new FemtoSourceModelGausMt(*this); }

  void FemtoSourceModelGausMt::GenerateCoordinates(FemtoPair* pair) {
    Double_t tpx = pair->GetPx1() + pair->GetPx2();
    Double_t tpy = pair->GetPy1() + pair->GetPy2();
    Double_t kt2 = (tpx * tpx + tpy * tpy) * 0.25;
    if (GetParameter(6) == 0.0) { SetParameter(pair->GetM1() * pair->GetM2(), 6); }
    Double_t tm = 1.0 / TMath::Sqrt(kt2 + GetParameter(6));
    if (TMath::IsNaN(tm)) tm = 100;
    double fSetRout  = GetParameter(0) * (tm + GetParameter(3));
    double fSetRside = GetParameter(1) * (tm + GetParameter(4));
    double fSetRlong = GetParameter(2) * (tm + GetParameter(5));

    fRout  = fRandom->Gaus(0, fSetRout * TMath::Sqrt2());
    fRside = fRandom->Gaus(0, fSetRside * TMath::Sqrt2());
    fRlong = fRandom->Gaus(0, fSetRlong * TMath::Sqrt2());
  }

  Package* FemtoSourceModelGausMt::Report() const {
    Package* pack = FemtoSourceModel::Report();
    return pack;
  }

  FemtoSourceModelGausMt::~FemtoSourceModelGausMt() {
    // TODO Auto-generated destructor stub
  }
}  // namespace Hal
