/*
 * TwoTrackComplexCut.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTWOTRACKCOMPLEXCUT_H_
#define HALTWOTRACKCOMPLEXCUT_H_

#include "TwoTrackCut.h"
namespace Hal {
  class TwoTrackComplexCut : public TwoTrackCut {
    TwoTrackCut* fRealCut;
    TwoTrackCut* fImgCut;
    TwoTrack* fRealPair;
    TwoTrack* fImgPair;
    Int_t fStep;
    ULong64_t fNullObjects;
    Bool_t fAcceptNulls;

  public:
    TwoTrackComplexCut();
    TwoTrackComplexCut(const TwoTrackCut* real, const TwoTrackCut* img);
    TwoTrackComplexCut(const TwoTrackCut& real, const TwoTrackCut& img);
    TwoTrackComplexCut(const TwoTrackComplexCut& other);
    virtual Bool_t Pass(TwoTrack* pair);
    virtual Bool_t Init(Int_t task_id);
    virtual Package* Report() const;
    virtual TString CutName(Option_t* opt = "") const;
    TwoTrackCut* GetRealCut() const { return fRealCut; };
    TwoTrackCut* GetImgCut() const { return fImgCut; };
    virtual ~TwoTrackComplexCut();
    ClassDef(TwoTrackComplexCut, 1)
  };

  class TwoTrackRealCut : public TwoTrackCut {
    TwoTrackCut* fRealCut;
    TwoTrack* fPair;

  public:
    TwoTrackRealCut();
    TwoTrackRealCut(const TwoTrackCut* real);
    TwoTrackRealCut(const TwoTrackCut& real);
    TwoTrackRealCut(const TwoTrackRealCut& other);
    virtual Bool_t Pass(TwoTrack* track);
    virtual Bool_t Init(Int_t task_id);
    virtual Package* Report() const;
    virtual TString CutName(Option_t* opt = "") const;
    TwoTrackCut* GetRealCut() const { return fRealCut; };
    virtual ~TwoTrackRealCut();
    ClassDef(TwoTrackRealCut, 1)
  };

  class TwoTrackImaginaryCut : public TwoTrackCut {
    TwoTrackCut* fImgCut;
    TwoTrack* fPair;
    ULong64_t fNullObjects;
    Bool_t fAcceptNulls;

  public:
    TwoTrackImaginaryCut();
    TwoTrackImaginaryCut(const TwoTrackCut* real);
    TwoTrackImaginaryCut(const TwoTrackCut& real);
    TwoTrackImaginaryCut(const TwoTrackImaginaryCut& other);
    void AcceptNulls(Bool_t accept = kTRUE) { fAcceptNulls = accept; };
    virtual Bool_t Pass(TwoTrack* track);
    virtual Bool_t Init(Int_t task_id);
    virtual Package* Report() const;
    virtual TString CutName(Option_t* opt = "") const;
    TwoTrackCut* GetImgCut() const { return fImgCut; };
    virtual ~TwoTrackImaginaryCut();
    ClassDef(TwoTrackImaginaryCut, 1)
  };
}  // namespace Hal
#endif /* HALTWOTRACKCOMPLEXCUT_H_ */
