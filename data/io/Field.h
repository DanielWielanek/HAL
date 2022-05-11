/*
 * Field.h
 *
 *  Created on: 5 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_BASE_STEER_HALFIELD_H_
#define HAL_ANALYSIS_BASE_STEER_HALFIELD_H_

#include <TObject.h>
#include <TVector3.h>

namespace Hal {
  class Field : public TObject {
  public:
    Field();
    TVector3 GetField(Double_t x, Double_t y, Double_t z) const { return TVector3(0, 0, 0); }
    virtual ~Field();
    ClassDef(Field, 1)
  };
}  // namespace Hal
#endif /* HAL_ANALYSIS_BASE_STEER_HALFIELD_H_ */
