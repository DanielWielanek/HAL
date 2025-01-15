/*
 * CutMonitorKinematics.h
 *
 *  Created on: 10 sty 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_CUTMONITORS_CUSTOMIZED_CUTMONITORYPT_H_
#define HAL_CUTS_CUTMONITORS_CUSTOMIZED_CUTMONITORYPT_H_

#include "PropertyMonitorXY.h"

namespace Hal {

  class CutMonitorYPt : public PropertyMonitorXY {
  public:
    CutMonitorYPt();
    virtual void Update(Bool_t passed, TObject* obj);
    virtual ~CutMonitorYPt() {};
    ClassDef(CutMonitorYPt, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_CUTMONITORS_CUSTOMIZED_CUTMONITORYPT_H_ */
