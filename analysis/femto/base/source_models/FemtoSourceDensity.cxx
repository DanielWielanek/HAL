/*
 * FemtoSourceDensity.cxx
 *
 *  Created on: 3 sty 2023
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoSourceDensity.h"


namespace Hal {

  FemtoSourceDensity::FemtoSourceDensity(const FemtoSourceDensity& other) {
    fNParams = other.fNParams;
    fAna1d   = other.fAna1d;
    fAna3d   = other.fAna3d;
    if (fNParams) {
      fParams = new Double_t[fNParams];
      for (int i = 0; i < fNParams; i++) {
        fParams[i] = other.fParams[i];
      }
    } else {
      fParams = nullptr;
    }
  }

  FemtoSourceDensity& FemtoSourceDensity::operator=(const FemtoSourceDensity& other) {
    if (&other == this) return *this;
    if (other.fNParams != fNParams) return *this;
    for (int i = 0; i < fNParams; i++) {
      fParams[i] = other.fParams[i];
    }
    return *this;
  }

  void FemtoSourceDensity::SetBaseParameters(const Double_t* params) {
    for (int i = 0; i < fNParams; i++) {
      fParams[i] = params[i];
    }
  }

  Double_t FemtoSourceDensity::GetRelativeProbDensity3d(const TVector3& vec, const Double_t* par) {
    return GetProbDensity3d(vec, par) / GetProbDensity3d(vec, fParams);
  }

  Double_t FemtoSourceDensity::GetRelativeProbDensity1d(const Double_t r, const Double_t* params) const {
    return GetProbDensity1d(r, params) / GetProbDensity1d(r, fParams);
  }

}  // namespace Hal
