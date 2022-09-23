/*
 * PropertyMonitorTemplateXYZ.h
 *
 *  Created on: 23 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_CMAKE_TEMPLATES_PROPERTYMONITORTEMPLATEXYZ_H_
#define HAL_CMAKE_TEMPLATES_PROPERTYMONITORTEMPLATEXYZ_H_

#include <Hal/PropertyMonitorXYZ.h>

namespace MyHal {
  class PropertyMonitorTemplateXYZ : public Hal::PropertyMonitorXYZ {
  public:
    PropertyMonitorTemplateXYZ();
    PropertyMonitorTemplateXYZ(std::initializer_list<Double_t> x,
                               std::initializer_list<Double_t> y,
                               std::initializer_list<Double_t> z);
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Bool_t Init(Int_t task_id);
    virtual ~PropertyMonitorTemplateXYZ();
    ClassDef(PropertyMonitorTemplateXYZ, 1)
  };
}  // namespace MyHal
#endif /* HAL_FEATURES_HAL_PLUS_TEMPLATES_PROPERTYMONITORTEMPLATEXYZ_H_ */
