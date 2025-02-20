/*
 * CorrFit3DCFPainter.h
 *
 *  Created on: 24 lip 2024
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_FEMTO_PAINTERS_CORRFIT3DCFPAINTER_H_
#define HAL_ANALYSIS_FEMTO_PAINTERS_CORRFIT3DCFPAINTER_H_

#include "CorrFitPainter.h"

namespace Hal {
  class CorrFit3DCF;
  class Femto3DCF;
  class Femto3DCFPainter;
  class CorrFit3DCFPainter : public CorrFitPainter {
  protected:
    Femto3DCF* fCF;
    Int_t fDrawOpt = {0};
    virtual void UpdateParameters();
    virtual void DrawFunctions();
    TF1* GetDrawableFunc(TString opt = "");
    virtual void MakeFunctions();
    virtual void InnerPaint();

  public:
    CorrFit3DCFPainter(CorrFit3DCF* fit = nullptr, Femto3DCF* cf = nullptr);
    virtual ~CorrFit3DCFPainter();
    ClassDef(CorrFit3DCFPainter, 0)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_PAINTERS_CORRFIT3DCFPAINTER_H_ */
