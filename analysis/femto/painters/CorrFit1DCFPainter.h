/*
 * CorrFit1DPainter.h
 *
 *  Created on: 23 lip 2024
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_PAINTERS_CORRFIT1DCFPAINTER_H_
#define HAL_ANALYSIS_FEMTO_PAINTERS_CORRFIT1DCFPAINTER_H_

#include "CorrFitPainter.h"

namespace Hal {
  class CorrFit1DCF;
  class Femto1DCF;
  class Femto1DCFPainter;

  class CorrFit1DCFPainter : public CorrFitPainter {
  protected:
    Femto1DCFPainter* fCFPainter = {nullptr};
    Femto1DCF* fCF;
    virtual void UpdateParameters();
    TF1* GetDrawableFunc(TString opt = "");
    virtual void FillFunctions();
    virtual void ScaleHistograms();
    virtual void MakeLegend();

  public:
    CorrFit1DCFPainter(CorrFit1DCF* fit = nullptr, Femto1DCF* cf = nullptr);
    virtual ~CorrFit1DCFPainter();
    ClassDef(CorrFit1DCFPainter, 0)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_PAINTERS_CORRFIT1DCFPAINTER_H_ */
