/*
 * HalFairField.h
 *
 *  Created on: 24 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FAIR_FIELD_H_
#define HAL_FAIR_FIELD_H_

#include "MagField.h"

#include <TVector3.h>

/**
 * interface to Fair magnetic field
 */
class FairField;
namespace Hal {
  namespace Fair {
    class Field : public ::Hal::MagField {
      FairField* fField;

    public:
      Field();
      void SetField(FairField* field) { fField = field; };
      FairField* GetFairField() const { return fField; }
      TVector3 GetField(Double_t x, Double_t y, Double_t z) const;
      virtual ~Field();
      ClassDef(Field, 1)
    };
  }  // namespace Fair
} /* namespace Hal */

#endif /* HAL_FAIR_FIELD_H_ */
