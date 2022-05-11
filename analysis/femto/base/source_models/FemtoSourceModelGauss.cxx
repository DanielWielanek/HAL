/*
 * FemtoSourceModelGaus.cxx
 *
 *  Created on: 1 mar 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoSourceModelGauss.h"

#include "Const.h"

namespace Hal {
  FemtoSourceModelGauss1D::FemtoSourceModelGauss1D() : FemtoSourceModel1D() { fModelName = "gaus1d"; }

  FemtoSourceModelGauss1D::FemtoSourceModelGauss1D(const FemtoSourceModelGauss1D& model) : FemtoSourceModel1D(model) {}

  FemtoSourceModel* FemtoSourceModelGauss1D::MakeCopy() const { return new FemtoSourceModelGauss1D(*this); }

  void FemtoSourceModelGauss1D::GenerateCoordinates(FemtoPair* /*Pair*/) {
    fRout  = fRandom->Gaus(0, GetParameter(0) * TMath::Sqrt2());
    fRside = fRandom->Gaus(0, GetParameter(0) * TMath::Sqrt2());
    fRlong = fRandom->Gaus(0, GetParameter(0) * TMath::Sqrt2());
  }

  Double_t FemtoSourceModelGauss1D::GetProbDensity1d(const Double_t r, const Double_t* params) const {
    if (r < 0) return 0;
    Double_t u          = params[0];
    const Double_t Norm = 2. * Const::SqrtPi() * u * u * u;  // TMath::Power(u * u, 1.5);
    return TMath::Exp(-r * r / (4.0 * u * u)) / Norm * r * r;
  }

  Double_t FemtoSourceModelGauss1D::GetProbDensity3d(const TVector3& vec, const Double_t* params) const {
    const Double_t sxsysz = params[0] * params[0] * params[0];
    const Double_t Gx     = TMath::Gaus(vec.X(), 0, params[0] * TMath::Sqrt2());
    const Double_t Gy     = TMath::Gaus(vec.Y(), 0, params[0] * TMath::Sqrt2());
    const Double_t Gz     = TMath::Gaus(vec.Z(), 0, params[0] * TMath::Sqrt2());
    return 0.02244839026564582 / sxsysz * Gx * Gy * Gz;
  }

  FemtoSourceModelGauss1D::~FemtoSourceModelGauss1D() { fModelName = "exp1d"; }

  //=========================== 3D ====================

  FemtoSourceModelGauss3D::FemtoSourceModelGauss3D() : FemtoSourceModel3D() { fModelName = "gaus3d"; }

  void FemtoSourceModelGauss3D::GenerateCoordinates(FemtoPair* /*Pair*/) {
    fRout  = fRandom->Gaus(0, GetParameter(0) * TMath::Sqrt2());
    fRside = fRandom->Gaus(0, GetParameter(1) * TMath::Sqrt2());
    fRlong = fRandom->Gaus(0, GetParameter(2) * TMath::Sqrt2());
#ifdef _kinetic_debug
    std::cout << "radii " << fRout << " " << fRside << " " << fRlong << std::endl;
#endif
  }

  FemtoSourceModelGauss3D::FemtoSourceModelGauss3D(const FemtoSourceModelGauss3D& model) : FemtoSourceModel3D(model) {}

  FemtoSourceModel* FemtoSourceModelGauss3D::MakeCopy() const { return new FemtoSourceModelGauss3D(*this); }

  FemtoSourceModelGauss3D::~FemtoSourceModelGauss3D() {}

  Double_t FemtoSourceModelGauss3D::GetProbDensity1d(const Double_t r, const Double_t* params) const {
    // TODO check formula
    if (r < 0) return 0;
    Double_t u          = TMath::Power(params[0] * params[1] * params[2], 1.0 / 3.0);
    const Double_t Norm = 2. * Const::SqrtPi() * u * u * u;  // TMath::Power(u * u, 1.5);
    return TMath::Exp(-r * r / (4.0 * u * u)) / Norm * r * r;
  }

  Double_t FemtoSourceModelGauss3D::GetProbDensity3d(const TVector3& vec, const Double_t* params) const {
    const Double_t sxsysz = params[0] * params[1] * params[2];  // sqt?
    const Double_t Gx     = TMath::Gaus(vec.X(), 0, params[0] * TMath::Sqrt2());
    const Double_t Gy     = TMath::Gaus(vec.Y(), 0, params[1] * TMath::Sqrt2());
    const Double_t Gz     = TMath::Gaus(vec.Z(), 0, params[2] * TMath::Sqrt2());
    return 0.02244839026564582 / sxsysz * Gx * Gy * Gz;
  }
}  // namespace Hal
