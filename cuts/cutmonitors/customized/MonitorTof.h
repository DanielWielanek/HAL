/*
 * MonitorTof.h
 *
 *  Created on: 9 lip 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITORTOF_H_
#define HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITORTOF_H_

#include "PropertyMonitorXY.h"

namespace Hal {
  class MonitorTofM2 : public PropertyMonitorXY {
  public:
    MonitorTofM2();
    virtual Bool_t Init(Int_t task_id);
    virtual void Update(Bool_t passed, TObject* obj);
    virtual ~MonitorTofM2() {};
    ClassDef(MonitorTofM2, 1)
  };
  class MonitorTofBeta : public PropertyMonitorXY {
  public:
    MonitorTofBeta();
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Bool_t Init(Int_t task_id);
    virtual ~MonitorTofBeta() {};
    ClassDef(MonitorTofBeta, 1)
  };
}  // namespace Hal

#endif /* HAL_CUTS_CUTMONITORS_CUSTOMIZED_MONITORTOF_H_ */
