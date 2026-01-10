/*
 * DividedHistoPainter.h
 *
 *  Created on: 10 sty 2026
 *      Author: daniel
 */

#ifndef HAL_FEATURES_HIST_HISTOPAINTER_H_
#define HAL_FEATURES_HIST_HISTOPAINTER_H_

#include "Painter.h"

#include <vector>

class TH1;
namespace Hal {
  class DividedHisto1D;
  class HistoPainter : public Painter {
  protected:
    static const int kHideTitlesBit;
    std::vector<std::vector<TH1*>> fHistograms;
    TString fDefDrawFlag = "SAME+P";
    void DeleteHistograms();
    virtual void MakeHistograms() = 0;
    virtual void DrawHistograms();
    virtual void InnerRepaint();
    virtual void InnerPaint();
    virtual void ScaleHistograms() {};
    TH1* CloneHist(TH1* h) const;

  public:
    HistoPainter() {};
    static Int_t LastBinPainter() { return kHideTitlesBit; }
    virtual ~HistoPainter();
    ClassDef(HistoPainter, 1)
  };


  class DividedHistoPainter : public HistoPainter {
  protected:
    static const int kNumBit;
    static const int kDenBit;
    static const int kAllBit;
    static const int kScaleBit;

    virtual ULong64_t SetOptionInternal(TString opt, ULong64_t prev = 0);
    virtual void MakePadsAndCanvases();
    virtual void MakeHistograms();
    Hal::DividedHisto1D* fHisto = {nullptr};

  public:
    DividedHistoPainter();
    DividedHistoPainter(Hal::DividedHisto1D* histo) { fHisto = histo; };
    static Int_t LastBinPainter() { return kScaleBit; }
    virtual ~DividedHistoPainter();
    ClassDef(DividedHistoPainter, 1)
  };

} /* namespace Hal */

#endif /* HAL_FEATURES_HIST_HISTOPAINTER_H_ */
