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
  class CorrFitFunc;
  class CorrFitPainter : public Painter {
  protected:
    Int_t fNormIndex                   = {-1};
    TLegend* fLegend                   = {nullptr};
    TVirtualPad* fLegendPad            = {nullptr};
    std::array<Double_t, 4> fLegendPos = {0.7, 0.95, 0.7, 0.95};
    std::vector<TLegendEntry*> fLegendEntries;
    static const int kAutoNormBit, kLegendBit, kChi2, kChi2Short;
    virtual ULong64_t SetOptionInternal(TString opt, ULong64_t prev = 0);
    std::vector<std::vector<TF1*>> fFunctions;
    CorrFitFunc* fFittedFunc        = {nullptr};  //!
    virtual void UpdateParameters() = 0;
    virtual void FillFunctions()    = 0;
    virtual void ScaleFunctions();
    virtual void ScaleHistograms() = 0;
    virtual void DrawFunctions();
    virtual void MakeLegend();
    virtual void UpdateLegend();
    virtual void MakeSubPads() {};
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
