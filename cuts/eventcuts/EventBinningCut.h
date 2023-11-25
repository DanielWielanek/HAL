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
    Int_t fTotalBinsNo = {1};
    Bool_t fLastPassed = {kFALSE};
    std::vector<Double_t> fMinTotal;
    std::vector<Double_t> fMaxTotal;
    std::vector<Int_t> fBinConv;
    std::vector<Int_t> fStepsNo;

    std::vector<std::vector<Double_t>> fValuesUp;
    EventCut* fEventCut = {nullptr};
    EventBinningCut(const EventCut& cut, const std::vector<std::vector<Double_t>>& init);
    void PreInit(const std::vector<std::vector<Double_t>>& vals);

  public:
    EventBinningCut();
    EventBinningCut(const EventCut& cut, const std::initializer_list<Int_t>& init);
    EventBinningCut(const EventCut& cut, const std::initializer_list<std::initializer_list<Double_t>>& init);
    EventBinningCut(const EventBinningCut& other);
    EventBinningCut* MakeCopy() const;
    EventBinningCut* MakeCopyReal() const;
    EventBinningCut* MakeCopyImg() const;
    EventBinningCut& operator=(const EventBinningCut& other);
    EventCut* GetCut() const { return (EventCut*) fEventCut->MakeCopy(); };
    Int_t GetBinsNo() const { return fTotalBinsNo; };
    void GetBinParam(Int_t bin, std::vector<Double_t>& mini, std::vector<Double_t>& maxi, std::vector<TString>& strings) const;
    virtual Package* Report() const;
    virtual Bool_t Pass(Event* event);
    virtual Int_t CheckBin(Event* event);
    virtual Bool_t Init(Int_t task_id);
    virtual void Print(Option_t* opt = 0) const;
    virtual TString CutName(Option_t* opt = "") const;
    virtual ~EventBinningCut();
    ClassDef(EventBinningCut, 1)
  };
}  // namespace Hal
#endif /* HALEVENTBINNINGCUT_H_ */
