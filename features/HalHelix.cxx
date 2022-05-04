/*
 * HalHelixBase.cxx
 *
 *  Created on: 3 sty 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "HalHelix.h"

#include <TMath.h>
#include <iostream>

HalHelixZ::HalHelixZ() : HalHelixBase() {}

HalHelixZ::HalHelixZ(const TVector3& x, const TVector3& mom, Double_t charge, Double_t conversion) :
  HalHelixBase(x, mom, charge, conversion) {}

void HalHelixZ::Shift(Double_t x, Double_t y, Double_t z) { BaseShift(x, y, z); }

HalHelixZ& HalHelixZ::operator=(const HalHelixZ& helix) {
  HalHelixBase::operator=(helix);
  return *this;
}

HalHelixX::HalHelixX() : HalHelixBase() {}

HalHelixX::HalHelixX(const TVector3& x, const TVector3& mom, Double_t charge, Double_t conversion) :
  HalHelixBase(x.Y(), x.Z(), x.X(), mom.Y(), mom.Z(), mom.X(), charge, conversion) {}

void HalHelixX::Shift(Double_t x, Double_t y, Double_t z) { BaseShift(y, z, x); }

HalHelixX& HalHelixX::operator=(const HalHelixX& helix) {
  HalHelixBase::operator=(helix);
  return *this;
}

HalHelixY::HalHelixY() : HalHelixBase() {}

HalHelixY::HalHelixY(const TVector3& x, const TVector3& mom, Double_t charge, Double_t conversion) :
  HalHelixBase(x.Z(), x.X(), x.Y(), mom.Z(), mom.X(), mom.Y(), charge, conversion) {}

HalHelixY& HalHelixY::operator=(const HalHelixY& helix) {
  HalHelixBase::operator=(helix);
  return *this;
}

void HalHelixY::Shift(Double_t x, Double_t y, Double_t z) { BaseShift(z, x, y); }
