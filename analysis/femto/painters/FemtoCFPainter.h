/*
 * FemtoFuncPainter.h
 *
 *  Created on: 22 lip 2024
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_PAINTERS_FEMTOFUNCPAINTER_H_
#define HAL_ANALYSIS_PAINTERS_FEMTOFUNCPAINTER_H_

#include "Painter.h"
#include "Style.h"

#include <iostream>

class TH1;

namespace Hal {
  /**
   * base class for drawing correlation functions
   * to check available option of drawing look @see #Hal::FemtoSHCFPainter#SetOptionInternal
   * @see #Hal::Femto1DCFPainter#SetOptionInternal @see
   * #Hal::Femto3DCFPainter#SetOptionInternal
   */
  class FemtoCFPainter : public Painter {
    friend class CorrFitPainter;

  protected:
    Double_t fRangeX[2] = {0, 0};
    Double_t fRangeY[2] = {0, 0};
    static const int kNumBit, kDenBit, kCFBit, kHideTitles;
    Double_t fScale      = {1.0};
    Double_t fDrawScale  = {1.0};
    TString fDefDrawFlag = "SAME+P";
    std::vector<std::vector<TH1*>> fHistograms;

    virtual ULong64_t SetOptionInternal(TString opt, ULong64_t prev = 0);
    TH1* CloneHist(TH1*) const;
    void DeleteHistograms();
    virtual void MakeHistograms() = 0;
    virtual void ScaleHistograms();
    virtual void DrawHistograms();
    ULong64_t PrepBitTemplate(std::initializer_list<int> temps) const;
    /**
     *
     * @param current
     * @param pattern
     * @return true if current have all bits from pattern
     */
    Bool_t AreSimiliar(ULong64_t current, ULong64_t pattern) const;
    virtual void InnerPaint();
    virtual void InnerRepaint();

  public:
    FemtoCFPainter() {}
    /**
     *
     * @param x
     * @param y
     * @return min and max value of draw histogram x, y, if no histogram was drawn or x/y are too big returns 0,0
     */
    std::pair<Double_t, Double_t> GetMinMax(Int_t x = 0, Int_t y = 0) const;
    virtual void Rescale(Double_t newScale);

    /**
     *
     * @return option for drawing corrfit
     */
    virtual TString GetOptionForCorrFit() const { return ""; };
    virtual ~FemtoCFPainter();
    ClassDef(FemtoCFPainter, 0)
  };
}  // namespace Hal

#endif /* HAL_ANALYSIS_PAINTERS_FEMTOFUNCPAINTER_H_ */
