/*
 * PropertyMonitorTemplateXY.h
 *
 *  Created on: 23 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_CMAKE_TEMPLATES_PROPERTYMONITORTEMPLATEXY_H_
#define HAL_CMAKE_TEMPLATES_PROPERTYMONITORTEMPLATEXY_H_

#include <Hal/PropertyMonitorXY.h>

namespace MyHal {
  class PropertyMonitorTemplateXY : public Hal::PropertyMonitorXY {
  public:
    PropertyMonitorTemplateXY();
    PropertyMonitorTemplateXY(std::initializer_list<Double_t> x, std::initializer_list<Double_t> y);
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Bool_t Init(Int_t task_id);
    virtual ~PropertyMonitorTemplateXY();
    ClassDef(PropertyMonitorTemplateXY, 1)
  };
}  // namespace MyHal
#endif /* HAL_FEATURES_HAL_PLUS_TEMPLATES_PROPERTYMONITORTEMPLATEXY_H_ */
