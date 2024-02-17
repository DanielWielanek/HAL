/*
 * FemtoSourceModelPseudo3d.cxx
 *
 *  Created on: 3 sty 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoSourceModelNumericalPseudo3d.h"
#include "Cout.h"

#include <TH1D.h>
#include <iostream>

namespace Hal {
  FemtoSourceModelNumericalPseudo3d::FemtoSourceModelNumericalPseudo3d() { fDensity = new FemtoSourceDensityNumericalPseudo3d(); }

  FemtoSourceModelNumericalPseudo3d::FemtoSourceModelNumericalPseudo3d(const FemtoSourceModelNumericalPseudo3d& other) :
    FemtoSourceModel3D(other) {
    if (other.fRawDistribution) this->fRawDistribution = (TH1D*) other.fRawDistribution->Clone();
  }

  void FemtoSourceModelNumericalPseudo3d::SetRadiusDistribution(const TH1D& distribution) {
    if (fRawDistribution) delete fRawDistribution;
    fRawDistribution = (TH1D*) distribution.Clone();
    fRawDistribution->SetDirectory(nullptr);
    Double_t scale = 0;
    fRawDistribution->SetBinContent(0, 0);
    ((FemtoSourceDensityNumericalPseudo3d*) fDensity)->SetRadiusDistribution(*fRawDistribution);
  }

  void FemtoSourceModelNumericalPseudo3d::GenerateCoordinates(FemtoPair* Pair) {
    fRout    = fRawDistribution->GetRandom();
    fRside   = fRawDistribution->GetRandom();
    fRlong   = fRawDistribution->GetRandom();
    int swap = gRandom->Integer(8);
    switch (swap) {
      case 0: {
        // do nothing
      } break;
      case 1: {
        fRout = -fRout;
      } break;
      case 2: {
        fRside = -fRside;
      } break;
      case 3: {
        fRout  = -fRout;
        fRside = -fRside;
      } break;
      case 4: {
        fRlong = -fRlong;
      } break;
      case 5: {
        fRout  = -fRout;
        fRlong = -fRlong;
      } break;
      case 6: {
        fRside = -fRside;
        fRlong = -fRlong;
      } break;
      case 7: {
        fRout  = -fRout;
        fRside = -fRside;
        fRlong = -fRlong;
      } break;
    }
  }

  FemtoSourceModelNumericalPseudo3d::~FemtoSourceModelNumericalPseudo3d() {
    if (fRawDistribution) delete fRawDistribution;
  }

  Double_t FemtoSourceDensityNumericalPseudo3d::GetProbDensity3d(const TVector3& r, const Double_t* params) const {
    auto findDens = [&](Double_t v) { return int((TMath::Abs(v) - fMin) * fDx + 1); };
    double rho    = fRawDistribution->GetBinContent(findDens(r.X()));
    rho *= fRawDistribution->GetBinContent(findDens(r.Y()));
    rho *= fRawDistribution->GetBinContent(findDens(r.Z()));
    return rho;
  }

  void FemtoSourceDensityNumericalPseudo3d::SetRadiusDistribution(const TH1D& distribution) {
    if (fRawDistribution) delete fRawDistribution;

    fRawDistribution = (TH1D*) distribution.Clone();
    fRawDistribution->SetDirectory(nullptr);
    fRawDistribution->SetBinContent(0, 0);
    fDx             = fRawDistribution->GetBinWidth(1);
    fDx             = 1.0 / fDx;
    fMin            = fRawDistribution->GetBinLowEdge(1);
    double integral = fRawDistribution->Integral("width");
    fRawDistribution->Scale(1.0 / integral);
  }

  FemtoSourceDensityNumericalPseudo3d::FemtoSourceDensityNumericalPseudo3d(const FemtoSourceDensityNumericalPseudo3d& other) :
    FemtoSourceDensity(other) {
    if (other.fRawDistribution) fRawDistribution = (TH1D*) other.fRawDistribution;
    fMin = other.fMin;
    fDx  = other.fDx;
  }

  FemtoSourceDensityNumericalPseudo3d::~FemtoSourceDensityNumericalPseudo3d() {
    if (fRawDistribution) delete fRawDistribution;
  }
}  // namespace Hal
