/*
 * FemtoSHCFPainter.cxx
 *
 *  Created on: 22 lip 2024
 *      Author: daniel
 */

#include "FemtoSHCFPainter.h"

#include <RtypesCore.h>
#include <TAttMarker.h>
#include <TAxis.h>
#include <TH1.h>
#include <TRandom.h>
#include <TVirtualPad.h>
#include <initializer_list>
#include <vector>

#include "Cout.h"
#include "FemtoSHCF.h"
#include "HistoStyle.h"
#include "PadStyle.h"
#include "Std.h"
#include "StdHist.h"
#include "StdString.h"

namespace Hal {
  const int FemtoSHCFPainter::kReBit    = 16;
  const int FemtoSHCFPainter::kImBit    = 17;
  const int FemtoSHCFPainter::kShortBit = 18;
  const int FemtoSHCFPainter::kSepBit   = 19;

  ULong64_t FemtoSHCFPainter::SetOptionInternal(TString opts, ULong64_t newFlags) {
    newFlags = FemtoCFPainter::SetOptionInternal(opts, newFlags);
    ContitionalPattern(opts, "re", newFlags, kReBit, kTRUE);
    ContitionalPattern(opts, "im", newFlags, kImBit, kTRUE);
    std::vector<double> yrange;
    GetPatterns(opts, "y", yrange);
    fRangesYY = yrange;
    if (!TESTBIT(newFlags, kNumBit) && TESTBIT(newFlags, kDenBit)) { SETBIT(newFlags, kCFBit); }
    if (Hal::Std::FindParam(opts, "fit", kTRUE)) {
      SETBIT(newFlags, kCFBit);
      CLRBIT(newFlags, kDenBit);
      CLRBIT(newFlags, kNumBit);
    }
    if (Hal::Std::FindParam(opts, "sep", kTRUE)) SETBIT(newFlags, kSepBit);
    if (!CheckOpt(kReBit) && !CheckOpt(kImBit)) {
      SETBIT(newFlags, kReBit);
      SETBIT(newFlags, kImBit);
    }
    if (Hal::Std::FindParam(opts, "short", kTRUE)) SETBIT(newFlags, kShortBit);
    return newFlags;
  }

  FemtoSHCFPainter::FemtoSHCFPainter(Hal::FemtoSHCF* cf) : FemtoCFPainter(), fSHCF(cf) {
    Hal::PadStyle style(0.045, 0.045, 0.025, 0.025);
    SetGlobalPadStyle(style);
  }

  std::pair<TH1*, TH1*> FemtoSHCFPainter::GetNewHistPair(Int_t l, Int_t m) {
    std::pair<TH1*, TH1*> res(nullptr, nullptr);
    if (CheckOpt(kNumBit)) {                        // return numerators
      if (CheckOpt(kImBit) && !CheckOpt(kReBit)) {  // only imag
        res.first = fSHCF->GetNumIm(l, m);
      } else if (!CheckOpt(kImBit) && CheckOpt(kReBit)) {  // only real
        res.first = fSHCF->GetNumRe(l, m);
      } else {  // im + real
        res.first  = fSHCF->GetNumRe(l, m);
        res.second = fSHCF->GetNumIm(l, m);
      }
    } else if (CheckOpt(kDenBit)) {                 // return denominators
      if (CheckOpt(kImBit) && !CheckOpt(kReBit)) {  // only imag
        res.first = fSHCF->GetDenIm(l, m);
      } else if (!CheckOpt(kImBit) && CheckOpt(kReBit)) {  // only real
        res.first = fSHCF->GetDenRe(l, m);
      } else {  // im + real
        res.first  = fSHCF->GetDenRe(l, m);
        res.second = fSHCF->GetDenIm(l, m);
      }
    } else {                                        // return cf's
      if (CheckOpt(kImBit) && !CheckOpt(kReBit)) {  // only imag
        res.first = fSHCF->GetCFIm(l, m);
      } else if (!CheckOpt(kImBit) && CheckOpt(kReBit)) {  // only real
        res.first = fSHCF->GetCFRe(l, m);
      } else {  // im + real
        res.first  = fSHCF->GetCFRe(l, m);
        res.second = fSHCF->GetCFIm(l, m);
      }
    }
    if (res.first) {
      res.first = CloneHist(res.first);
      Hal::Std::CopyHistProp(*fSHCF->GetNum(), *res.first, "!tit");
      if (res.first) SetHistRanges(res.first, l);
    }
    if (res.second) {
      res.second = CloneHist(res.second);
      Hal::Std::CopyHistProp(*fSHCF->GetNum(), *res.second, "!tit");
      if (res.second) SetHistRanges(res.second, l);
    }
    // fix colors
    if (CheckOpt(kImBit) && !CheckOpt(kReBit)) {  // only imag
      Hal::Std::SetColor(*res.first, fColorIm);
    } else if (!CheckOpt(kImBit) && CheckOpt(kReBit)) {  // only real
      Hal::Std::SetColor(*res.first, fColorRe);
    } else {  // im + real
      Hal::Std::SetColor(*res.second, fColorIm);
      Hal::Std::SetColor(*res.first, fColorRe);
    }

    return res;
  }

