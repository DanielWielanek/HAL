/*
 * FemtoSourceModelNumerical1D.cxx
 *
 *  Created on: 23 sie 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoSourceModelNumerical1D.h"

#include "Package.h"

#include <TH1D.h>
#include <iostream>
namespace Hal {
  FemtoSourceModelNumerical1D::FemtoSourceModelNumerical1D() : fRawDistribution(nullptr), ff(0) {
    fDensity = new FemtoSourceDensityNumerical1D();
  }

  FemtoSourceModelNumerical1D::FemtoSourceModelNumerical1D(const FemtoSourceModelNumerical1D& other) :
    FemtoSourceModel1D(other), fRawDistribution(nullptr), ff(other.ff) {
    if (other.fRawDistribution) fRawDistribution = (TH1D*) other.fRawDistribution->Clone();
    fRandomDistributionX = other.fRandomDistributionX;
    fRandomDistributionY = other.fRandomDistributionY;
  }

  void FemtoSourceModelNumerical1D::SetRadiusDistribution(const TH1D& distribution) {
    if (fRawDistribution) delete fRawDistribution;
    fRandomDistributionX.clear();
    fRandomDistributionY.clear();
    fRawDistribution = (TH1D*) distribution.Clone();
    fRawDistribution->SetDirectory(nullptr);
    Double_t scale = 0;
    fRawDistribution->SetBinContent(0, 0);
    ff = fRawDistribution->GetXaxis()->GetBinWidth(1);
    for (int i = 1; i <= fRawDistribution->GetNbinsX(); i++) {
      scale += fRawDistribution->GetBinContent(i);
      fRandomDistributionX.push_back(fRawDistribution->GetXaxis()->GetBinCenter(i));
    }
    fRawDistribution->Scale(1.0 / (scale * ff));
    for (int i = 1; i <= fRawDistribution->GetNbinsX(); i++) {
      Double_t val = fRawDistribution->GetBinContent(i);
      if (i > 1) val += fRandomDistributionY[fRandomDistributionY.size() - 1];
      fRandomDistributionY.push_back(val);
    }
    ((FemtoSourceDensityNumerical1D*) fDensity)->SetRadiusDistribution(*fRawDistribution);
  }

  FemtoSourceModel* FemtoSourceModelNumerical1D::MakeCopy() const { return new FemtoSourceModelNumerical1D(*this); }

  void FemtoSourceModelNumerical1D::GenerateCoordinates(FemtoPair* /*Pair*/) {
    Double_t randVal = gRandom->Rndm();
    Int_t i          = -1;
    while (randVal > fRandomDistributionY[i])
      i++;
    Double_t R = fRandomDistributionX[i] + ff * (gRandom->Rndm() - 0.5);
    gRandom->Sphere(fRout, fRside, fRlong, R);
  }

  Package* FemtoSourceModelNumerical1D::Report() const {
    Package* pack = FemtoSourceModel1D::Report();
    pack->AddObject(fRawDistribution);
    return pack;
  }

  FemtoSourceModelNumerical1D::~FemtoSourceModelNumerical1D() {
    if (fRawDistribution) delete fRawDistribution;
  }

  void FemtoSourceDensityNumerical1D::SetRadiusDistribution(const TH1D& distribution) {
    if (fRawDistribution) delete fRawDistribution;
    fRawDistribution = (TH1D*) distribution.Clone();
    fRawDistribution->SetDirectory(nullptr);
  }

  Double_t FemtoSourceDensityNumerical1D::GetProbDensity1d(const Double_t r, const Double_t* /*params*/) const {
    Int_t bin = fRawDistribution->GetXaxis()->FindBin(r);
    return fRawDistribution->GetBinContent(bin);
  }

  Double_t FemtoSourceDensityNumerical1D::GetProbDensity3d(const TVector3& r, const Double_t* params) const {
    return GetProbDensity1d(r.Mag(), params);
  }

}  // namespace Hal
