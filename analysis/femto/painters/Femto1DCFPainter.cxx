/*
 * Femto1DCFPainter.cxx
 *
 *  Created on: 22 lip 2024
 *      Author: daniel
 */

#include "Femto1DCFPainter.h"

#include <RtypesCore.h>
#include <TH1.h>
#include <TString.h>

#include "Femto1DCF.h"
#include "StdHist.h"
#include "StdString.h"

namespace Hal {

  Femto1DCFPainter::Femto1DCFPainter(Hal::Femto1DCF* cf) { fCF = cf; }

  void Femto1DCFPainter::FillHistograms() {
    std::vector<TH1*> hist;
    if (CheckOpt(kDenBit)) { hist.push_back(CloneHist(fCF->GetDen())); }
    if (CheckOpt(kNumBit)) { hist.push_back(CloneHist(fCF->GetNum())); }
    if (CheckOpt(kCFBit)) { hist.push_back(fCF->GetHist(false)); }
    if (hist.size() == 2) {
      Hal::Std::SetColor(*hist[0], kGreen);
      Hal::Std::SetColor(*hist[1], kRed);
    }
    if (fRangeX[0] != fRangeX[1]) {
      for (auto h : hist) {
        h->GetXaxis()->SetRangeUser(fRangeX[0], fRangeX[1]);
      }
    }
    if (fRangeY[0] != fRangeY[1]) {
      for (auto h : hist) {
        h->SetMaximum(fRangeY[1]);
        h->SetMinimum(fRangeY[0]);
      }
    }
    fHistograms.push_back(hist);
  }

  void Femto1DCFPainter::ScaleHistograms() {
    FemtoCFPainter::ScaleHistograms();
    if (fRangeY[0] != fRangeY[1])
      for (auto& x : fHistograms) {
        for (auto y : x) {
          y->SetMinimum(fRangeY[0]);
          y->SetMaximum(fRangeY[1]);
        }
      }
  }

  Femto1DCFPainter::~Femto1DCFPainter() {}


}  // namespace Hal
