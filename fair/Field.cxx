/*
 * HalFairField.cxx
 *
 *  Created on: 24 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "Field.h"

#include <FairField.h>

namespace Hal {
  namespace Fair {

    Field::Field() : fField(nullptr) {}

    TVector3 Field::GetField(Double_t x, Double_t y, Double_t z) const {
      return TVector3(fField->GetBx(x, y, z), fField->GetBy(x, y, z), fField->GetBz(x, y, z));
    }
    Field::~Field() { fField = nullptr; }
  }  // namespace Fair
} /* namespace Hal */
