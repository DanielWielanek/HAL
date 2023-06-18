/*
 * Field.cxx
 *
 *  Created on: 5 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "MagField.h"
namespace Hal {
  MagField::MagField() {
    // TODO Auto-generated constructor stub
  }

  MagField::~MagField() {
    // TODO Auto-generated destructor stub
  }

  void MagField::GetFieldValue(const Double_t point[3], Double_t* bField) const {
    auto field = GetField(point[0], point[1], point[2]);
    bField[0]  = field.X();
    bField[1]  = field.Y();
    bField[2]  = field.Z();
  }

}  // namespace Hal
