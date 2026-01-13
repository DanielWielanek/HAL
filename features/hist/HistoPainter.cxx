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
#include <iostream>

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

  ULong64_t DividedHistoPainter::SetOptionInternal(TString opt, ULong64_t newOpts) {
    newOpts = HistoPainter::SetOptionInternal(opt, newOpts);
    if (Hal::Std::FindParam(opt, "num", kTRUE)) SETBIT(newOpts, kNumBit);
    if (Hal::Std::FindParam(opt, "den", kTRUE)) SETBIT(newOpts, kDenBit);
    if (Hal::Std::FindParam(opt, "all", kTRUE)) SETBIT(newOpts, kAllBit);
    if (Hal::Std::FindParam(opt, "scale", kTRUE)) SETBIT(newOpts, kScaleBit);
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

  ULong64_t HistoPainter::SetOptionInternal(TString opt, ULong64_t newFlags) {
    if (Hal::Std::FindParam(opt, "!tit", kTRUE)) SETBIT(newFlags, kHideTitlesBit);
    auto ranges = Hal::Std::FindBrackets(opt, kTRUE, kTRUE);
    for (auto range : ranges) {
      std::vector<double> res;
      auto foundx = GetPatterns(range, "x", res);
      if (res.size() == 2 && foundx) {
        fRangeX[0] = res[0];
        fRangeX[1] = res[1];
      }
      auto foundy = GetPatterns(range, "y", res);
      if (res.size() == 2 && foundy) {
        fRangeY[0] = res[0];
        fRangeY[1] = res[1];
      }
      auto foundz = GetPatterns(range, "z", res);
      if (res.size() == 2 && foundz) {
        fRangeZ[0] = res[0];
        fRangeZ[1] = res[1];
      }
      auto foundt = GetPatterns(range, "t", res);
      if (res.size() == 2 && foundt) {
        fRangeT[0] = res[0];
        fRangeT[1] = res[1];
      }
    }
    return newFlags;
  }

} /* namespace Hal */
