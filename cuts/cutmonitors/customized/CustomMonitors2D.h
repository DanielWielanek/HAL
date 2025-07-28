/*
 * CutsomMonitors2D.h
 *
 *  Created on: 28 lip 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_CUTMONITORS_CUSTOMIZED_CUSTOMMONITORS2D_H_
#define HAL_CUTS_CUTMONITORS_CUSTOMIZED_CUSTOMMONITORS2D_H_

#include "PropertyMonitorXY.h"

namespace Hal {
  /**
   * class almost like PropertyMonitors but with automatical switch between complex and im format
   * use "re" "im" flag from SetOption
   */

  class CustomTrackMonitors2D : public PropertyMonitorXY {

  protected:
    Int_t fFieldId1 = {0};
    Int_t fFieldId2 = {0};

  public:
    CustomTrackMonitors2D();
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Bool_t Init(Int_t task_id);
    virtual Bool_t AreSimilar(const Hal::CutMonitor& other) const;
    virtual ~CustomTrackMonitors2D() {};
    ClassDef(CustomTrackMonitors2D, 1)
  };
  class CustomEventMonitors2D : public PropertyMonitorXY {

  protected:
    Int_t fFieldId1 = {0};
    Int_t fFieldId2 = {0};

  public:
    CustomEventMonitors2D();
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Bool_t AreSimilar(const Hal::CutMonitor& other) const;
    virtual Bool_t Init(Int_t task_id);
    virtual ~CustomEventMonitors2D() {};
    ClassDef(CustomEventMonitors2D, 1)
  };
} /* namespace Hal */

#endif /* HAL_CUTS_CUTMONITORS_CUSTOMIZED_CUSTOMMONITORS2D_H_ */
