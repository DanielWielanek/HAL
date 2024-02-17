/*
 * Femto3DCFPainter.h
 *
 *  Created on: 17 lut 2024
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef HAL_ANALYSIS_FEMTO_BASE_GRAPHIC_FEMTO3DCFPAINTER_H_
#define HAL_ANALYSIS_FEMTO_BASE_GRAPHIC_FEMTO3DCFPAINTER_H_

#include "FemtoPainter.h"

#include <vector>

class TH1;
namespace Hal {
  class Femto3DCF;
  class Femto3DCFPainter : public FemtoPainter {
    TH1* fNum = {nullptr};
    TH1* fDen = {nullptr};
    std::vector<TH1*> fCFs;
    std::vector<TH1*> fDiag1;
    std::vector<TH1*> fDiag2;

  public:
    Femto3DCFPainter(Femto3DCF& other, FemtoDrawOptions opts);
    Femto3DCFPainter() {};
    virtual void Paint();
    virtual void Rescale(Double_t newScale);
    virtual ~Femto3DCFPainter();
    ClassDef(Femto3DCFPainter, 1)
  };

} /* namespace Hal */

#endif /* HAL_ANALYSIS_FEMTO_BASE_GRAPHIC_FEMTO3DCFPAINTER_H_ */
