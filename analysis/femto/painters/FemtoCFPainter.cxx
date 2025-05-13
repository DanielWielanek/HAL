/*
 * FemtoFuncPainter.cxx
 *
 *  Created on: 22 lip 2024
 *      Author: daniel
 */

#include "FemtoCFPainter.h"

#include "Cout.h"
#include "Std.h"

#include <iostream>

#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>

namespace Hal {
  const int FemtoCFPainter::kNumBit     = 8;
  const int FemtoCFPainter::kDenBit     = 9;
  const int FemtoCFPainter::kCFBit      = 10;
  const int FemtoCFPainter::kHideTitles = 11;

  void FemtoCFPainter::DeleteHistograms() {
    for (auto& x : fHistograms) {
      for (auto y : x)
        if (y) delete y;
    }
    fHistograms.clear();
  }


  void FemtoCFPainter::ScaleHistograms() {
    for (auto& x : fHistograms) {
      for (auto y : x)
        if (y) y->Scale(fScale);
    }
    fDrawScale = fScale * fDrawScale;
  }

  void FemtoCFPainter::Rescale(Double_t newScale) {
    if (newScale == 0) return;
    fScale = newScale / fDrawScale;
  }

  void FemtoCFPainter::DrawHistograms() {
    LockPad();
    int count = 0;
    for (auto i : fHistograms) {
      GotoPad(++count);
      for (auto j : i) {
        if (j) {
          if (CheckOpt(kHideTitles)) j->SetTitle("");
          j->SetStats(0);
          j->Draw(fDefDrawFlag);
        }
      }
    }
    UnlockPad();
  }

  FemtoCFPainter::~FemtoCFPainter() { DeleteHistograms(); }

  void FemtoCFPainter::InnerPaint() {
    LockPad();
    DeleteHistograms();
    MakeHistograms();
    ScaleHistograms();
    DrawHistograms();
    Painter::InnerPaint();
    OptionsApplied();
    UnlockPad();
  }

  void FemtoCFPainter::InnerRepaint() {
    LockPad();
    if (fOptionsChanged) {
      DeleteHistograms();
      MakeHistograms();
      ScaleHistograms();
    } else if (fScale != 1)
      ScaleHistograms();
    Painter::InnerRepaint();
    OptionsApplied();
    UnlockPad();
  }

  TH1* FemtoCFPainter::CloneHist(TH1* h) const {
    TH1* copy = (TH1*) h->Clone();
    h->SetName(Form("%i", Hal::Std::anonymCounter++));
    copy->SetDirectory(nullptr);
    copy->SetStats(0);
    copy->SetObjectStat(kFALSE);
    return copy;
  }

  ULong64_t FemtoCFPainter::PrepBitTemplate(std::initializer_list<int> temps) const {
    ULong64_t res = 0;
    auto vec      = Hal::Std::GetVector(temps);
    for (auto i : vec)
      SETBIT(res, i);
    return res;
  }

  Bool_t FemtoCFPainter::AreSimiliar(ULong64_t current, ULong64_t pattern) const { return (pattern == current) & pattern; }

  ULong64_t FemtoCFPainter::SetOptionInternal(TString opt, ULong64_t newOpts) {
    if (Hal::Std::FindParam(opt, "num", kTRUE)) { SETBIT(newOpts, kNumBit); }
    if (Hal::Std::FindParam(opt, "den", kTRUE)) { SETBIT(newOpts, kDenBit); }
    if (Hal::Std::FindParam(opt, "tit", kTRUE)) { SETBIT(newOpts, kHideTitles); }
    if (Hal::Std::FindParam(opt, "fit", kTRUE)) {
      CLRBIT(newOpts, kDenBit);
      CLRBIT(newOpts, kNumBit);
    }
    auto ranges = Hal::Std::FindBrackets(opt, kTRUE, kTRUE);
    for (auto range : ranges) {
      std::vector<double> res;
      auto foundx = GetPatterns(range, "x", res);
      auto foundy = GetPatterns(range, "y", res);
      if (res.size() == 2 && foundx) {
        fRangeX[0] = res[0];
        fRangeX[1] = res[1];
      }
      if (res.size() == 2 && foundy) {
        fRangeY[0] = res[0];
        fRangeY[1] = res[1];
      }
    }
    if (!TESTBIT(newOpts, kNumBit) && !TESTBIT(newOpts, kDenBit)) { SETBIT(newOpts, kCFBit); }
    return newOpts;
  }

  std::pair<Double_t, Double_t> FemtoCFPainter::GetMinMax(Int_t x, Int_t y) const {
    std::pair<Double_t, Double_t> res;
    if (fHistograms.size() < x + 1) return res;
    if (fHistograms[x].size() < y + 1) return res;
    auto h = fHistograms[x][y];
    if (!h) return res;
    res.first  = h->GetMinimum();
    res.second = h->GetMaximum();
    return res;
  }

  std::vector<std::vector<TH1*>> FemtoCFPainter::GetFakeDrawFuncs() const {
    std::vector<std::vector<TH1*>> res;
    int counter = 0;
    for (auto i : fHistograms) {
      std::vector<TH1*> row;
      for (auto h : i) {
        Int_t binx, biny;
        Double_t minx, miny, maxx, maxy;
        if (h->InheritsFrom("TH2")) {
          Hal::Std::GetAxisPar(*h, binx, minx, maxx, "x");
          Hal::Std::GetAxisPar(*h, biny, miny, maxy, "y");
          row.push_back(new TH2D(Form("func_%i", counter++), "", binx, minx, maxx, biny, miny, maxy));
        } else {
          Hal::Std::GetAxisPar(*h, binx, minx, maxx, "x");
          row.push_back(new TH1D(Form("func_%i", counter++), "", binx, minx, maxx));
        }
      }
      res.push_back(row);
    }
    return res;
  }

}  // namespace Hal
