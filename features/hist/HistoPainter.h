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
    /**
     * flag bit
     */
    static const int kHideTitlesBit;
    /**
     * array of histograsm [canvas][pad]
     */
    std::vector<std::vector<TH1*>> fHistograms;
    /**
     * flag used to draw histograms
     */
    TString fDefDrawFlag = "SAME+P";
    /**
     * theta angle of pads
     */
    Double_t fThetaPad = {30.};
    /**
     * phi angle of pads
     */
    Double_t fPhiPad = {30.};
    /**
     * x-range of histograms
     */
    Double_t fRangeX[2] = {0, 0};
    /**
     * y-range of histograms
     */
    Double_t fRangeY[2] = {0, 0};
    /**
     * z-range of histograms
     */
    Double_t fRangeZ[2] = {0, 0};
    /**
     * t-range of histograms
     */
    Double_t fRangeT[2] = {0, 0};
    /**
     * delete all histograms from fHistograms
     */
    void DeleteHistograms();
    /**
     * create histograms in fHistograms
     */
    virtual void MakeHistograms() = 0;
    /**
     * draw histograms
     */
    virtual void DrawHistograms();
    virtual void InnerRepaint();
    virtual void InnerPaint();
    virtual void ScaleHistograms() {};
    /**
     *
     * @param opt @see HistoPainter::SetOptionInternal plus additionaly:
     * - "{x=a,b}" where a-b are axis limits the same logic is for x,y,z, and "t" axis
     * Note - if given axis is not found (e.g. y axis for TH1D) then SetMinimum/Maximum is applied
     * - {ang=theta,phi} - set theta/phi angles (for drawing 3d objects
     * -"!tit" - hide titles
     * @param newFlags
     * @return
     */
    ULong64_t SetOptionInternal(TString opt, ULong64_t newFlags);
    /**
     * clone new histograms, clone is not owned by gDirectory
     * @param h
     * @return
     */
    TH1* CloneHist(TH1* h) const;

  public:
    HistoPainter() {};
    /**
     * last bit used by this class
     * @return
     */
    static Int_t LastBinPainter() { return kHideTitlesBit; }
    virtual ~HistoPainter();
    ClassDef(HistoPainter, 1)
  };


  class DividedHistoPainter : public HistoPainter {
  protected:
    /**
     * flag bit
     */
    static const int kNumBit;
    /**
     * flag bit
     */
    static const int kDenBit;
    /**
     * flag bit
     */
    static const int kAllBit;
    /**
     * flag bit
     */
    static const int kScaleBit;
    /**
     *
     * @param opt drawing options @see HistoPainter::SetOptionInternal + additionaly:
     * - "num" - draw numerator only
     * - "den" - draw denominator only
     * - "all" - draw all histograms
     * - "scale" - draw scaled by scale of DividedHisto
     * @param prev
     * @return
     */
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
