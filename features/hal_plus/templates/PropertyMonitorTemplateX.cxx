/*
 * PropertyMonitorTemplateX.cxx
 *
 *  Created on: 23 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "PropertyMonitorTemplateX.h"

namespace MyHal {

  PropertyMonitorTemplateX::PropertyMonitorTemplateX() : PropertyMonitorX(__XLABEL__, __YLABEL__, __UPDATE__) {}

  PropertyMonitorTemplateX::PropertyMonitorTemplateX(std::initializer_list<Double_t> x) : PropertyMonitorTemplateX() {
    SetAxisList(x, 'x');
  }

  void PropertyMonitorTemplateX::Update(Bool_t passed, TObject* obj) {
    __UPDATE_METHOD__
    if (passed) {
      fHistoPassed->Fill(__XYZ__);
    } else {
      fHistoFailed->Fill(__XYX__)
    }
  }

  Bool_t PropertyMonitorTemplateX::Init(Int_t task_id) { return kTRUE; }

  PropertyMonitorTemplateX::~PropertyMonitorTemplateX() {}

}  // namespace MyHal
