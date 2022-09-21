/*
 * EventBinCut.cxx
 *
 *  Created on: 1 paź 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "EventBinningCut.h"

#include "Cout.h"
#include "Cut.h"
#include "EventComplexCut.h"
#include "Package.h"
#include "Parameter.h"
#include "Std.h"

#include <iostream>


namespace Hal {
  EventBinningCut::EventBinningCut() : EventCut(1), fEventCut(nullptr), fTotalBinsNo(1) {}


  Package* EventBinningCut::Report() const {
    Package* report = EventCut::Report();
    for (int i = 0; i < GetCutSize(); i++) {
      // report->AddObject(new ParameterInt(Form("Bins[%i]", i), fStepsNo.Get(i)));
    }
    report->AddObject(fEventCut->Report());
    return report;
  }

  EventBinningCut::EventBinningCut(const EventBinningCut& other) : EventCut(other), fEventCut(nullptr) {
    fMinTotal    = other.fMinTotal;
    fMaxTotal    = other.fMaxTotal;
    fBinConv     = other.fBinConv;
    fStepsNo     = other.fStepsNo;
    fValuesUp    = other.fValuesUp;
    fTotalBinsNo = other.fTotalBinsNo;
    if (other.fEventCut) { fEventCut = (EventCut*) other.fEventCut->MakeCopy(); }
  }

  EventBinningCut::EventBinningCut(const EventCut& cut, const std::vector<std::vector<Double_t>>& init) :
    EventCut(cut.GetCutSize()), fEventCut((EventCut*) cut.MakeCopy()) {
    PreInit(init);
  }

  EventBinningCut::EventBinningCut(const EventCut& cut, const std::initializer_list<int>& init) :
    EventCut(cut.GetCutSize()), fEventCut((EventCut*) cut.MakeCopy()) {
    Int_t cutSize = fEventCut->GetCutSize();
    std::vector<std::vector<Double_t>> xvals;
    Int_t counter = 0;
    for (auto val : init) {
      std::vector<Double_t> vals;
      Double_t minX = cut.GetMin(counter);
      Double_t maxX = cut.GetMax(counter);
      Double_t step = (maxX - minX) / Double_t(val);
      for (int i = 0; i <= val; i++) {
        vals.push_back(minX + i * step);
      }
      xvals.push_back(vals);
      counter++;
    }
    for (int i = counter; i < GetCutSize(); i++) {
      std::vector<Double_t> vals;
      vals.push_back(fEventCut->GetMin(i));
      vals.push_back(fEventCut->GetMax(i));
      xvals.push_back(vals);
    }
    PreInit(xvals);
  }


  EventBinningCut::EventBinningCut(const EventCut& cut, const std::initializer_list<std::initializer_list<Double_t>>& init) :
    EventCut(cut.GetCutSize()) {
    fEventCut = (EventCut*) cut.MakeCopy();
    std::vector<std::vector<Double_t>> vec;
    for (auto i : init) {
      vec.push_back(Hal::Std::GetVector(i));
    }
    for (unsigned int i = vec.size(); i < fEventCut->GetCutSize(); i++) {
      std::vector<Double_t> temp;
      temp.push_back(fEventCut->GetMin(i));
      temp.push_back(fEventCut->GetMax(i));
      vec.push_back(temp);
    }
    PreInit(vec);
  }

  void EventBinningCut::PreInit(const std::vector<std::vector<Double_t>>& vals) {
    Int_t cutSize = fEventCut->GetCutSize();
    fValuesUp     = vals;
    if (int(fValuesUp.size()) != cutSize) { Cout::PrintInfo("Wrong setting in event cut binned", EInfo::kCriticalError); }
    for (int i = 0; i < cutSize; i++) {
      fEventCut->SetMinMax(fValuesUp[i][0], fValuesUp[i][fValuesUp[i].size() - 1], i);
      SetMinMax(fEventCut->GetMin(i), fEventCut->GetMax(i), i);
      SetUnitName(fEventCut->GetUnit(i), i);
    }
    fBinConv.resize(cutSize);
    fStepsNo.resize(cutSize);
    fBinConv[0] = 1;
    fStepsNo[0] = fValuesUp[0].size() - 1;
    for (int i = 1; i < cutSize; i++) {
      fStepsNo[i] = fValuesUp[i].size() - 1;
      fBinConv[i] = fBinConv[i - 1] * fStepsNo[i - 1];
    }
    fTotalBinsNo = 1;
    for (auto i : fStepsNo) {
      fTotalBinsNo *= i;
    }
    fMinTotal.resize(cutSize);
    fMaxTotal.resize(cutSize);
    for (int i = 0; i < cutSize; i++) {
      fMinTotal[i] = fValuesUp[i][0];
      fMaxTotal[i] = fValuesUp[i][fValuesUp[i].size() - 1];
    }
  }

  EventBinningCut& EventBinningCut::operator=(const EventBinningCut& other) {
    if (&other == this) return *this;
    EventCut::operator=(other);
    if (fEventCut) delete fEventCut;
    fEventCut    = nullptr;
    fMinTotal    = other.fMinTotal;
    fMaxTotal    = other.fMaxTotal;
    fBinConv     = other.fBinConv;
    fStepsNo     = other.fStepsNo;
    fValuesUp    = other.fValuesUp;
    fTotalBinsNo = other.fTotalBinsNo;
    if (other.fEventCut) fEventCut = (EventCut*) other.fEventCut->MakeCopy();
    return *this;
  }

  EventBinningCut::~EventBinningCut() {
    if (fEventCut) delete fEventCut;
  }

  EventBinningCut* EventBinningCut::MakeCopy() const { return new EventBinningCut(*this); }

  Int_t EventBinningCut::CheckBin(Event* event) {
    if (fEventCut->Pass(event) == kFALSE) return -1;
    Int_t res = 0;

    for (int iParam = 0; iParam < GetCutSize(); iParam++) {
      Double_t val = fEventCut->GetValue(iParam);
      Int_t bin    = 0;
      for (int iRange = 1; iRange < (int) fValuesUp[iParam].size(); iRange++) {
        if (val <= fValuesUp[iParam][iRange]) { break; }
        bin++;
      }
      if (bin < 0 || bin >= fValuesUp[iParam].size()) { std::cout << " EER" << std::endl; }
      res = res + bin * fBinConv.at(iParam);
    }

    return res;
  }

  Bool_t EventBinningCut::Init(Int_t task_id) {
    if (fEventCut == nullptr) { return kFALSE; }
    return fEventCut->Init(task_id);
  }

  EventBinningCut* EventBinningCut::MakeCopyReal() const { return new EventBinningCut(EventRealCut(*fEventCut), fValuesUp); }

  EventBinningCut* EventBinningCut::MakeCopyImg() const { return new EventBinningCut(EventImaginaryCut(*fEventCut), fValuesUp); }

  void EventBinningCut::Print(Option_t* /*opt*/) const {
    auto printF = [](TString name, const std::vector<Double_t>& vec) {
      std::cout << name << std::endl;
      for (auto i : vec) {
        std::cout << i << " ";
      }
      std::cout << std::endl;
    };
    auto printI = [](TString name, const std::vector<Int_t>& vec) {
      std::cout << name << std::endl;
      for (auto i : vec) {
        std::cout << i << " ";
      }
      std::cout << std::endl;
    };
    printF("MinTot", fMinTotal);
    printF("MaxTot", fMaxTotal);
    printI("BinConv", fBinConv);
    printI("StepNo", fStepsNo);
    int c = 0;
    for (auto i : fValuesUp) {
      printF(Form("vals_%i ", c++), i);
    };
  }

}  // namespace Hal
