/*
 * EventBinCut.h
 *
 *  Created on: 1 paź 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HALEVENTBINNINGCUT_H_
#define HALEVENTBINNINGCUT_H_

#include "EventCut.h"

#include "Array.h"
#include <initializer_list>
#include <vector>


namespace Hal {
  class EventBinningCut : public EventCut {
  protected:
    Int_t fTotalBinsNo;
    Array_1<Float_t> fMinTotal;
    Array_1<Float_t> fMaxTotal;
    Array_1<Float_t> fStep;
    Array_1<Int_t> fBinConv;
    Array_1<Int_t> fStepsNo;
    EventCut* fEventCut;
    EventBinningCut(Int_t nparamNo);
    EventBinningCut(const EventCut& cut, const std::vector<int> init);

  public:
    EventBinningCut();
    EventBinningCut(const EventCut& cut, const std::initializer_list<int>& init);
    EventBinningCut(const EventBinningCut& other);
    EventBinningCut* MakeCopy() const;
    EventBinningCut* MakeCopyReal() const;
    EventBinningCut* MakeCopyImg() const;
    EventBinningCut& operator=(const EventBinningCut& other);
    const EventCut* GetCut() const { return (EventCut*) fEventCut->MakeCopy(); };
    Int_t GetBinsNo() const { return fTotalBinsNo; };
    virtual Package* Report() const;
    virtual Bool_t Pass(Event* event) { return kTRUE; };
    virtual Int_t CheckBin(Event* event);
    virtual Bool_t Init(Int_t task_id);
    virtual ~EventBinningCut();
    ClassDef(EventBinningCut, 1)
  };
}  // namespace Hal
#endif /* HALEVENTBINNINGCUT_H_ */
