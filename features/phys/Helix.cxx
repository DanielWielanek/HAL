/*
 * HalHelixBase.cxx
 *
 *  Created on: 3 sty 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "Helix.h"

#include <TMath.h>
#include <iostream>
namespace Hal {
  HelixZ::HelixZ() : HelixBase() {}

  HelixZ::HelixZ(const TVector3& x, const TVector3& mom, Double_t charge, Double_t conversion) :
    HelixBase(x, mom, charge, conversion) {}

  void HelixZ::Shift(Double_t x, Double_t y, Double_t z) { BaseShift(x, y, z); }

  HelixZ& HelixZ::operator=(const HelixZ& helix) {
    HelixBase::operator=(helix);
    return *this;
  }

  HelixX::HelixX() : HelixBase() {}

  HelixX::HelixX(const TVector3& x, const TVector3& mom, Double_t charge, Double_t conversion) :
    HelixBase(x.Y(), x.Z(), x.X(), mom.Y(), mom.Z(), mom.X(), charge, conversion) {}

  void HelixX::Shift(Double_t x, Double_t y, Double_t z) { BaseShift(y, z, x); }

  HelixX& HelixX::operator=(const HelixX& helix) {
    HelixBase::operator=(helix);
    return *this;
  }

  HelixY::HelixY() : HelixBase() {}

  HelixY::HelixY(const TVector3& x, const TVector3& mom, Double_t charge, Double_t conversion) :
    HelixBase(x.Z(), x.X(), x.Y(), mom.Z(), mom.X(), mom.Y(), charge, conversion) {}

  HelixY& HelixY::operator=(const HelixY& helix) {
    HelixBase::operator=(helix);
    return *this;
  }

  void HelixY::Shift(Double_t x, Double_t y, Double_t z) { BaseShift(z, x, y); }
}  // namespace Hal