  void FemtoSHCFPainter::SetHistRanges(TH1* hist, Int_t l) {
    hist->SetMinimum(0);
    double max = hist->GetBinContent(hist->GetMaximumBin());
    double min = hist->GetBinContent(hist->GetMinimumBin());
    if (min < 0) {
      hist->SetMinimum(-1);
    } else {
      hist->SetMinimum(0);
    }
    if (fRangesYY.size() == 4) {
      if (l == 0) {
        hist->SetMinimum(fRangesYY[0]);
        hist->SetMaximum(fRangesYY[1]);
      } else {
        hist->SetMinimum(fRangesYY[2]);
        hist->SetMaximum(fRangesYY[3]);
      }
    }
    if (max < 1) { hist->SetMaximum(1); }
    // if (fRangesX[0[] == 2) { hist->GetXaxis()->SetRangeUser(fRangesX[0], fRangesX[1]); }
  }

  void FemtoSHCFPainter::MakeHistograms() {
    fColorIm = fSHCF->fColIm;
    fColorRe = fSHCF->fColRe;
    int lx   = fSHCF->GetLMax();
    Hal::Std::ResizeVector2D(fHistograms, (lx + 1) * (lx + 1), 2);
    for (auto x : fHistograms)
      for (auto& y : x)
        y = nullptr;
    for (int l = 0; l <= fSHCF->GetLMax(); l++) {
      for (int m = -l; m <= l; m++) {
        auto histos = GetNewHistPair(l, m);
        int padid   = fSHCF->fLmVals.GetPadId(l, m) - 1;
        if (fRangesYY.size() == 4) {
          if (l == 0 && histos.first) {
            histos.first->SetMinimum(fRangesYY[0]);
            histos.first->SetMaximum(fRangesYY[1]);
          } else {
            if (histos.first) {
              histos.first->SetMinimum(fRangesYY[2]);
              histos.first->SetMaximum(fRangesYY[3]);
            }
            if (histos.second) {
              histos.second->SetMinimum(fRangesYY[2]);
              histos.second->SetMaximum(fRangesYY[3]);
            }
          }
        }
        fHistograms[padid][0] = histos.first;
        fHistograms[padid][1] = histos.second;
        if (CheckOpt(kSepBit) && m >= 0) fHistograms[padid][1] = nullptr;
        if (CheckOpt(kSepBit) && m < 0) fHistograms[padid][0] = nullptr;
        if (CheckOpt(kShortBit) && m < 0) {
          fHistograms[padid][0] = nullptr;
          fHistograms[padid][1] = nullptr;
        }
        if (histos.first && !fHistograms[padid][0]) delete histos.first;
        if (histos.second && !fHistograms[padid][1]) delete histos.second;
      }
    }
  }

  void FemtoSHCFPainter::AddHistograms(TH1* re, TH1* im) {
    std::vector<TH1*> pairHist;
    pairHist.push_back(re);
    pairHist.push_back(im);
    fHistograms.push_back(pairHist);
  }

  Bool_t FemtoSHCFPainter::CheckPads() const {
    Int_t padsNo = GetPadsNo();
    Int_t req    = (fSHCF->GetLMax() + 1) * (fSHCF->GetLMax() + 1);
    if (padsNo != req) return kFALSE;
    return kTRUE;
  }

  TString FemtoSHCFPainter::GetOptionForCorrFit() const {
    if (CheckOpt(kShortBit)) { return "fit1"; }
    if (CheckOpt(kSepBit)) { return "fit3"; }
    return "fit2";
  }

  void FemtoSHCFPainter::ScaleHistograms() {
    FemtoCFPainter::ScaleHistograms();
    int count = 0;
    for (auto x : fHistograms) {
      for (auto y : x) {
        if (y && fRangesYY.size() == 4) {
          int step = 0;
          if (count != 0) step = 2;
          y->SetMinimum(fRangesYY[step]);
          y->SetMaximum(fRangesYY[step + 1]);
        }
        count++;
      }
    }
  }

  void FemtoSHCFPainter::MakePadsAndCanvases() {
    Int_t req = (fSHCF->GetLMax() + 1) * (fSHCF->GetLMax() + 1);
    MakeCanvasPads(TMath::Sqrt(req), TMath::Sqrt(req));
  }

}  // namespace Hal
