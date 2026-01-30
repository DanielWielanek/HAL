/*
 * FemtoFuncPainter.h
 *
 *  Created on: 22 lip 2024
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_PAINTERS_FEMTOFUNCPAINTER_H_
#define HAL_ANALYSIS_PAINTERS_FEMTOFUNCPAINTER_H_

#include "HistoPainter.h"
#include "Painter.h"
#include "Style.h"

#include <iostream>

class TH1;

namespace Hal {
  /**
   * base class for drawing correlation functions
   * to check available option of drawing look @see #Hal::FemtoSHCFPainter#SetOptionInternal
   * @see Hal::Femto1DCFPainter::SetOptionInternal @see
   * Hal::Femto3DCFPainter::SetOptionInternal
   */
  class FemtoCFPainter : public DividedHistoPainter {
    friend class CorrFitPainter;

  protected:
    static const int kCFBit;
    Double_t fScale     = {1.0};
    Double_t fDrawScale = {1.0};

    virtual ULong64_t SetOptionInternal(const Options& opt, ULong64_t prev = 0);
    virtual void ScaleHistograms();
    ULong64_t PrepBitTemplate(std::initializer_list<int> temps) const;
    /**
     *
     * @param current
     * @param pattern
     * @return true if current have all bits from pattern
     */
    Bool_t AreSimiliar(ULong64_t current, ULong64_t pattern) const;
    virtual void InnerRepaint();
    /**
     *
     * @return fake histograms used instead of TF1 if "th1" options is called
     */
    virtual std::vector<std::vector<TH1*>> GetFakeDrawFuncs() const;

  public:
    FemtoCFPainter() {}
    /**
     *
     * @param x
     * @param y
     * @return min and max value of draw histogram x, y, if no histogram was drawn or x/y are too big returns 0,0
     */
    std::pair<Double_t, Double_t> GetMinMax(Int_t x = 0, Int_t y = 0) const;
    /**
     * rescale hisgorams used by corrfits
     * @param newScale
     */
    virtual void Rescale(Double_t newScale);
    /**
     *
     * @return option for drawing corrfit
     */
    virtual TString GetOptionForCorrFit() const { return ""; };
    static Int_t LastBitPainter() { return kCFBit; }
    virtual ~FemtoCFPainter();
    ClassDef(FemtoCFPainter, 0)
  };
}  // namespace Hal

#endif /* HAL_ANALYSIS_PAINTERS_FEMTOFUNCPAINTER_H_ */
