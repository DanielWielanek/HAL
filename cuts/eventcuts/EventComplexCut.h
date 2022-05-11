/*
 * EventComplexCut.h
 *
 *  Created on: 24 maj 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALROOT_CUTS_EVENTCUTS_HALEVENTCOMPLEXCUT_H_
#define HALROOT_CUTS_EVENTCUTS_HALEVENTCOMPLEXCUT_H_

#include "EventCut.h"

namespace Hal {
  class EventComplexCut : public EventCut {
    EventCut* fRealCut;
    EventCut* fImgCut;
    Int_t fStep;
    ULong64_t fNullObjects;
    Bool_t fAcceptNulls;

  public:
    EventComplexCut();
    EventComplexCut(const EventCut* real, const EventCut* img);
    EventComplexCut(const EventCut& real, const EventCut& img);
    EventComplexCut(const EventComplexCut& other);
    void AcceptNulls(Bool_t accept = kTRUE) { fAcceptNulls = accept; };
    virtual Bool_t Pass(Event* pair);
    virtual Bool_t Init(Int_t task_id);
    virtual Package* Report() const;
    virtual TString CutName(Option_t* opt = "") const;
    EventCut* GetRealCut() const { return fRealCut; };
    EventCut* GetImgCut() const { return fImgCut; };
    virtual ~EventComplexCut();
    ClassDef(EventComplexCut, 1)
  };

  class EventRealCut : public EventCut {
    EventCut* fRealCut;

  public:
    EventRealCut();
    EventRealCut(const EventCut* real);
    EventRealCut(const EventCut& real);
    EventRealCut(const EventRealCut& other);
    virtual Bool_t Pass(Event* event);
    virtual Bool_t Init(Int_t task_id);
    virtual Package* Report() const;
    virtual TString CutName(Option_t* opt = "") const;
    EventCut* GetRealCut() const { return fRealCut; };
    virtual ~EventRealCut();
    ClassDef(EventRealCut, 1)
  };

  class EventImaginaryCut : public EventCut {
    EventCut* fImgCut;
    ULong64_t fNullObjects;
    Bool_t fAcceptNulls;

  public:
    EventImaginaryCut();
    EventImaginaryCut(const EventCut* img);
    EventImaginaryCut(const EventCut& img);
    EventImaginaryCut(const EventImaginaryCut& other);
    void AcceptNulls(Bool_t accept = kTRUE) { fAcceptNulls = accept; };
    virtual Bool_t Pass(Event* event);
    virtual Bool_t Init(Int_t task_id);
    virtual Package* Report() const;
    virtual TString CutName(Option_t* opt = "") const;
    EventCut* GetImgCut() const { return fImgCut; };
    virtual ~EventImaginaryCut();
    ClassDef(EventImaginaryCut, 1)
  };
}  // namespace Hal

#endif /* HALROOT_CUTS_EVENTCUTS_HALEVENTCOMPLEXCUT_H_ */
