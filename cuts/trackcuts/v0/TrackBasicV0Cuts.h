/*
 * TrackBasicV0Cuts.h
 *
 *  Created on: 11 cze 2024
 *      Author: daniel
 */

#ifndef HAL_CUTS_TRACKCUTS_V0_TRACKBASICV0CUTS_H_
#define HAL_CUTS_TRACKCUTS_V0_TRACKBASICV0CUTS_H_

#include <Rtypes.h>
#include <RtypesCore.h>

#include "CutMonitorRequest.h"
#include "CutsAndMonitors.h"

namespace Hal {
  class TrackDCACut;
  class TrackV0DaughterDistCut;
  class TrackV0MinvCut;
  class TrackV0DecLenCut;
}  // namespace Hal

namespace Hal {

  class TrackBasicV0Cuts : public CutsAndMonitors {
    enum kCuts { kDCA = 0, kDauDCA = 1, kMinv = 2, kDecLen = 3 };

    Hal::CutMonAxisConf fDCAxyzXY;
    Hal::CutMonAxisConf fDCAxyzZ;
    Hal::CutMonAxisConf fDCA;
    Hal::CutMonAxisConf fMinv;
    Hal::CutMonAxisConf fDecLen;
    Hal::CutMonAxisConf fDauDist;

  protected:
    virtual void AddAllCutMonitorRequests(Option_t* opt);

  public:
    TrackBasicV0Cuts();
    Hal::TrackDCACut* GetDCACut() const { return (Hal::TrackDCACut*) CutAt(0); };
    Hal::TrackV0DaughterDistCut* GetDaughterDistCut() const { return (Hal::TrackV0DaughterDistCut*) CutAt(1); }
    Hal::TrackV0MinvCut* GetMinvCut() const { return (Hal::TrackV0MinvCut*) CutAt(2); }
    Hal::TrackV0DecLenCut* GetDecLenCut() const { return (Hal::TrackV0DecLenCut*) CutAt(3); }
    virtual ~TrackBasicV0Cuts();
    ClassDef(TrackBasicV0Cuts, 1)
  };

} /* namespace Hal */

#endif /* HAL_CUTS_TRACKCUTS_V0_TRACKBASICV0CUTS_H_ */
