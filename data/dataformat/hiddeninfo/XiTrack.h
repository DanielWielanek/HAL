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

#include "HiddenTrackInfo.h"

namespace Hal {
  class Track;
  class V0Track;
  class XiTrack : public HiddenTrackInfo {
    Int_t fAssumedPdg         = {-1};
    Int_t fAssumedBachelorPdg = {-1};
    TVector3 fDecayPoint;

  public:
    XiTrack(Int_t size = 2) : HiddenTrackInfo(size) {};
    void SetBachelorId(Int_t id) { fData[1] = id; }
    void SetPdg(Int_t pid) { fAssumedPdg = pid; };
    void SetBachelorPdg(Int_t pid) { fAssumedBachelorPdg = pid; };
    void RotateZ(Double_t /*phi*/) {};  // TODO implement
    void SetDecayPoint(TVector3 vec) { fDecayPoint = vec; }
    Int_t GetBachelorId() const { return fData[1]; }
    Int_t GetBachelorPdg() const { return fAssumedBachelorPdg; }
    Int_t GetPdg() const { return fAssumedPdg; }
    virtual Int_t GetType() const { return 0; }
    TVector3 GetDecayPoint() const { return fDecayPoint; }
    virtual void CopyData(HiddenTrackInfo* v);
    virtual ~XiTrack() {};
    ClassDef(XiTrack, 1)
  };  // namespace Hal
  /**
   * cascade in format bachelor + V0
   */
  class XiTrack2 : public XiTrack {
    Int_t fAssumedV0Pdg = {-1};

  public:
    XiTrack2(Int_t size) : XiTrack(3) {};
    virtual void CopyData(HiddenTrackInfo* other);
    void SetV0Pdg(Int_t pid) { fAssumedV0Pdg = pid; }
    void SetV0Id(Int_t id) { fData[2] = id; };
    Int_t GetV0Pdg() const { return fAssumedV0Pdg; }
    Int_t GetV0Id() const { return fData[2]; }
    Int_t GetType() const final { return 2; }
    std::array<int, 2> GetLinks() const;
    virtual ~XiTrack2() {};
    ClassDef(XiTrack2, 1)
  };

  /**
   * cascade in format bachelor + decay products
   */
  class XiTrack3 : public XiTrack {
    Int_t fAssumedGranddaughter1pid = {-1};
    Int_t fAssumedGranddaughter2pid = {-1};

  public:
    XiTrack3() : XiTrack(4) {};
    virtual void CopyData(HiddenTrackInfo* other);
    void SetGranddau1Pid(Int_t pid) { fAssumedGranddaughter1pid = pid; }
    void SetGranddau2Pid(Int_t pid) { fAssumedGranddaughter2pid = pid; }
    void SetGranddau1Id(Int_t id) { fData[2] = id; }
    void SetGranddau2Id(Int_t id) { fData[3] = id; }
    Int_t GetGranddauPid1() const { return fAssumedGranddaughter1pid; };
    Int_t GetGranddauPid2() const { return fAssumedGranddaughter2pid; };
    Int_t GetGranddau1Id() const { return fData[2]; }
    Int_t GetGranddau2Id() const { return fData[3]; }
    Int_t GetType() const final { return 3; }
    virtual ~XiTrack3() {};
    ClassDef(XiTrack3, 1)
  };


} /* namespace Hal */

#endif /* HAL_DATA_DATAFORMAT_HIDDENINFO_XITRACK_H_ */
