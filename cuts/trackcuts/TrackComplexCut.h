/*
 * TrackCutComplex.h
 *
 *  Created on: 04-05-2022
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALTRACKCOMPLEXCUT_H_
#define HALTRACKCOMPLEXCUT_H_

#include "TrackVirtualCut.h"

#include "TrackCut.h"

namespace Hal {
  class TrackComplexCut : public TrackCut {
    TrackCut* fRealCut;
    TrackCut* fImgCut;
    Int_t fStep;
    ULong64_t fNullObjects;
    Bool_t fAcceptNulls;

  public:
    /**
     * dummy ct-ro only for ROOT
     */
    TrackComplexCut();
    /**
     * default constructor
     * @param real - real cut
     * @param img imaginary cut
     */
    TrackComplexCut(const TrackCut* real, const TrackCut* img);
    TrackComplexCut(const TrackCut& real, const TrackCut& img);
    TrackComplexCut(TrackCut* cut);
    TrackComplexCut(const TrackComplexCut& other);
    void AcceptNulls(Bool_t accept = kTRUE) { fAcceptNulls = accept; };
    virtual Bool_t Pass(Track* track);
    virtual Bool_t Init(Int_t task_id);
    virtual Package* Report() const;
    virtual TString CutName(Option_t* opt = "") const;
    /**
     *
     * @return "real" part of cut
     */
    TrackCut* GetRealCut() const { return fRealCut; };
    /**
     *
     * @return "imaginary" part of cut
     */
    TrackCut* GetImgCut() const { return fImgCut; };
    virtual ~TrackComplexCut();
    ClassDef(TrackComplexCut, 1)
  };

  class TrackRealCut : public TrackCut {
    TrackCut* fRealCut;

  public:
    TrackRealCut();
    TrackRealCut(const TrackCut* real);
    TrackRealCut(const TrackCut& real);
    TrackRealCut(const TrackRealCut& other);
    virtual Bool_t Pass(Track* track);
    virtual Bool_t Init(Int_t task_id);
    virtual Package* Report() const;
    virtual TString CutName(Option_t* opt = "") const;
    TrackCut* GetRealCut() const { return fRealCut; };
    virtual ~TrackRealCut();
    ClassDef(TrackRealCut, 1)
  };

  class TrackImaginaryCut : public TrackCut {
    TrackCut* fImgCut;
    ULong64_t fNullObjects;
    Bool_t fAcceptNulls;

  public:
    TrackImaginaryCut();
    TrackImaginaryCut(const TrackCut* img);
    TrackImaginaryCut(const TrackCut& img);
    TrackImaginaryCut(const TrackImaginaryCut& other);
    /**
     * accept tracks without imaginary tracks
     * @param accept
     */
    void AcceptNulls(Bool_t accept = kTRUE) { fAcceptNulls = accept; };
    virtual Bool_t Pass(Track* track);
    virtual Bool_t Init(Int_t task_id);
    virtual Package* Report() const;
    virtual TString CutName(Option_t* opt = "") const;
    TrackCut* GetImgCut() const { return fImgCut; };
    virtual ~TrackImaginaryCut();
    ClassDef(TrackImaginaryCut, 1)
  };
}  // namespace Hal
#endif /* HALTRACKCOMPLEXCUT_H_ */
