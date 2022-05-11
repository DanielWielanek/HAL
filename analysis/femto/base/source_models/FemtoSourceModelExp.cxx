/*
 * FemtoSourceModelExp.cxx
 *
 *  Created on: 1 mar 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoSourceModelExp.h"

namespace Hal {
  FemtoSourceModelExp1D::FemtoSourceModelExp1D(const FemtoSourceModelExp1D& model) : FemtoSourceModel1D(model) {}

  FemtoSourceModel* FemtoSourceModelExp1D::MakeCopy() const { return new FemtoSourceModelExp1D(*this); }

  void FemtoSourceModelExp1D::GenerateCoordinates(FemtoPair* /*Pair*/) {
    fRout  = fRandom->Exp(GetParameter(0));
    fRside = fRandom->Exp(GetParameter(0));
    fRlong = fRandom->Exp(GetParameter(0));
    if (fRandom->Rndm() > 0.5) { fRout = -fRout; }
    if (fRandom->Rndm() > 0.5) { fRside = -fRside; }
    if (fRandom->Rndm() > 0.5) { fRlong = -fRlong; }
  }

  FemtoSourceModelExp1D::FemtoSourceModelExp1D() : FemtoSourceModel1D() { fModelName = "gaus1d"; }

  FemtoSourceModelExp1D::~FemtoSourceModelExp1D() {}
}  // namespace Hal
