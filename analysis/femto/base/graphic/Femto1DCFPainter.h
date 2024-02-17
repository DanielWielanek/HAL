/*
 * Femto1DCFPainter.h
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_GRAPHIC_FEMTO1DCFPAINTER_H_
#define HAL_ANALYSIS_FEMTO_BASE_GRAPHIC_FEMTO1DCFPAINTER_H_

#include "FemtoPainter.h"

class TH1;
namespace Hal {
  class Femto1DCF;
  class Femto1DCFPainter : public FemtoPainter {
    TH1* fNum   = {nullptr};
    TH1* fDen   = {nullptr};
    TH1* fRatio = {nullptr};

  public:
    Femto1DCFPainter() {};
    Femto1DCFPainter(Hal::Femto1DCF& cf, FemtoDrawOptions opts);
    virtual void Rescale(Double_t newScale);
    virtual void Paint();
    virtual ~Femto1DCFPainter();
    ClassDef(Femto1DCFPainter, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_BASE_GRAPHIC_FEMTO1DCFPAINTER_H_ */
