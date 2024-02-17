/*
 * FemtoPainter.h
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_FEMTOPAINTER_H_
#define HAL_ANALYSIS_FEMTO_BASE_FEMTOPAINTER_H_

/**
 * represents painted correlation functions
 */
#include "Painter.h"

#include "FemtoDrawOptions.h"

namespace Hal {
  class HistoStyle;
  class PadStyle;
  class FemtoPainter : public Painter {
  protected:
    Bool_t fDrawNormLines = {kFALSE};
    Double_t fPrevScale   = {1.0};
    Double_t fNewScale    = {1.0};
    FemtoDrawOptions fDrawOpts;
    FemtoPainter(FemtoDrawOptions opts) { fDrawOpts = opts; }

  public:
    FemtoPainter() {};
    virtual void Rescale(Double_t newScale);
    virtual void Repaint() { Paint(); };
    virtual void Paint() = 0;
    virtual void Draw(Option_t* opt = "") { Paint(); };
    virtual ~FemtoPainter() {};
    ClassDef(FemtoPainter, 1)
  };
} /* namespace Hal */


#endif /* HAL_ANALYSIS_FEMTO_BASE_FEMTOPAINTER_H_ */
