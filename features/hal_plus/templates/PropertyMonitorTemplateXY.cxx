/*
 * PropertyMonitorTemplateXY.cxx
 *
 *  Created on: 23 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "PropertyMonitorTemplateXY.h"

namespace MyHal {
  PropertyMonitorTemplateXY::PropertyMonitorTemplateXY() : PropertyMonitorXY(__XLABEL__, __YLABEL__, __UPDATE__) {}

  PropertyMonitorTemplateXY::PropertyMonitorTemplateXY(std::initializer_list<Double_t> x, std::initializer_list<Double_t> y) :
    PropertyMonitorTemplateXY() {
    SetAxisList(x, 'x');
    SetAxisList(y, 'y');
  }

  void PropertyMonitorTemplateXY::Update(Bool_t passed, TObject* obj) {
    __UPDATE_METHOD__
    if (passed) {
      fHistoPassed->Fill(__XYZ__);
    } else {
      fHistoFailed->Fill(__XYX__)
    }
  }

  Bool_t PropertyMonitorTemplateXY::Init(Int_t task_id) { return kTRUE; }

  PropertyMonitorTemplateXY::~PropertyMonitorTemplateXY() {}

}  // namespace MyHal
