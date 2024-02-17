/*
 * FemtoSHCFPainter.cxx
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "FemtoSHCFPainter.h"

#include "FemtoSHCF.h"
#include "Std.h"
#include "StdHist.h"

#include <TH1.h>
#include <TVirtualPad.h>

namespace Hal {

  FemtoSHCFPainter::FemtoSHCFPainter(Hal::FemtoSHCF& cf, FemtoDrawOptions opts) : FemtoPainter(opts) {
    if (!cf.GetCFRe(0, 0)) { cf.RecalculateCF(); }
    fDrawOpts       = opts;
    Int_t padsNo    = Hal::Std::GetListOfSubPads(fMainPad);
    fMaxL           = cf.GetLMax();
    const int maxL1 = cf.GetLMax() + 1;
    Int_t req       = maxL1 * maxL1;
    if (padsNo != req) {
      fMainPad->Clear();
      fMainPad->Divide(maxL1, maxL1);
    }
    if (fDrawOpts.DrawNum()) {
      for (int i = 0; i < maxL1 * maxL1; i++) {
        fReal.push_back((TH1*) cf.fDenReal[i]->Clone());
        fImag.push_back((TH1*) cf.fDenImag[i]->Clone());
      }
    } else if (fDrawOpts.DrawDen()) {
      for (int i = 0; i < maxL1 * maxL1; i++) {
        fReal.push_back((TH1*) cf.fNumReal[i]->Clone());
        fImag.push_back((TH1*) cf.fNumImag[i]->Clone());
      }
    } else {

      for (int i = 0; i < maxL1 * maxL1; i++) {
        fReal.push_back((TH1*) cf.fCFReal[i]->Clone());
        fImag.push_back((TH1*) cf.fCFImag[i]->Clone());
      }
    }
    fPadStyle = new Hal::PadStyle(0.045, 0.045, 0.05, 0.045);
  }

  void FemtoSHCFPainter::Repaint() {}

  void FemtoSHCFPainter::Paint() {
    TString opT;
    auto tempPad = gPad;
    if (!fImStyle && fHistoStyle) {
      fImStyle = new Hal::HistoStyle*(fHistoStyle);
      fImStyle->SetAntiColor(kTRUE);
    }
    if (fDrawOpts.DrawSame()) { opT = "SAME"; }
    auto rangesX = fDrawOpts.GetXRanges();
    auto rangesY = fDrawOpts.GetYRanges();
    auto indexes = Hal::FemtoYlmIndexes(fMaxL);
    auto drawSub = [&](Int_t l, Int_t m, Int_t flag) {
      fPadStyle->Apply(gPad);
      fSubPads.push_back(gPad);
      TH1D* cf = nullptr;

      if (flag == 0)
        cf = fReal[indexes.GetIndex(l, m)];
      else
        cf = fImag[indexes.GetIndex(l, m)];
      if (cf == nullptr) return;
      if (fHistoStyle) fHistoStyle->Apply(*cf);

      cf->SetMinimum(0);
      double max = cf->GetBinContent(cf->GetMaximumBin());
      double min = cf->GetBinContent(cf->GetMinimumBin());
      if (min < 0) {
        cf->SetMinimum(-1);
      } else {
        cf->SetMinimum(0);
      }
      if (max < 1) { cf->SetMaximum(1); }
      cf->SetStats(kFALSE);
      cf->SetObjectStat(kFALSE);
      if (rangesY.size() == 4) {
        if (l == 0) {
          cf->SetMinimum(rangesY[0]);
          cf->SetMaximum(rangesY[1]);
        } else {
          cf->SetMinimum(rangesY[2]);
          cf->SetMaximum(rangesY[3]);
        }
      }
      if (rangesX.size() == 2) { cf->GetXaxis()->SetRangeUser(rangesX[0], rangesX[1]); }
      cf->Draw(opT);
    };
    for (int l = 0; l <= fMaxL; l++) {
      if (fDrawOpts.DrawSeparate()) {  // draw seperate
        for (int m = 0; m <= l; m++) {
          drawSub(l, m, 0, opT);
          for (int m = -l; m < 0; m++) {
            fMainPad->cd(indexes.GetPadId(l, m));
            drawSub(l, m, 1, opT);
          }
        }
      } else if (fDrawOpts.SkipNegative()) {
        for (int m = 0; m <= l; m++) {
          fMainPad->cd(indexes.GetPadId(l, m));
          if (fDrawOpts.DrawRe() && !fDrawOpts.DrawIm()) {
            drawSub(l, m, 0, opT);
          } else if (fDrawOpts.DrawIm() && !fDrawOpts.DrawRe()) {
            drawSub(l, m, 1, opT);
          } else {
            drawSub(l, m, 0, opT);
            drawSub(l, m, 1, opT + "SAME");
          }
        }
      } else {  // draw everything
        for (int m = -l; m <= l; m++) {
          fMainPad->cd(indexes.GetPadId(l, m));
          if (fDrawOpts.DrawRe() && !fDrawOpts.DrawIm()) {
            drawSub(l, m, 0, opT);
          } else if (fDrawOpts.DrawIm() && !fDrawOpts.DrawRe()) {
            drawSub(l, m, 1, opT);
          } else {
            drawSub(l, m, 0, opT);
            drawSub(l, m, 1, opT + "SAME");
          }
        }
      }
    }
    gPad = tempPad;
  }

  void FemtoSHCFPainter::Rescale(Double_t newScale) {
    FemtoSHCFPainter::Rescale(newScale);
    for (auto i : fReal) {
      i->Scale(fNewScale);
    }
    for (auto i : fImag) {
      i->Scale(fNewScale);
    }
  }

  FemtoSHCFPainter::~FemtoSHCFPainter() {
    Hal::Std::DeletePointerVector(fReal);
    Hal::Std::DeletePointerVector(fImag);
    if (fImStyle) delete fImStyle;
  }

} /* namespace Hal */
