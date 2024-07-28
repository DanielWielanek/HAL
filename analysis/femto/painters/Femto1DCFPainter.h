/*
 * Femto1DCFPainter.h
 *
 *  Created on: 22 lip 2024
 *      Author: daniel
 */

#ifndef HAL_ANALYSIS_PAINTERS_FEMTO1DCFPAINTER_H_
#define HAL_ANALYSIS_PAINTERS_FEMTO1DCFPAINTER_H_

#include "FemtoCFPainter.h"

namespace Hal {
  class Femto1DCF;
  class Femto1DCFPainter : public FemtoCFPainter {
  protected:
    Femto1DCF* fCF = {nullptr};
    virtual void FillHistograms();
    virtual void ScaleHistograms();
    virtual void MakeSubPads() { GenerateSubPads(1, 1); };

  public:
    Femto1DCFPainter(Hal::Femto1DCF* cf = nullptr);
    virtual ~Femto1DCFPainter();
    ClassDef(Femto1DCFPainter, 0)
  };
}  // namespace Hal

#endif /* HAL_ANALYSIS_PAINTERS_FEMTO1DCFPAINTER_H_ */
