/*
 * PropertyMonitorTemplateX.h
 *
 *  Created on: 23 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_CMAKE_TEMPLATES_PROPERTYMONITORTEMPLATEX_H_
#define HAL_CMAKE_TEMPLATES_PROPERTYMONITORTEMPLATEX_H_

#include <Hal/PropertyMonitorX.h>
namespace MyHal {
  class PropertyMonitorTemplateX : public Hal::PropertyMonitorX {
  public:
    PropertyMonitorTemplateX();
    PropertyMonitorTemplateX(std::initializer_list<Double_t> x);
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Bool_t Init(Int_t task_id);
    virtual ~PropertyMonitorTemplateX();
    ClassDef(PropertyMonitorTemplateX, 1)
  };
}  // namespace MyHal

#endif /* HAL_FEATURES_HAL_PLUS_TEMPLATES_PROPERTYMONITORTEMPLATEX_H_ */
