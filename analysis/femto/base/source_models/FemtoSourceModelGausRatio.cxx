/*
 * FemtoSourceModelGaus3DRelative.cxx
 *
 *  Created on: 15 paź 2024
 *      Author: daniel
 */

#include "FemtoSourceModelGausRatio.h"
#include "FemtoSourceModelGauss.h"

#include "Const.h"
#include "Cout.h"

#include <TDecompChol.h>
#include <TMath.h>
#include <TMatrix.h>
#include <TRandom.h>
namespace Hal {
  FemtoSourceModelGaussDelta::FemtoSourceModelGaussDelta() {
    fModelName = "gaus3draio";
    fDensity   = new FemtoSourceDensityGausDelta();
    SetParName(0, "R_{out}");
    SetParName(3, "R_{out}");
    SetParName(4, "R_{#Delta side}");
    SetParName(5, "R_{#times long}");
  }

  FemtoSourceModelGaussDelta::FemtoSourceModelGaussDelta(const FemtoSourceModelGaussDelta& model) : FemtoSourceModel3D(model) {}

  FemtoSourceModel* FemtoSourceModelGaussDelta::MakeCopy() const { return new FemtoSourceModelGaussDelta(*this); }

  void FemtoSourceModelGaussDelta::GenerateCoordinates(FemtoPair* Pair) {
    fRout  = fRandom->Gaus(0, GetParameter(0) * TMath::Sqrt2());
    fRside = fRandom->Gaus(0, (GetParameter(1) + GetParameter(0)) * TMath::Sqrt2());
    fRlong = fRandom->Gaus(0, (GetParameter(2) + GetParameter(0)) * TMath::Sqrt2());
  }

  FemtoSourceModelGaussDelta::~FemtoSourceModelGaussDelta() {}

  FemtoSourceDensityGausDelta::FemtoSourceDensityGausDelta() {}

  Double_t FemtoSourceDensityGausDelta::GetProbDensity1d(const Double_t r, const Double_t* params) const {
    if (r < 0) return 0;
    Double_t rside      = params[1] + params[0];
    Double_t rlong      = params[2] + params[0];
    Double_t u          = TMath::Power(params[0] * rside * rlong, 1.0 / 3.0);
    const Double_t Norm = 2. * Const::SqrtPi() * u * u * u;  // TMath::Power(u * u, 1.5);
    return TMath::Exp(-r * r / (4.0 * u * u)) / Norm * r * r;
  }

  Double_t FemtoSourceDensityGausDelta::GetProbDensity3d(const TVector3& vec, const Double_t* params) const {
    const Double_t sxsysz = params[0] * (params[1] + params[0]) * (params[2] + params[0]);  // sqt?
    const Double_t Gx     = TMath::Gaus(vec.X(), 0, params[0] * TMath::Sqrt2());
    const Double_t Gy     = TMath::Gaus(vec.Y(), 0, (params[1] + params[0]) * TMath::Sqrt2());
    const Double_t Gz     = TMath::Gaus(vec.Z(), 0, (params[2] + params[0]) * TMath::Sqrt2());
    return 0.02244839026564582 / sxsysz * Gx * Gy * Gz;
  }

}  // namespace Hal
