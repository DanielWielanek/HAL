/*
 * TrackFlagMonitor1D.h
 *
 *  Created on: 7 paź 2025
 *      Author: daniel
 */

#ifndef HAL_CUTS_CUTMONITORS_CUSTOMIZED_TRACKFLAGMONITOR1D_H_
#define HAL_CUTS_CUTMONITORS_CUSTOMIZED_TRACKFLAGMONITOR1D_H_

#include <RtypesCore.h>
#include <vector>

#include "Object.h"
#include "PropertyMonitorX.h"
#include "PropertyMonitorXY.h"

namespace Hal {
  class Track;
} /* namespace Hal */

namespace Hal {
  /**
   * helper class for extraction of flags
   */
  class TrackFlagChecker : public Object {
    enum eFlags {
      kPrimary            = 0,
      kGlobal             = 1,
      kKnownMother        = 2,
      kV0daughtersKnown   = 3,
      kV0daughtersUnknown = 4,
      kXidaughtersKnown   = 5,
      kXidaughersUnknown  = 6,
      kBackground         = 7,
      kEmbeded            = 8,
      kAny                = 9
    };


  public:
    TrackFlagChecker() {};
    const Int_t GetFlagNo() const { return 10; }
    std::vector<Int_t> GetFlags(Track* track) const;
    void SetupAxis(TAxis* axis) const;
    virtual ~TrackFlagChecker() {};
    ClassDef(TrackFlagChecker, 1)
  };

  /**
   * plot flags
   */
  class TrackFlagMonitor1D : public PropertyMonitorX {
    TrackFlagChecker fChecker;

  public:
    TrackFlagMonitor1D();
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Bool_t Init(Int_t task_id);
    virtual ~TrackFlagMonitor1D() {};
    ClassDef(TrackFlagMonitor1D, 1)
  };
  /**
   * plots correlation between flags
   */
  class TrackFlagMonitor2D : public PropertyMonitorXY {
    TrackFlagChecker fChecker;

  public:
    TrackFlagMonitor2D();
    virtual void Update(Bool_t passed, TObject* obj);
    virtual Bool_t Init(Int_t task_id);
    virtual ~TrackFlagMonitor2D() {};
    ClassDef(TrackFlagMonitor2D, 1)
  };
}  // namespace Hal

#endif /* HAL_CUTS_CUTMONITORS_CUSTOMIZED_TRACKFLAGMONITOR1D_H_ */
