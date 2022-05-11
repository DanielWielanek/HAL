/*
 * EventBinCut.cxx
 *
 *  Created on: 1 paź 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "EventBinningCut.h"

#include "Cut.h"
#include "EventComplexCut.h"
#include "Package.h"
#include "Parameter.h"

#include "FairLogger.h"
#include <iostream>


namespace Hal {
  EventBinningCut::EventBinningCut() : EventCut(1), fTotalBinsNo(1), fEventCut(nullptr) {}


  Package* EventBinningCut::Report() const {
    Package* report = EventCut::Report();
    for (int i = 0; i < GetCutSize(); i++) {
      report->AddObject(new ParameterInt(Form("Bins[%i]", i), fStepsNo.Get(i)));
    }
    report->AddObject(fEventCut->Report());
    return report;
  }

  EventBinningCut::EventBinningCut(const EventBinningCut& other) : EventCut(other), fEventCut(nullptr) {
    fMinTotal    = other.fMinTotal;
    fMaxTotal    = other.fMaxTotal;
    fStep        = other.fStep;
    fBinConv     = other.fBinConv;
    fStepsNo     = other.fStepsNo;
    fTotalBinsNo = other.fTotalBinsNo;
    if (other.fEventCut) { fEventCut = (EventCut*) other.fEventCut->MakeCopy(); }
  }

  EventBinningCut& EventBinningCut::operator=(const EventBinningCut& other) {
    if (&other == this) return *this;
    EventCut::operator=(other);
    if (fEventCut) delete fEventCut;
    fEventCut    = nullptr;
    fMinTotal    = other.fMinTotal;
    fMaxTotal    = other.fMaxTotal;
    fStep        = other.fStep;
    fBinConv     = other.fBinConv;
    fStepsNo     = other.fStepsNo;
    fTotalBinsNo = other.fTotalBinsNo;
    if (other.fEventCut) fEventCut = (EventCut*) other.fEventCut->MakeCopy();
    return *this;
  }

  EventBinningCut::~EventBinningCut() {
    if (fEventCut) delete fEventCut;
  }

  EventBinningCut::EventBinningCut(Int_t nparamNo) : EventCut(nparamNo), fTotalBinsNo(1), fEventCut(nullptr) {
    fMinTotal.Resize(nparamNo);
    fMaxTotal.Resize(nparamNo);
    fStep.Resize(nparamNo);
    fBinConv.Resize(nparamNo);
    fStepsNo.Resize(nparamNo);
  }

  EventBinningCut::EventBinningCut(const EventCut& cut, const std::initializer_list<int>& init) :
    EventBinningCut(cut.GetCutSize()) {
    fEventCut = (EventCut*) cut.MakeCopy();
    for (int i = 0; i < GetCutSize(); i++) {
      SetUnitName(fEventCut->GetUnit(i), i);
      SetMinMax(fEventCut->GetMin(i), fEventCut->GetMax(i), i);
    }
    std::initializer_list<int>::iterator it;
    Int_t pos = 0;
    for (it = init.begin(); it != init.end(); ++it) {
      if (pos < fStepsNo.GetSize()) {
        fStepsNo.Set(pos++, *it);
        fTotalBinsNo = fTotalBinsNo * *it;
      }
    }
  }

  EventBinningCut* EventBinningCut::MakeCopy() const { return new EventBinningCut(*this); }

  Int_t EventBinningCut::CheckBin(Event* event) {
    if (fEventCut->Pass(event) == kFALSE) return -1;
    Int_t res = 0;
    for (int i = 0; i < GetCutSize(); i++) {
      Double_t val = fEventCut->GetValue(i) - fMinTotal.Get(i);
      Int_t no     = TMath::Floor(val * fStep.Get(i));
      res          = res + no * fBinConv.Get(i);
    }
    if (res == fTotalBinsNo) return fTotalBinsNo - 1;  // protection against "edge"
    return res;
  }

  Bool_t EventBinningCut::Init(Int_t task_id) {
    if (fEventCut == nullptr) { return kFALSE; }
    Bool_t res = fEventCut->Init(task_id);
    fBinConv.Set(0, 1);
    for (int i = 0; i < fEventCut->GetCutSize(); i++) {
      fMinTotal.Set(i, fEventCut->GetMin(i));
      fMaxTotal.Set(i, fEventCut->GetMax(i));
      SetMinMax(fMinTotal.Get(i), fMaxTotal.Get(i), i);
      Double_t nDiv = fStepsNo.Get(i);
      if (nDiv < 0) nDiv = 1;
      Double_t diff = fMaxTotal.Get(i) - fMinTotal.Get(i);
      fStep.Set(i, nDiv / diff);
      if (i > 0) fBinConv.Set(i, fBinConv.Get(i - 1));
    }
    return res;
  }

  EventBinningCut* EventBinningCut::MakeCopyReal() const {
    std::vector<int> bins;
    for (int i = 0; i < GetCutSize(); i++) {
      bins.push_back(fStepsNo.Get(i));
    }
    return new EventBinningCut(EventRealCut(*fEventCut), bins);
  }

  EventBinningCut* EventBinningCut::MakeCopyImg() const {
    std::vector<int> bins;
    for (int i = 0; i < GetCutSize(); i++) {
      bins.push_back(fStepsNo.Get(i));
    }
    return new EventBinningCut(EventImaginaryCut(*fEventCut), bins);
  }

  EventBinningCut::EventBinningCut(const EventCut& cut, const std::vector<int> init) : EventBinningCut(cut.GetCutSize()) {
    fEventCut = (EventCut*) cut.MakeCopy();
    for (unsigned int pos = 0; pos < init.size(); pos++) {
      fStepsNo.Set(pos, init[pos]);
      if (init[pos] == 0) {
        LOG(warning) << "cannot have event bins with 0 binst at " << pos;
      } else {
        fTotalBinsNo = fTotalBinsNo * init[pos];
      }
    }
  }
}  // namespace Hal
