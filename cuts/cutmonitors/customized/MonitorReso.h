/*
 * CutMonitorResoSph.h
 *
 *  Created on: 23 sty 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITORRESO_H_
#define HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITORRESO_H_

#include "PropertyMonitorXY.h"

namespace Hal {
  /**
   * property monitor to plot p vs dp
   */
  class MonitorResoP : public PropertyMonitorXY {
  public:
    MonitorResoP();
    virtual Bool_t Init(Int_t task_id);
    virtual void Update(Bool_t passed, TObject* obj);
    virtual ~MonitorResoP() {};
    ClassDef(MonitorResoP, 1)
  };
  /**
   * property monitor to plot p vs delta theta
   */
  class MonitorResoTheta : public PropertyMonitorXY {
  public:
    MonitorResoTheta();
    virtual Bool_t Init(Int_t task_id);
    virtual void Update(Bool_t passed, TObject* obj);
    virtual ~MonitorResoTheta() {};
    ClassDef(MonitorResoTheta, 1)
  };
  /**
   * property monitor to plot p vs delta phi
   */
  class MonitorResoPhi : public PropertyMonitorXY {
  public:
    MonitorResoPhi();
    virtual Bool_t Init(Int_t task_id);
    virtual void Update(Bool_t passed, TObject* obj);
    virtual ~MonitorResoPhi() {};
    ClassDef(MonitorResoPhi, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITORRESO_H_ */
