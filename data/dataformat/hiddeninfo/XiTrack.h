/*
 * XiTrack.h
 *
 *  Created on: 17 cze 2024
 *      Author: daniel
 */

#ifndef HAL_DATA_DATAFORMAT_HIDDENINFO_XITRACK_H_
#define HAL_DATA_DATAFORMAT_HIDDENINFO_XITRACK_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TVector3.h>

#include "HiddenInfo.h"

namespace Hal {
  class Track;
  class V0Track;
  class XiTrack : public HiddenInfo {
    Int_t fTrackId           = {-1};
    Int_t fV0Id              = {-1};
    Int_t fChargedId         = {-1};
    Int_t fAssumedPdg        = {-1};
    Int_t fAssumedPdgNeu     = {-1};
    Int_t fAssumedPdgCharged = {-1};
    TVector3 fDecay;

  public:
    XiTrack() {};
    void SetTrackId(Int_t id) { fTrackId = id; }
    void SetV0Id(Int_t neu) { fV0Id = neu; }
    void SetChId(Int_t id) { fChargedId = id; };
    void RotateZ(Double_t /*phi*/) {};  // TODO implement
    Int_t GetTrackId() const { return fTrackId; }
    Int_t GetV0Id() const { return fV0Id; }
    Int_t GetChId() const { return fChargedId; };
    virtual void CopyData(XiTrack* v);
    virtual ~XiTrack() {};
    ClassDef(XiTrack, 1)
  };

} /* namespace Hal */

#endif /* HAL_DATA_DATAFORMAT_HIDDENINFO_XITRACK_H_ */
