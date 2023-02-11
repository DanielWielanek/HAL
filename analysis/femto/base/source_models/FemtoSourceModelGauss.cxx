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

#include <TDecompChol.h>
#include <TMath.h>
#include <TMatrix.h>
#include <TRandom.h>

namespace Hal {
  FemtoSourceModelGauss1D::FemtoSourceModelGauss1D() : FemtoSourceModel1D() {
    fModelName = "gaus1d";
    fDensity   = new FemtoSourceDensityGaus1d();
  }

  FemtoSourceModelGauss1D::FemtoSourceModelGauss1D(const FemtoSourceModelGauss1D& model) : FemtoSourceModel1D(model) {}

  FemtoSourceModel* FemtoSourceModelGauss1D::MakeCopy() const { return new FemtoSourceModelGauss1D(*this); }

  void FemtoSourceModelGauss1D::GenerateCoordinates(FemtoPair* /*Pair*/) {
    fRout  = fRandom->Gaus(0, GetParameter(0) * TMath::Sqrt2());
    fRside = fRandom->Gaus(0, GetParameter(0) * TMath::Sqrt2());
    fRlong = fRandom->Gaus(0, GetParameter(0) * TMath::Sqrt2());
  }

  FemtoSourceModelGauss1D::~FemtoSourceModelGauss1D() {}

  //=========================== 3D ====================

  FemtoSourceModelGauss3D::FemtoSourceModelGauss3D() : FemtoSourceModel3D() {
    fModelName = "gaus3d";
    fDensity   = new FemtoSourceDensityGaus3d();
  }

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

  //=================================================

  FemtoSourceDensityGaus1d::FemtoSourceDensityGaus1d() : FemtoSourceDensity(1, kTRUE, kTRUE) {}

  Double_t FemtoSourceDensityGaus1d::GetProbDensity1d(const Double_t r, const Double_t* params) const {
    if (r < 0) return 0;
    Double_t u          = params[0];
    const Double_t Norm = 2. * Const::SqrtPi() * u * u * u;  // TMath::Power(u * u, 1.5);
    return TMath::Exp(-r * r / (4.0 * u * u)) / Norm * r * r;
  }

  Double_t FemtoSourceDensityGaus1d::GetProbDensity3d(const TVector3& vec, const Double_t* params) const {
    const Double_t sxsysz = params[0] * params[0] * params[0];
    const Double_t Gx     = TMath::Gaus(vec.X(), 0, params[0] * TMath::Sqrt2());
    const Double_t Gy     = TMath::Gaus(vec.Y(), 0, params[0] * TMath::Sqrt2());
    const Double_t Gz     = TMath::Gaus(vec.Z(), 0, params[0] * TMath::Sqrt2());
    return 0.02244839026564582 / sxsysz * Gx * Gy * Gz;
  }

  //=================================================
  FemtoSourceDensityGaus3d::FemtoSourceDensityGaus3d() : FemtoSourceDensity(3, kTRUE, kTRUE) {}

  Double_t FemtoSourceDensityGaus3d::GetProbDensity1d(const Double_t r, const Double_t* params) const {
    if (r < 0) return 0;
    Double_t u          = TMath::Power(params[0] * params[1] * params[2], 1.0 / 3.0);
    const Double_t Norm = 2. * Const::SqrtPi() * u * u * u;  // TMath::Power(u * u, 1.5);
    return TMath::Exp(-r * r / (4.0 * u * u)) / Norm * r * r;
  }

  Double_t FemtoSourceDensityGaus3d::GetProbDensity3d(const TVector3& vec, const Double_t* params) const {
    const Double_t sxsysz = params[0] * params[1] * params[2];  // sqt?
    const Double_t Gx     = TMath::Gaus(vec.X(), 0, params[0] * TMath::Sqrt2());
    const Double_t Gy     = TMath::Gaus(vec.Y(), 0, params[1] * TMath::Sqrt2());
    const Double_t Gz     = TMath::Gaus(vec.Z(), 0, params[2] * TMath::Sqrt2());
    return 0.02244839026564582 / sxsysz * Gx * Gy * Gz;
  }

  //=================================================
  FemtoSourceModelGauss3DCross::FemtoSourceModelGauss3DCross() : FemtoSourceModel3D(6), fAMatrix(3, 3), fCovMatrix(3, 3) {
    SetParameter(1, 3);
    SetParameter(1, 4);
    SetParameter(1, 5);
    SetParName("R_{out-side}", 3);
    SetParName("R_{out-long}", 4);
    SetParName("R_{side-long}", 5);
  }

  FemtoSourceModelGauss3DCross::FemtoSourceModelGauss3DCross(const FemtoSourceModelGauss3DCross& model) :
    FemtoSourceModel3D(model), fAMatrix(3, 3), fCovMatrix(3, 3) {
    fAMatrix   = model.fAMatrix;
    fCovMatrix = model.fCovMatrix;
    for (int i = 0; i < 3; i++) {
      fRowA[i] = model.fRowA[i];
      fRowB[i] = model.fRowB[i];
      fRowC[i] = model.fRowC[i];
    }
  }

  FemtoSourceModel* FemtoSourceModelGauss3DCross::MakeCopy() const { return new FemtoSourceModelGauss3DCross(*this); }

  void FemtoSourceModelGauss3DCross::GenerateCoordinates(FemtoPair* Pair) {
    // fRout       = fRandom->Gaus(0, GetParameter(0) * TMath::Sqrt2());
    // fRside      = fRandom->Gaus(0, GetParameter(1) * TMath::Sqrt2());
    // fRlong      = fRandom->Gaus(0, GetParameter(2) * TMath::Sqrt2());
    Double_t Z1 = fRandom->Gaus(0, TMath::Sqrt2());
    Double_t Z2 = fRandom->Gaus(0, TMath::Sqrt2());
    Double_t Z3 = fRandom->Gaus(0, TMath::Sqrt2());
    fRout       = Z1 * fRowA[0] + Z2 * fRowA[1] + Z3 * fRowA[2];
    fRside      = Z1 * fRowB[0] + Z2 * fRowB[1] + Z3 * fRowB[2];
    fRlong      = Z1 * fRowC[0] + Z2 * fRowC[1] + Z3 * fRowC[2];
  }

  Bool_t FemtoSourceModelGauss3DCross::Init() {
    TMatrixD sigma(3, 3);
    sigma[0][0] = GetParameter(0);  // out out
    sigma[0][1] = GetOutSide();     // out side
    sigma[0][2] = GetOutLong();
    sigma[1][0] = GetOutSide();
    sigma[1][1] = GetParameter(1);
    sigma[1][2] = GetSideLong();
    sigma[2][0] = GetOutLong();
    sigma[2][1] = GetSideLong();
    sigma[2][2] = GetParameter(2);
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        sigma[i][j] = sigma[i][j] * sigma[i][j];
      }
    }
    sigma = sigma.Invert();


    TDecompChol chol(sigma);
    if (!chol.Decompose()) return kFALSE;
    fAMatrix = chol.GetU();
    for (int i = 0; i < 3; i++) {
      fRowA[i] = fAMatrix[i][0];
      fRowB[i] = fAMatrix[i][1];
      fRowC[i] = fAMatrix[i][2];
    }
    return kFALSE;
  }

}  // namespace Hal
