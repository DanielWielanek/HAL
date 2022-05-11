/*
 * TwoTrack3DCFCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALFEMTO_CUTS_TWOTRACKCUTS_KINEMATICS_HALTWOTRACK3DCFCUT_H_
#define HALFEMTO_CUTS_TWOTRACKCUTS_KINEMATICS_HALTWOTRACK3DCFCUT_H_

#include "TwoTrackCut.h"

#include "TH3D.h"
namespace Hal {
  class TwoTrack3DCFCut : public TwoTrackCut {
    TH3D* fMap;
    Int_t fPID1, fPID2;
    Int_t fFrame;
    Bool_t fAbs;
    Double_t fM12, fM22;

  public:
    TwoTrack3DCFCut();
    void SetHistogram(TH3D const& histo);
    void SetAssumedPID(Int_t pid1, Int_t pid2) {
      fPID1 = pid1;
      fPID2 = pid2;
    };
    void UseLCMS() { fFrame = 0; };
    void UsePRF() { fFrame = 1; };
    void UseAbs(Bool_t abs = kTRUE) { fAbs = kTRUE; };
    virtual Bool_t Init(Int_t);
    virtual Cut* MakeCopy() const;
    virtual Bool_t Pass(TwoTrack* pair);
    virtual Package* Report() const;
    virtual ~TwoTrack3DCFCut();
    TwoTrack3DCFCut(const TwoTrack3DCFCut& other);
    TwoTrack3DCFCut& operator=(const TwoTrack3DCFCut& other);
    ClassDef(TwoTrack3DCFCut, 1)
  };
}  // namespace Hal

#endif /* HALFEMTO_CUTS_TWOTRACKCUTS_KINEMATICS_HALTWOTRACK3DCFCUT_H_ */
