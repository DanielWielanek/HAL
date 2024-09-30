/*
 * CorrFitPainter.h
 *
 *  Created on: 23 lip 2024
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_PAINTERS_CORRFITPAINTER_H_
#define HAL_ANALYSIS_FEMTO_PAINTERS_CORRFITPAINTER_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TF1.h>
#include <TLegendEntry.h>
#include <TString.h>
#include <array>
#include <initializer_list>
#include <vector>

#include "Painter.h"

class TLegend;
class TVirtualPad;
namespace Hal {
  /**
   * class for drawing CF's, when you call CF->Draw a painter is created
   * later you can get this painter and modify current pads etc.
   * the property of drawn object depends on used flags
   *
   */
  class CorrFitFunc;
  class FemtoCFPainter;
  class CorrFitPainter : public Painter {
  protected:
    Int_t fNormIndex                   = {-1};
    TLegend* fLegend                   = {nullptr};
    TVirtualPad* fLegendPad            = {nullptr};
    TString fDefFuncDrawOpt            = "SAME";
    std::array<Double_t, 4> fLegendPos = {0.7, 0.95, 0.7, 0.95};
    std::vector<TLegendEntry*> fLegendEntries;
    static const int kAutoNormBit, kLegendBit, kChi2, kChi2Short;
    /**
     *
     * @param opt, for this class additional options are available:
     * - "chi2" - draw chi2 on legend (including total value of chi2 and NDF)
     * - "chi2s " - draw chi2 on legend
     * - "legend" - draw legend
     * @param prev
     * @return
     */
    virtual ULong64_t SetOptionInternal(TString opt, ULong64_t prev = 0);
    std::vector<std::vector<TF1*>> fFunctions;
    CorrFitFunc* fFittedFunc   = {nullptr};  //!
    FemtoCFPainter* fCFPainter = {nullptr};

    virtual void MakePadsAndCanvases() {};
    virtual void UpdateParameters() = 0;
    virtual void MakeFunctions()    = 0;
    virtual void ScaleFunctions();
    virtual void ScaleHistograms();
    virtual void DrawFunctions();
    virtual void MakeLegend();
    virtual void UpdateLegend();
    void DeleteFunctions();
    std::vector<TString> GetLegendLabels() const;
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
    CorrFitPainter(CorrFitFunc* func = nullptr);
    virtual ~CorrFitPainter();
    ClassDef(CorrFitPainter, 0)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_PAINTERS_CORRFITPAINTER_H_ */
