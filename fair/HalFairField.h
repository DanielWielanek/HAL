/*
 * HalFairField.h
 *
 *  Created on: 24 maj 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_FAIR_HALFAIRFIELD_H_
#define HAL_FAIR_HALFAIRFIELD_H_

#include "Field.h"
class FairField;
namespace Hal {

  class HalFairField : public Field {
    FairField* fField;

  public:
    HalFairField();
    void SetField(FairField* field) { fField = field; };
    FairField* GetFairField() const { return fField; }
    TVector3 GetField(Double_t x, Double_t y, Double_t z) const;
    virtual ~HalFairField();
    ClassDef(HalFairField, 1)
  };

} /* namespace Hal */

#endif /* HAL_FAIR_HALFAIRFIELD_H_ */
