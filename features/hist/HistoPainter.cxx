/*
 * DividedHistoPainter.cxx
 *
 *  Created on: 10 sty 2026
 *      Author: daniel
 */

#include "DividedHisto.h"
#include "StdString.h"

#include "HistoPainter.h"
#include <RtypesCore.h>
#include <TH1.h>
#include <TString.h>
#include <TVirtualPad.h>
#include <iostream>

#include "Options.h"
#include "Std.h"

namespace Hal {
  const int HistoPainter::kHideTitlesBit = Painter::LastBitPainter() + 1;

  const int DividedHistoPainter::kNumBit   = HistoPainter::LastBinPainter() + 1;
  const int DividedHistoPainter::kDenBit   = HistoPainter::LastBinPainter() + 2;
  const int DividedHistoPainter::kAllBit   = HistoPainter::LastBinPainter() + 3;
  const int DividedHistoPainter::kScaleBit = HistoPainter::LastBinPainter() + 4;


  TH1* HistoPainter::CloneHist(TH1* h) const {
    if (!h) return nullptr;
    TH1* copy = (TH1*) h->Clone();
    h->SetName(Form("%i", Hal::Std::anonymCounter++));
    copy->SetDirectory(nullptr);
    copy->SetStats(0);
    copy->SetObjectStat(kFALSE);
    return copy;
  }

  void HistoPainter::DeleteHistograms() {
    for (auto& x : fHistograms) {
      for (auto y : x)
        if (y) delete y;
    }
    fHistograms.clear();
  }

  void HistoPainter::DrawHistograms() {
    LockPad();
    int count = 0;
    for (auto i : fHistograms) {
      GotoPad(++count);
      for (auto j : i) {
        if (j) {
          if (CheckOpt(kHideTitlesBit)) j->SetTitle("");
          j->SetStats(0);
          j->Draw(fDefDrawFlag);
        }
      }
    }
    UnlockPad();
  }

  void HistoPainter::InnerRepaint() {
    LockPad();
    if (fOptionsChanged) {
      DeleteHistograms();
      MakeHistograms();
      ScaleHistograms();
    }
    OptionsApplied();
    UnlockPad();
  }

  void HistoPainter::InnerPaint() {
    LockPad();
    DeleteHistograms();
    MakeHistograms();
    ScaleHistograms();
    DrawHistograms();
    OptionsApplied();
    UnlockPad();
  }

  HistoPainter::~HistoPainter() {
    for (auto& x : fHistograms) {
      for (auto y : x)
        if (y) delete y;
    }
    fHistograms.clear();
  }


  DividedHistoPainter::DividedHistoPainter() {}

  ULong64_t DividedHistoPainter::SetOptionInternal(const Options& opt, ULong64_t newOpts) {
    newOpts = HistoPainter::SetOptionInternal(opt, newOpts);
    if (opt.HasOption("num")) SETBIT(newOpts, kNumBit);
    if (opt.HasOption("den")) SETBIT(newOpts, kDenBit);
    if (opt.HasOption("all")) SETBIT(newOpts, kAllBit);
    if (opt.HasOption("scale")) SETBIT(newOpts, kScaleBit);
    return newOpts;
  }

  void DividedHistoPainter::MakePadsAndCanvases() {
    if (CheckOpt(kAllBit)) {
      MakeCanvasPads(2, 2, 0);
    } else if (CheckOpt(kDenBit)) {
      MakeCanvasPads(1, 1, 0);
    } else if (CheckOpt(kNumBit)) {
      MakeCanvasPads(1, 1, 0);
    } else {
      MakeCanvasPads(1, 1, 0);
    }
  }

  void DividedHistoPainter::MakeHistograms() {
    std::vector<TH1*> row1;
    if (CheckOpt(kAllBit)) {
      fHistograms.push_back({CloneHist(fHisto->GetNum())});
      fHistograms.push_back({CloneHist(fHisto->GetDen())});
      fHistograms.push_back({CloneHist(fHisto->GetHist(CheckOpt(kScaleBit)))});
      auto num = CloneHist(CloneHist(fHisto->GetNum()));
      num->SetLineColor(kGreen);
      num->SetMarkerColor(kGreen);
      auto den = CloneHist(CloneHist(fHisto->GetDen()));
      den->SetLineColor(kRed);
      den->SetMarkerColor(kRed);
      fHistograms.push_back({num, den});
    } else if (CheckOpt(kDenBit)) {
      row1.push_back(CloneHist(fHisto->GetDen()));
      fHistograms.push_back(row1);
    } else if (CheckOpt(kNumBit)) {
      row1.push_back(CloneHist(fHisto->GetNum()));
      fHistograms.push_back(row1);
    } else {
      row1.push_back(CloneHist(fHisto->GetHist(CheckOpt(kScaleBit))));
      fHistograms.push_back(row1);
    }
  }

  DividedHistoPainter::~DividedHistoPainter() {
    // TODO Auto-generated destructor stub
  }

  ULong64_t HistoPainter::SetOptionInternal(const Options& opt, ULong64_t newFlags) {
    if (opt.HasOption("!tit")) SETBIT(newFlags, kHideTitlesBit);
    auto foundx = opt.GetLabeledArray("x");
    auto foundy = opt.GetLabeledArray("y");
    auto foundz = opt.GetLabeledArray("z");
    auto foundt = opt.GetLabeledArray("t");
    if(foundx.values.size()==2){
        fRangeX[0] = foundx.values[0];
        fRangeX[1] = foundx.values[1];
    }
    if(foundy.values.size()==2){
        fRangeY[0] = foundy.values[0];
        fRangeY[1] = foundy.values[1];
    }
    if(foundz.values.size()==2){
        fRangeZ[0] = foundz.values[0];
        fRangeZ[1] = foundz.values[1];
    }
    if(foundt.values.size()==2){
        fRangeT[0] = foundt.values[0];
        fRangeT[1] = foundt.values[1];
    }
    return newFlags;
  }

} /* namespace Hal */
