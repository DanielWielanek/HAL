/*
 * CutMonitorEtaPt.h
 *
 *  Created on: 15 sty 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_CUTMONITORS_CUSTOMIZED_CUTMONITORETAPT_H_
#define HAL_CUTS_CUTMONITORS_CUSTOMIZED_CUTMONITORETAPT_H_

#include "PropertyMonitorXY.h"

namespace Hal {

  class CutMonitorEtaPt : public PropertyMonitorXY {
  public:
    CutMonitorEtaPt();
    virtual void Update(Bool_t passed, TObject* obj);
    virtual ~CutMonitorEtaPt() {};
    ClassDef(CutMonitorEtaPt, 1)
  };
} /* namespace Hal */

#endif /* HAL_CUTS_CUTMONITORS_CUSTOMIZED_CUTMONITORETAPT_H_ */
