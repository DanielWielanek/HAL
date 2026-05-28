/*
 * FemtoFuncPainter.cxx
 *
 *  Created on: 22 lip 2024
 *      Author: daniel
 */

#include "FemtoCFPainter.h"

#include "Cout.h"
#include "Options.h"
#include "Std.h"

#include <iostream>

#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>

namespace Hal {
  const int FemtoCFPainter::kCFBit = DividedHistoPainter::LastBinPainter() + 1;

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


  FemtoCFPainter::~FemtoCFPainter() {}

  void FemtoCFPainter::InnerRepaint() {
    LockPad();
    if (fOptionsChanged) {
      DeleteHistograms();
      MakeHistograms();
      ScaleHistograms();
    } else if (fScale != 1) {
      ScaleHistograms();
    }
    OptionsApplied();
    UnlockPad();
  }

  ULong64_t FemtoCFPainter::PrepBitTemplate(std::initializer_list<int> temps) const {
    ULong64_t res = 0;
    auto vec      = Hal::Std::GetVector(temps);
    for (auto i : vec)
      SETBIT(res, i);
    return res;
  }

  Bool_t FemtoCFPainter::AreSimiliar(ULong64_t current, ULong64_t pattern) const { return (pattern == current) & pattern; }

  ULong64_t FemtoCFPainter::SetOptionInternal(const Options& opt, ULong64_t newOpts) {
    newOpts = Hal::DividedHistoPainter::SetOptionInternal(opt, newOpts);
    if (opt.HasOption("fit")) {
      CLRBIT(newOpts, kDenBit);
      CLRBIT(newOpts, kNumBit);
    }
    if (opt.HasOption("cf")) {
      CLRBIT(newOpts, kDenBit);
      CLRBIT(newOpts, kNumBit);
      SETBIT(newOpts, kCFBit);
    }

    if (!TESTBIT(newOpts, kNumBit) && !TESTBIT(newOpts, kDenBit)) { SETBIT(newOpts, kCFBit); }
    return newOpts;
  }

  std::pair<Double_t, Double_t> FemtoCFPainter::GetMinMax(Int_t x, Int_t y) const {
    std::pair<Double_t, Double_t> res;
    if ((int) fHistograms.size() < x + 1) return res;
    if ((int) fHistograms[x].size() < y + 1) return res;
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
