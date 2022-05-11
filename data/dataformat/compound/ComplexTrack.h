/*
 * TrackCombined.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALCOMPLEXTRACK_H_
#define HALCOMPLEXTRACK_H_

#include "Track.h"

/**
 * base class for "comlex track" - object that is build from "real" and
 * "imaginary" track
 */
namespace Hal {
  class ComplexTrack : public Track {
    Int_t fMatchID;
    Track* fRealTrack;
    Track* fImgTrack;

  public:
    ComplexTrack();
    ComplexTrack(const ComplexTrack& other);
    ComplexTrack& operator=(const ComplexTrack& other);
    void SetMatchID(Int_t match) { fMatchID = match; };
    void SetRealTrack(Track* tr) { fRealTrack = tr; };
    void SetImgTrack(Track* tr) { fImgTrack = tr; };
    Int_t GetMatchID() const { return fMatchID; };
    virtual void Clear(Option_t* opt = "");
    virtual Float_t GetFieldVal(Int_t fieldID) const;
    virtual void CopyData(Track* other);
    virtual void CopyAllData(Track* other);
    virtual void ResetTrack(Int_t thisId = -1, Event* event = nullptr);
    virtual TString GetFieldName(Int_t fieldID) const;
    Track* GetRealTrack() const { return fRealTrack; };
    Track* GetImgTrack() const { return fImgTrack; };
    virtual ~ComplexTrack();
    ClassDef(ComplexTrack, 1)
  };
}  // namespace Hal
#endif /* HALCOMPLEXTRACK_H_ */
