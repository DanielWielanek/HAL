/*
 * HalFairField.cxx
 *
 *  Created on: 24 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "HalFairField.h"

#include <FairField.h>

namespace Hal {

  HalFairField::HalFairField() : fField(nullptr) {}
  TVector3 HalFairField::GetField(Double_t x, Double_t y, Double_t z) const {
    return TVector3(fField->GetBx(x, y, z), fField->GetBy(x, y, z), fField->GetBz(x, y, z));
  }
  HalFairField::~HalFairField() { fField = nullptr; }

} /* namespace Hal */
