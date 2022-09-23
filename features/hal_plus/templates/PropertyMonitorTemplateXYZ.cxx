/*
 * PropertyMonitorTemplateXYZ.cxx
 *
 *  Created on: 23 wrz 2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "PropertyMonitorTemplateXYZ.h"

namespace MyHal {}
PropertyMonitorTemplateXYZ::PropertyMonitorTemplateXYZ() : PropertyMonitorXYZ(__XLABEL__, __YLABEL__, __ZLABEL__, __UPDATE__) {}

PropertyMonitorTemplateXYZ::PropertyMonitorTemplateXYZ(std::initializer_list<Double_t> x,
                                                       std::initializer_list<Double_t> y,
                                                       std::initializer_list<Double_t> z) {
  SetAxisList(x, 'x');
  SetAxisList(y, 'y');
  SetAxisList(z, 'z');
}

void PropertyMonitorTemplateXYZ::Update(Bool_t passed, TObject* obj) {
  __UPDATE_METHOD__
  if (passed) {
    fHistoPassed->Fill(__XYZ__);
  } else {
    fHistoFailed->Fill(__XYX__)
  }
}

Bool_t PropertyMonitorTemplateXYZ::Init(Int_t task_id) { return kTRUE; }

PropertyMonitorTemplateXYZ::~PropertyMonitorTemplateXYZ() {}
}
